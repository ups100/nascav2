/**
 * @file  CryptographyInitializer.cpp
 * @brief  Definition of the Class INZ_project::TcpStandardModule::CryptographyInitializer
 * @date  08-12-2013 13:56:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "CryptographyInitializer.h"
#include "MessageFormer.h"
#include "MessageCodes.h"
#include "LogEntry.h"
#include "Session.h"
#include "ClientSession.h"
#include "SymetricAlgorithm.h"
#include "MessageCipher.h"
#include "MessageHasher.h"

namespace INZ_project {
namespace TcpStandardModule {

CryptographyInitializer::CryptographyInitializer(
        const QSet<QString>& allowedAlgorithms,
        INZ_project::Cryptographic::AsymetricAlgorithm *asym,
        INZ_project::Base::DataProvider *provider,
        INZ_project::Cryptographic::HashAlgorithm *hash, Session *session,
        int milis)
        : SessionPart(session, milis), m_state(NONE), m_isStarted(false),
                m_allowedAlgorithms(allowedAlgorithms), m_asym(asym),
                m_hash(hash), m_clientSession(0L), m_provider(provider),
                m_sym(0L)
{
    m_timer.setInterval(milis);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(requestTimedOut()));
}

CryptographyInitializer::~CryptographyInitializer()
{
    delete m_hash;
    delete m_sym;
    delete m_clientSession;
}

SessionPart *CryptographyInitializer::clone()
{
    return m_isStarted ?
            0L :
            new CryptographyInitializer(m_allowedAlgorithms, m_asym, m_provider,
                    m_hash->clone(), m_session, m_timer.interval());
}

void CryptographyInitializer::execute()
{
    if (m_isStarted) {
        return;
    }

    m_isStarted = true;

    boost::shared_ptr<MessageSink> destination = m_session->getDestination();

    connect(destination.get(), SIGNAL(readyRead()), this,
            SLOT(readNextMessage()));
    connect(destination.get(), SIGNAL(error(MessageSink::SinkError)), this,
            SLOT(sinkError(MessageSink::SinkError)));
    connect(destination.get(), SIGNAL(sinkClosed()), this, SLOT(sinkClosed()));

    m_state = WAITING_FOR_CLIENT_ID;
}

void CryptographyInitializer::terminate()
{
    disconnectAll();

    emit finished(false);
}

void CryptographyInitializer::sinkError(MessageSink::SinkError error)
{
    LOG_ENTRY(MyLogger::DEBUG,
            "Error #"<<error<<"in MessageSink in state: "<<m_state);
    disconnectAll();

    emit finished(false);
}

void CryptographyInitializer::readNextMessage()
{
    m_timer.stop();
    QByteArray message = m_session->getDestination()->read();
    switch (m_state) {
        case WAITING_FOR_CLIENT_ID:
            clientIdReceived(message);
            break;
        case WAITING_FOR_ALGORITHM:
            algorithmReceived(message);
            break;
        case WAITING_FOR_KEY:
            keyReceived(message);
            break;
        default:
            LOG_ENTRY(MyLogger::FATAL, "We should not be here");
            m_timer.start();
            break;
    }
}

void CryptographyInitializer::clientIdReceived(const QByteArray& message)
{
    try {
        QByteArray decrypted = m_asym->decrypt(message);
        boost::shared_ptr<MessageSink> destination =
                m_session->getDestination();

        switch (MessageCodes::getMessageType(decrypted)) {
            case MessageCodes::CLIENT_ID: {
                //We have suitable message
                QString clientId = decrypted.right(decrypted.length() - 1);
                try {
                    m_clientSession = new Base::ClientSession(clientId,
                            m_provider);
                    //now we know that this client exist, so let's reply
                    QByteArray clientHash = m_hash->generateHash(
                            clientId.toUtf8());
                    QByteArray plainMessage = MessageCodes::getMessageCode(
                            MessageCodes::CHOOSE_ALGORITHM) + clientHash;
                    destination->write(m_asym->encrypt(plainMessage, true));
                    m_state = WAITING_FOR_ALGORITHM;
                    m_timer.start();

                } catch (Base::ClientSession::ClientException& e) {
                    //Unable to find such client
                    LOG_ENTRY(MyLogger::DEBUG,
                            "Error while creating client session: "<<e.what());
                    disconnectAll();
                    emit finished(false);
                }
            }
                break;
            default:
                LOG_ENTRY(MyLogger::DEBUG,
                        "Unexpected message: #"<<MessageCodes::getMessageType(message) <<" received from client.");
                //close the session
                disconnectAll();
                emit finished(false);
                break;
        }
    } catch (Cryptographic::AsymetricAlgorithm::AsymmetricAlgorithmException& e) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to use encryption: "<<e.what());
        disconnectAll();
        emit finished(false);
    }
}

void CryptographyInitializer::algorithmReceived(const QByteArray& message)
{
    try {
        QByteArray decrypted = m_asym->decrypt(message);
        boost::shared_ptr<MessageSink> destination =
                m_session->getDestination();

        switch (MessageCodes::getMessageType(decrypted)) {
            case MessageCodes::CHOOSEN_ALGORITHM: {
                //We have suitable message
                QString algorithm = decrypted.right(decrypted.length() - 1);

                m_sym = m_clientSession->getSymetricAlgorithm(algorithm);

                if (m_sym != 0L) {
                    //client has chosen suitable algorithm
                    destination->write(
                            MessageCodes::getMessageCode(MessageCodes::ACK));
                    m_state = WAITING_FOR_KEY;
                } else {
                    //unavailable algorithm
                    LOG_ENTRY(MyLogger::INFO,
                            "Algorithm: "<<algorithm<<" is unavailable for "<<m_clientSession->getClientId());

                    destination->write(
                            MessageCodes::getMessageCode(MessageCodes::NACK));
                }

                m_timer.start();
            }
                break;
            default:
                LOG_ENTRY(MyLogger::DEBUG,
                        "Unexpected message: #"<<MessageCodes::getMessageType(message) <<" received from client.");
                //close the session
                disconnectAll();
                emit finished(false);
                break;
        }
    } catch (Cryptographic::AsymetricAlgorithm::AsymmetricAlgorithmException& e) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to use encryption: "<<e.what());
        disconnectAll();
        emit finished(false);
    }
}

void CryptographyInitializer::keyReceived(const QByteArray& message)
{
    try {
        QByteArray decrypted = m_asym->decrypt(message);
        boost::shared_ptr<MessageSink> destination =
                m_session->getDestination();

        //decrypt the message
        QByteArray plainMessage = decrypted.right(decrypted.length() - 1);
        QByteArray hashLength = plainMessage.left(4);
        int length = qFromBigEndian<qint32>((const uchar*) hashLength.data());
        if (length <= 0) {
            LOG_ENTRY(MyLogger::ERROR, "Wrong message format. Size: "<<length);
            disconnectAll();
            emit finished(false);
        }
        QByteArray hash = plainMessage.right(length);
        QByteArray plain = plainMessage.mid(4,
                plainMessage.size() - 4 - length);

        if (!m_hash->verifyHash(plain, hash)) {
            LOG_ENTRY(MyLogger::ERROR, "Incorrect hash given");
            disconnectAll();
            emit finished(false);
        }

        switch (MessageCodes::getMessageType(plain)) {
            case MessageCodes::ESTABLISH_ENCRYPTION: {
                //We have suitable message, let's set the key
                try {
                    int keySize = plain[1];
                    m_sym->setKey(plain.mid(2, keySize));
                    m_sym->setIv(plain.mid(2 + keySize));

                    //Key has been successfully set, so let's set up sinks
                    boost::shared_ptr<MessageSink> cipher = boost::shared_ptr<
                            MessageSink>(new MessageCipher(destination, m_sym));
                    m_session->setDestination(cipher);
                    m_sym = 0L;

                    boost::shared_ptr<MessageSink> hasher = boost::shared_ptr<
                            MessageSink>(new MessageHasher(cipher, m_hash));
                    m_session->setDestination(hasher);
                    m_hash = 0L;

                    //inform session that it belongs to this client
                    m_session->setClient(m_clientSession);
                    m_clientSession = 0L;

                    //This session part has been finished, cryptography is set
                    disconnectAll();
                    emit finished(true);

                } catch (Cryptographic::SymetricAlgorithm::SymmetricAlgorithmException& e) {
                    //Unable to use this key with selected algorithm
                    LOG_ENTRY(MyLogger::DEBUG,
                            "Unable to use given key with selected algorithm: "<<e.what());
                    disconnectAll();
                    emit finished(false);
                }
            }
                break;
            default:
                LOG_ENTRY(MyLogger::DEBUG,
                        "Unexpected message: #"<<MessageCodes::getMessageType(message) <<" received from client.");
                //close the session
                disconnectAll();
                emit finished(false);
                break;
        }
    } catch (Cryptographic::AsymetricAlgorithm::AsymmetricAlgorithmException& e) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to use encryption: "<<e.what());
        disconnectAll();
        emit finished(false);
    } catch (Cryptographic::HashAlgorithm::HashAlgorithmException& e) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to use hash: "<<e.what());
        disconnectAll();
        emit finished(false);
    }
}

void CryptographyInitializer::sinkClosed()
{
    LOG_ENTRY(MyLogger::DEBUG, "Sink closed unexpectedly in state: "<<m_state);
    disconnectAll();

    emit finished(false);
}

void CryptographyInitializer::requestTimedOut()
{
    LOG_ENTRY(MyLogger::DEBUG,
            "No response from client in state: "<<m_state<<". Timeout.");
    disconnectAll();

    emit finished(false);
}

void CryptographyInitializer::disconnectAll()
{
    m_timer.stop();
    boost::shared_ptr<MessageSink> destination = m_session->getDestination();

    disconnect(destination.get(), SIGNAL(readyRead()), this,
            SLOT(readNextMessage()));
    disconnect(destination.get(), SIGNAL(error(MessageSink::SinkError)), this,
            SLOT(sinkError(MessageSink::SinkError)));
    disconnect(destination.get(), SIGNAL(sinkClosed()), this,
            SLOT(sinkClosed()));
    disconnect(&m_timer, SIGNAL(timeout()), this, SLOT(requestTimedOut()));
}

void CryptographyInitializer::setTimeout(int milis)
{
    if (!m_isStarted) {
        m_timer.setInterval(milis);
    }
}

} //namespace TcpStandardModule
} //namespace INZ_project