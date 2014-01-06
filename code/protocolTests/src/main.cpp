#include <QTcpSocket>
#include <QtEndian>
#include "LogEntry.h"
#include "MessageCodes.h"
#include "CryptographicFactory.h"
#include "AsymetricAlgorithm.h"
#include "SignAlgorithm.h"
#include "SymetricAlgorithm.h"
#include "HashAlgorithm.h"

using INZ_project::TcpStandardModule::MessageCodes;
using INZ_project::Cryptographic::CryptographicFactory;
using INZ_project::Cryptographic::AsymetricAlgorithm;
using INZ_project::Cryptographic::SignAlgorithm;
using INZ_project::Cryptographic::SymetricAlgorithm;
using INZ_project::Cryptographic::HashAlgorithm;

int main(void)
{
    QTcpSocket socket;

    socket.connectToHost("127.0.0.1", 8888);

    if (!socket.waitForConnected()) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to connect");
        return -1;
    }

    if (!socket.waitForReadyRead()) {
        LOG_ENTRY(MyLogger::ERROR, "No data received");
        return -1;
    }

    //////////////////////
    //got the HELLO
    /////////////////////
    QByteArray message = socket.readAll();

    int size = qFromBigEndian<qint32>((const uchar*) message.data());
    LOG_ENTRY(MyLogger::INFO, "Message recived. Size: "<<size);
    message = message.mid(4);
    if (size != 1
            || MessageCodes::getMessageType(message) != MessageCodes::HELLO) {
        LOG_ENTRY(MyLogger::ERROR,
                "Received wrong message: "<<MessageCodes::getMessageType(message));
        return -1;
    }

    LOG_ENTRY(MyLogger::INFO, "Received HELLO");

    QString protocolVersion = "v1.0";

    message = MessageCodes::getMessageCode(MessageCodes::REQUEST_PROTOCOL)
            + protocolVersion.toUtf8();

    uchar rawSize[4];
    qToBigEndian((qint32) message.size(), rawSize);

    message = QByteArray((char*) rawSize, 4) + message;
    //////////////////////
    //sent REQUEST_PROTOCOL
    ///////////////////////
    socket.write(message);
    if (!socket.waitForBytesWritten()) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to send data");
        return -1;
    }
    LOG_ENTRY(MyLogger::INFO, "REQUEST_PROTOCOL sent");

    if (socket.bytesAvailable() == 0 && !socket.waitForReadyRead()) {
        LOG_ENTRY(MyLogger::ERROR, "No data received");
        return -1;
    }

    ///////////////////////
    //Got ACK
    //////////////////////
    message = socket.readAll();

    size = qFromBigEndian<qint32>((const uchar*) message.data());
    LOG_ENTRY(MyLogger::INFO, "Message recived. Size: "<<size);
    message = message.mid(4);
    if (size != 1
            || MessageCodes::getMessageType(message) != MessageCodes::ACK) {
        LOG_ENTRY(MyLogger::ERROR,
                "Received wrong message: "<<MessageCodes::getMessageType(message));
        return -1;
    }
    LOG_ENTRY(MyLogger::INFO, "Received ACK");

    ///////////
    // Let's read the private key
    //////////

    AsymetricAlgorithm *algorithm = CryptographicFactory::getAsymAlgorithm(
            "RSA_OAEP_SHA");

    if (!algorithm) {
        LOG_ENTRY(MyLogger::ERROR, "No such algorithm");
        return -1;
    }
    QByteArray key;
    QFile file("keys/Providers::Provider1.pub");
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        key = file.readAll();

        try {
            algorithm->setPublicKey(key);
            LOG_ENTRY(MyLogger::INFO,
                    "Content of "<<file.fileName()<<" used as key file.");
        } catch (AsymetricAlgorithm::AsymmetricAlgorithmException& e) {
            LOG_ENTRY(MyLogger::ERROR,
                    "Unable to use content of file: "<<file.fileName()<<" as key: "<<e.what());
        }
    } else {
        LOG_ENTRY(MyLogger::ERROR,
                "Unable to use file: "<<file.fileName()<<" as key file");
        return -1;
    }

    QString clientID = "Clients::Client1";

    message = MessageCodes::getMessageCode(MessageCodes::CLIENT_ID)
            + clientID.toUtf8();

    try {
        message = algorithm->encrypt(message);
    } catch (...) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to encrypt");
        return -1;
    }
    LOG_ENTRY(MyLogger::INFO, "");

    qToBigEndian((qint32) message.size(), rawSize);

    message = QByteArray((char*) rawSize, 4) + message;
    //////////////////////
    //sent CLIENT_ID
    ///////////////////////
    socket.write(message);
    if (!socket.waitForBytesWritten()) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to send data");
        return -1;
    }
    LOG_ENTRY(MyLogger::INFO, "CLIENT_ID sent");

    if (socket.bytesAvailable() == 0 && !socket.waitForReadyRead()) {
        LOG_ENTRY(MyLogger::ERROR, "No data received");
        return -1;
    }

    ///////////////////////
    //Got confirmation and sign of client
    //////////////////////
    message = socket.readAll();

    //prepare signer to verify
    SignAlgorithm *signer = CryptographicFactory::getSignAlgorithm(
            "RSASSA_PKCSv15_SHA256");
    signer->setPublicKey(key);

    size = qFromBigEndian<qint32>((const uchar*) message.data());
    LOG_ENTRY(MyLogger::INFO, "Message recived. Size: "<<size);
    message = message.mid(4);
    if (size <= 0
            || MessageCodes::getMessageType(message)
                    != MessageCodes::CHOOSE_ALGORITHM) {
        LOG_ENTRY(MyLogger::ERROR,
                "Received wrong message: "<<MessageCodes::getMessageType(message));
        return -1;
    }
    LOG_ENTRY(MyLogger::INFO,
            "Received CHOOSE_ALGORITHM, hash size: "<< message.mid(5, size - 5).size());

    if (!signer->verify(clientID.toUtf8(), message.mid(1, size - 1))) {
        LOG_ENTRY(MyLogger::ERROR, "Signature missmatch");
        return -1;
    }

    LOG_ENTRY(MyLogger::INFO, "Signature valid.");

    //prepare the message for choosing algorithm
    message = MessageCodes::getMessageCode(MessageCodes::CHOOSEN_ALGORITHM)
            + QString("AES_CBC").toUtf8();

    try {
        message = algorithm->encrypt(message);
    } catch (...) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to encrypt");
        return -1;
    }

    qToBigEndian((qint32) message.size(), rawSize);

    message = QByteArray((char*) rawSize, 4) + message;
    //////////////////////
    //sent CHOOSEN_Algorithm
    ///////////////////////
    socket.write(message);
    if (!socket.waitForBytesWritten()) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to send data");
        return -1;
    }
    LOG_ENTRY(MyLogger::INFO, "CHOOSE_ALGORITHM sent");

    if (socket.bytesAvailable() == 0 && !socket.waitForReadyRead()) {
        LOG_ENTRY(MyLogger::ERROR, "No data received");
        return -1;
    }

    ///////////////////////
    //Got ACK
    //////////////////////
    message = socket.readAll();

    size = qFromBigEndian<qint32>((const uchar*) message.data());
    LOG_ENTRY(MyLogger::INFO, "Message recived. Size: "<<size);
    message = message.mid(4);
    if (size != 1
            || MessageCodes::getMessageType(message) != MessageCodes::ACK) {
        LOG_ENTRY(MyLogger::ERROR,
                "Received wrong message: "<<MessageCodes::getMessageType(message));
        return -1;
    }
    LOG_ENTRY(MyLogger::INFO, "Received ACK");

    //////////////////
    //generate the key, count the hash encrytp and send it
    //////////////////

    SymetricAlgorithm *sym = CryptographicFactory::getSymAlgorithm("AES_CBC");
    HashAlgorithm *hash = CryptographicFactory::getHashAlgorithm("SHA_256");

    if (!sym || !hash) {
        LOG_ENTRY(MyLogger::ERROR, "No such algorithm");
        return -1;
    }

    sym->generateKey();
    QByteArray symetricKey = sym->getKey();
    qToBigEndian((qint32) symetricKey.size(), rawSize);

    message = MessageCodes::getMessageCode(MessageCodes::ESTABLISH_ENCRYPTION)
            + QByteArray((char*) rawSize, 4) + sym->getKey() + sym->getIv();

    QByteArray messageHash = hash->generateHash(message);

    qToBigEndian((qint32) messageHash.size(), rawSize);

    message = QByteArray((char*) rawSize, 4) + message + messageHash;

    try {
        message = algorithm->encrypt(message);
    } catch (...) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to encrypt");
        return -1;
    }

    qToBigEndian((qint32) message.size(), rawSize);
    LOG_ENTRY(MyLogger::INFO, message.size());
    message = QByteArray((char*) rawSize, 4) + message;

    socket.write(message);
    if (!socket.waitForBytesWritten()) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to send data");
        return -1;
    }
    LOG_ENTRY(MyLogger::INFO, "ESTABLISH_ENCRYPTION sent");

    if (socket.bytesAvailable() == 0 && !socket.waitForReadyRead()) {
        LOG_ENTRY(MyLogger::ERROR, "No data received");
        return -1;
    }

    /////////////////////////
    //get CHOOSE_AUTH_MODULE
    ////////////////////////
    message = socket.readAll();

    size = qFromBigEndian<qint32>((const uchar*) message.data());
    LOG_ENTRY(MyLogger::INFO, "Message received. Size: "<<size);
    message = message.mid(4);

    //check the hash
    QByteArray plainMessage = sym->decrypt(message);
    size = qFromBigEndian<qint32>((const uchar*) plainMessage.data());
    messageHash = plainMessage.right(size);
    plainMessage = plainMessage.mid(4, plainMessage.size() - 4 - size);

    if (!hash->verifyHash(plainMessage, messageHash)) {
        LOG_ENTRY(MyLogger::ERROR, "Wrong hash given");
        return -1;
    }

    if (plainMessage.size() != 1
            || MessageCodes::getMessageType(plainMessage)
                    != MessageCodes::CHOOSE_AUTH_MODULE) {
        LOG_ENTRY(MyLogger::ERROR,
                "Received wrong message: "<<MessageCodes::getMessageType(plainMessage)<<" size: "<<plainMessage.size());
        return -1;
    }
    LOG_ENTRY(MyLogger::INFO, "Received CHOOSE_AUTH_MODULE");

    ////////////////////
    // send CHOOSEN_AUTH_MODULE
    ///////////////////

    plainMessage = MessageCodes::getMessageCode(
            MessageCodes::CHOOSEN_AUTH_MODULE)
            + QString("LoginPass").toUtf8();

    messageHash = hash->generateHash(plainMessage);

    qToBigEndian((qint32) messageHash.size(), rawSize);

    message = QByteArray((char*) rawSize, 4) + plainMessage + messageHash;

    try {
        message = sym->encrypt(message);
    } catch (...) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to encrypt");
        return -1;
    }

    qToBigEndian((qint32) message.size(), rawSize);
    LOG_ENTRY(MyLogger::INFO, message.size());
    message = QByteArray((char*) rawSize, 4) + message;

    socket.write(message);
    if (!socket.waitForBytesWritten()) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to send data");
        return -1;
    }
    LOG_ENTRY(MyLogger::INFO, "CHOOSEN_AUTH_MODULE sent");

    if (socket.bytesAvailable() == 0 && !socket.waitForReadyRead()) {
        LOG_ENTRY(MyLogger::ERROR, "No data received");
        return -1;
    }

    ////////////////////////////////////////////////////////////////////////
    // LOGIN PASS AUTH
    ///////////////////////////////////////////////////////////////////////
    //get AUTH_DATA
    message = socket.readAll();

    size = qFromBigEndian<qint32>((const uchar*) message.data());
    LOG_ENTRY(MyLogger::INFO, "Message received. Size: "<<size);
    message = message.mid(4);

    //check the hash
    plainMessage = sym->decrypt(message);
    size = qFromBigEndian<qint32>((const uchar*) plainMessage.data());
    messageHash = plainMessage.right(size);
    plainMessage = plainMessage.mid(4, plainMessage.size() - 4 - size);

    if (!hash->verifyHash(plainMessage, messageHash)) {
        LOG_ENTRY(MyLogger::ERROR, "Wrong hash given");
        return -1;
    }

    if (MessageCodes::getMessageType(plainMessage) != MessageCodes::AUTH_DATA) {
        LOG_ENTRY(MyLogger::ERROR,
                "Received wrong message: "<<MessageCodes::getMessageType(plainMessage)<<" size: "<<plainMessage.size());
        return -1;
    }
    LOG_ENTRY(MyLogger::INFO, "Received AUTH_DATA");

    plainMessage = plainMessage.mid(1);

    if (plainMessage[0] != (char)0) {
        LOG_ENTRY(MyLogger::ERROR, "Received wrong auth message");
        return -1;
    }
    LOG_ENTRY(MyLogger::INFO, "Received REQUEST_LOGIN");

    ////////////////////
    // send LOGIN
    ///////////////////

    char authCode = 1;
    plainMessage = MessageCodes::getMessageCode(MessageCodes::AUTH_DATA)
            + QByteArray(&authCode, 1) + QString("LoginTestowy").toUtf8();

    messageHash = hash->generateHash(plainMessage);

    qToBigEndian((qint32) messageHash.size(), rawSize);

    message = QByteArray((char*) rawSize, 4) + plainMessage + messageHash;

    try {
        message = sym->encrypt(message);
    } catch (...) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to encrypt");
        return -1;
    }

    qToBigEndian((qint32) message.size(), rawSize);
    LOG_ENTRY(MyLogger::INFO, message.size());
    message = QByteArray((char*) rawSize, 4) + message;

    socket.write(message);
    if (!socket.waitForBytesWritten()) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to send data");
        return -1;
    }
    LOG_ENTRY(MyLogger::INFO, "AUTH_DATA sent");

    if (socket.bytesAvailable() == 0 && !socket.waitForReadyRead()) {
        LOG_ENTRY(MyLogger::ERROR, "No data received");
        return -1;
    }

    ////////////////////
    // get REQUEST_PASSWORD
    ///////////////////
    message = socket.readAll();

    size = qFromBigEndian<qint32>((const uchar*) message.data());
    LOG_ENTRY(MyLogger::INFO, "Message received. Size: "<<size);
    message = message.mid(4);

    //check the hash
    plainMessage = sym->decrypt(message);
    size = qFromBigEndian<qint32>((const uchar*) plainMessage.data());
    messageHash = plainMessage.right(size);
    plainMessage = plainMessage.mid(4, plainMessage.size() - 4 - size);

    if (!hash->verifyHash(plainMessage, messageHash)) {
        LOG_ENTRY(MyLogger::ERROR, "Wrong hash given");
        return -1;
    }

    if (MessageCodes::getMessageType(plainMessage) != MessageCodes::AUTH_DATA) {
        LOG_ENTRY(MyLogger::ERROR,
                "Received wrong message: "<<MessageCodes::getMessageType(plainMessage)<<" size: "<<plainMessage.size());
        return -1;
    }
    LOG_ENTRY(MyLogger::INFO, "Received AUTH_DATA");

    plainMessage = plainMessage.mid(1);

    if (plainMessage[0] != (char)2) {
        LOG_ENTRY(MyLogger::ERROR, "Received wrong auth message");
        return -1;
    }
    LOG_ENTRY(MyLogger::INFO, "Received REQUEST_PASSWORD");

    ////////////////////
    // send Password
    ///////////////////

    authCode = 3;
    plainMessage = MessageCodes::getMessageCode(MessageCodes::AUTH_DATA)
            + QByteArray(&authCode, 1) + QString("PasswordTestowe").toUtf8();

    messageHash = hash->generateHash(plainMessage);

    qToBigEndian((qint32) messageHash.size(), rawSize);

    message = QByteArray((char*) rawSize, 4) + plainMessage + messageHash;

    try {
        message = sym->encrypt(message);
    } catch (...) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to encrypt");
        return -1;
    }

    qToBigEndian((qint32) message.size(), rawSize);
    LOG_ENTRY(MyLogger::INFO, message.size());
    message = QByteArray((char*) rawSize, 4) + message;

    socket.write(message);
    if (!socket.waitForBytesWritten()) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to send data");
        return -1;
    }
    LOG_ENTRY(MyLogger::INFO, "AUTH_DATA sent");

    if (socket.bytesAvailable() == 0 && !socket.waitForReadyRead()) {
        LOG_ENTRY(MyLogger::ERROR, "No data received");
        return -1;
    }

    /////////////////////////
    //get WAITING_FOR_LOGS
    ////////////////////////
    message = socket.readAll();

    size = qFromBigEndian<qint32>((const uchar*) message.data());
    LOG_ENTRY(MyLogger::INFO, "Message received. Size: "<<size);
    message = message.mid(4);

    //check the hash
    plainMessage = sym->decrypt(message);
    size = qFromBigEndian<qint32>((const uchar*) plainMessage.data());
    messageHash = plainMessage.right(size);
    plainMessage = plainMessage.mid(4, plainMessage.size() - 4 - size);

    if (!hash->verifyHash(plainMessage, messageHash)) {
        LOG_ENTRY(MyLogger::ERROR, "Wrong hash given");
        return -1;
    }

    if (plainMessage.size() != 1
            || MessageCodes::getMessageType(plainMessage)
                    != MessageCodes::WAITING_FOR_LOGS) {
        LOG_ENTRY(MyLogger::ERROR,
                "Received wrong message: "<<MessageCodes::getMessageType(plainMessage)<<" size: "<<plainMessage.size());
        return -1;
    }
    LOG_ENTRY(MyLogger::INFO, "Received WAITING_FOR_LOGS");

    ////////////////////
    // send LOGS_PORTION
    ///////////////////
    plainMessage = MessageCodes::getMessageCode(MessageCodes::LOGS_PORTION)
            + QString("To jest LOG\n").toUtf8();
    messageHash = hash->generateHash(plainMessage);

    qToBigEndian((qint32) messageHash.size(), rawSize);

    message = QByteArray((char*) rawSize, 4) + plainMessage + messageHash;

    try {
        message = sym->encrypt(message);
    } catch (...) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to encrypt");
        return -1;
    }

    qToBigEndian((qint32) message.size(), rawSize);
    LOG_ENTRY(MyLogger::INFO, message.size());
    message = QByteArray((char*) rawSize, 4) + message;

    socket.write(message);
    if (!socket.waitForBytesWritten()) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to send data");
        return -1;
    }
    LOG_ENTRY(MyLogger::INFO, "LOGS_PORTION sent");

    if (socket.bytesAvailable() == 0 && !socket.waitForReadyRead()) {
        LOG_ENTRY(MyLogger::ERROR, "No data received");
        return -1;
    }

    ////////////////////
    // Receive ACK
    ///////////////////
    message = socket.readAll();

    size = qFromBigEndian<qint32>((const uchar*) message.data());
    LOG_ENTRY(MyLogger::INFO, "Message received. Size: "<<size);
    message = message.mid(4);

    //check the hash
    plainMessage = sym->decrypt(message);
    size = qFromBigEndian<qint32>((const uchar*) plainMessage.data());
    messageHash = plainMessage.right(size);
    plainMessage = plainMessage.mid(4, plainMessage.size() - 4 - size);

    if (!hash->verifyHash(plainMessage, messageHash)) {
        LOG_ENTRY(MyLogger::ERROR, "Wrong hash given");
        return -1;
    }

    if (MessageCodes::getMessageType(plainMessage) != MessageCodes::ACK) {
        LOG_ENTRY(MyLogger::ERROR,
                "Received wrong message: "<<MessageCodes::getMessageType(plainMessage)<<" size: "<<plainMessage.size());
        return -1;
    }
    LOG_ENTRY(MyLogger::INFO, "Received ACK");

    ////////////////////
    // send END
    ///////////////////

    plainMessage = MessageCodes::getMessageCode(MessageCodes::END);

    messageHash = hash->generateHash(plainMessage);

    qToBigEndian((qint32) messageHash.size(), rawSize);

    message = QByteArray((char*) rawSize, 4) + plainMessage + messageHash;

    try {
        message = sym->encrypt(message);
    } catch (...) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to encrypt");
        return -1;
    }

    qToBigEndian((qint32) message.size(), rawSize);
    LOG_ENTRY(MyLogger::INFO, message.size());
    message = QByteArray((char*) rawSize, 4) + message;

    socket.write(message);
    if (!socket.waitForBytesWritten()) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to send data");
        return -1;
    }
    LOG_ENTRY(MyLogger::INFO, "END sent");

    if (socket.bytesAvailable() == 0 && !socket.waitForReadyRead()) {
        LOG_ENTRY(MyLogger::ERROR, "No data received");
        return -1;
    }
    return 0;
}
