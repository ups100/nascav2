/**
 * @file  ClientAuthorizer.cpp
 * @brief  Definition of the Class INZ_project::TcpStandardModule::ClientAuthorizer
 * @date  08-12-2013 13:56:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "ClientAuthorizer.h"
#include "MessageFormer.h"
#include "MessageCodes.h"
#include "LogEntry.h"
#include "Session.h"
#include "ClientSession.h"
#include "SymetricAlgorithm.h"

using boost::shared_ptr;
using INZ_project::Base::ClientSession;

namespace INZ_project {
namespace TcpStandardModule {

ClientAuthorizer::ClientAuthorizer(Session *session, int milis)
        : SessionPart(session, milis), m_state(NONE), m_isStarted(false)
{
    m_timer.setInterval(milis);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(requestTimedOut()));
}

ClientAuthorizer::~ClientAuthorizer()
{

}

SessionPart *ClientAuthorizer::clone()
{
    return m_isStarted ?
            0L : new ClientAuthorizer(m_session, m_timer.interval());
}

void ClientAuthorizer::execute()
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

    destination->write(
            MessageCodes::getMessageCode(MessageCodes::CHOOSE_AUTH_MODULE));
    m_timer.start();
    m_state = WAITING_FOR_MODULE_ID;
}

void ClientAuthorizer::terminate()
{
    disconnectAll();

    emit finished(false);
}

void ClientAuthorizer::sinkError(MessageSink::SinkError error)
{
    LOG_ENTRY(MyLogger::DEBUG,
            "Error #"<<error<<"in MessageSink in state: "<<m_state);
    disconnectAll();

    emit finished(false);
}

void ClientAuthorizer::readNextMessage()
{
    m_timer.stop();
    QByteArray message = m_session->getDestination()->read();
    switch (m_state) {
        case WAITING_FOR_MODULE_ID:
            moduleIdReceived(message);
            break;
        case AUTH_IN_PROGRESS:
            authDataReceived(message);
            break;
        default:
            LOG_ENTRY(MyLogger::FATAL, "We should not be here");
            m_timer.start();
            break;
    }
}

void ClientAuthorizer::moduleIdReceived(const QByteArray& message)
{
    boost::shared_ptr<MessageSink> destination = m_session->getDestination();

    switch (MessageCodes::getMessageType(message)) {
        case MessageCodes::CHOOSEN_AUTH_MODULE: {
            //We have suitable message
            QString moduleId = message.mid(1);

            shared_ptr<ClientSession> client = m_session->getClient();

            if (!client) {
                //Session part executed but there is no client
                LOG_ENTRY(MyLogger::ERROR,
                        "Client authorization in progress, but client has not been set.");
                disconnectAll();
                emit finished(false);
                break;
            }

            try {
                connect(client.get(), SIGNAL(authorizationFinished(bool)), this,
                        SLOT(authorizationFinished(bool)));
                connect(&m_coversationInterface, SIGNAL(newDataToWrite()), this,
                        SLOT(writeAuthData()));

                m_state = AUTH_IN_PROGRESS;
                client->authorize(moduleId, &m_coversationInterface);

            } catch (ClientSession::ClientException &e) {
                LOG_ENTRY(MyLogger::INFO,
                        "Unable to authorize client: "<<client->getClientId()<<" using: "<<moduleId);

                disconnect(client.get(), SIGNAL(authorizationFinished(bool)),
                        this, SLOT(authorizationFinished(bool)));
                disconnect(&m_coversationInterface, SIGNAL(newDataToWrite()),
                        this, SLOT(writeAuthData()));

                m_state = WAITING_FOR_MODULE_ID;
                m_session->getDestination()->write(
                        MessageCodes::getMessageCode(MessageCodes::NACK));

                m_timer.start();
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
}

void ClientAuthorizer::authDataReceived(const QByteArray& message)
{
    boost::shared_ptr<MessageSink> destination = m_session->getDestination();

    switch (MessageCodes::getMessageType(message)) {
        case MessageCodes::AUTH_DATA: {
            //We have auth data so we should notify authorization module
            m_coversationInterface.appendNewData(message.mid(1));
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
}

void ClientAuthorizer::authorizationFinished(bool result)
{
    shared_ptr<ClientSession> client = m_session->getClient();
    disconnect(client.get(), SIGNAL(authorizationFinished(bool)), this,
            SLOT(authorizationFinished(bool)));
    disconnect(&m_coversationInterface, SIGNAL(newDataToWrite()), this,
            SLOT(writeAuthData()));

    LOG_ENTRY(MyLogger::DEBUG,
            "Client: "<<client->getClientId()<<" authorized: "<<result);

    disconnectAll();
    emit finished(result);
}

void ClientAuthorizer::writeAuthData()
{
    m_session->getDestination()->write(
            MessageCodes::getMessageCode(MessageCodes::AUTH_DATA)
                    + m_coversationInterface.getNextDataToWrite());

    m_timer.start();
}

void ClientAuthorizer::sinkClosed()
{
    LOG_ENTRY(MyLogger::DEBUG, "Sink closed unexpectedly in state: "<<m_state);
    disconnectAll();

    emit finished(false);
}

void ClientAuthorizer::requestTimedOut()
{
    LOG_ENTRY(MyLogger::DEBUG,
            "No response from client in state: "<<m_state<<". Timeout.");
    disconnectAll();

    emit finished(false);
}

void ClientAuthorizer::disconnectAll()
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

void ClientAuthorizer::setTimeout(int milis)
{
    if (!m_isStarted) {
        m_timer.setInterval(milis);
    }
}

} //namespace TcpStandardModule
} //namespace INZ_project
