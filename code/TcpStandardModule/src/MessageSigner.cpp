/**
 * @file  MessageSigner.cpp
 * @brief  Implementation of the Class INZ_project::TcpStandardModule::MessageSigner
 * @date  23-11-2013 11:09:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */
#include <QtEndian>

#include "MessageSigner.h"
#include "LogEntry.h"

namespace INZ_project {

using namespace Cryptographic;

namespace TcpStandardModule {

MessageSigner::MessageSigner(boost::shared_ptr<MessageSink> messageSink,
        Cryptographic::HashAlgorithm *hash,
        Cryptographic::AsymetricAlgorithm *crypt, bool usePrivate)
        : MessageDecorator(messageSink), m_hash(hash), m_crypto(crypt),
                m_usePrivate(usePrivate)
{

}

void MessageSigner::transformReadData(const QByteArray& message)
{
    QByteArray rawSize = message.left(4);
    qint32 size = qFromBigEndian<qint32>((const uchar*) rawSize.data());
    if ((size <= 0) || (message.size() - 4 - size <= 0)) {
        //message is not well-formed
        LOG_ENTRY(MyLogger::ERROR,
                "Wrong message format. Total size: "<<message.size()<<" Hash Size: "<<size);
        ///////////////////////////////////
        //emit SocketError() TODO
        ///////////////////////////////////
    } else {
        //we have the message now it's time to decrypt the sign
        QByteArray rawMessage = message.right(message.size() - 4);
        QByteArray rawHash = rawMessage.right(size);
        try {
            QByteArray hash = m_crypto->decrypt(rawHash);
            if (m_hash->verifyHash(rawMessage.left(rawMessage.size() - size),
                    hash)) {
                appendToBuffer(rawMessage.left(rawMessage.size() - size));
            } else {
                LOG_ENTRY(MyLogger::ERROR,
                        "Hash verification failed. Message corrupted.");
                ///////////////////////////////////
                //emit SocketError() TODO
                ///////////////////////////////////
            }
        } catch (const HashAlgorithm::HashAlgorithmException &e) {
            LOG_ENTRY(MyLogger::ERROR,
                    "Unable to validate a message: "<<message<<" because: "<<e.what());
            ///////////////////////////////////
            //emit SocketError() TODO
            ///////////////////////////////////
        } catch (const AsymetricAlgorithm::AsymmetricAlgorithmException &e) {
            LOG_ENTRY(MyLogger::ERROR,
                    "Unable to decrypt a hash: "<<rawHash<<" because: "<<e.what());
            ///////////////////////////////////
            //emit SocketError() TODO
            ///////////////////////////////////
        }
    }
}

QByteArray MessageSigner::transformWriteData(const QByteArray& message)
{
    QByteArray hash;
    qint32 hashSize;
    try {
        hash = m_hash->generateHash(message);
        hash = this->m_crypto->encrypt(hash, m_usePrivate);
        hashSize = hash.size();
    } catch (const HashAlgorithm::HashAlgorithmException &e) {
        LOG_ENTRY(MyLogger::ERROR,
                "Unable to hash a message: "<<message<<" because: "<<e.what());
        ///////////////////////////////////
        //emit SocketError() TODO
        ///////////////////////////////////
    } catch (const AsymetricAlgorithm::AsymmetricAlgorithmException &e) {
        LOG_ENTRY(MyLogger::ERROR,
                "Unable to encrypt a message: "<<message<<" because: "<<e.what());
        ///////////////////////////////////
        //emit SocketError() TODO
        ///////////////////////////////////
    }
    hashSize = qToBigEndian(hashSize);
    return QByteArray((char *)&hashSize, 4) + message + hash;
}

void MessageSigner::closeThisSink()
{

}

} //namespace TcpStandardModule
} //namespace INZ_project
