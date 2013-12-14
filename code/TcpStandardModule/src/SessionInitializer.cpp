/**
 * @file  SessionInitializer.cpp
 * @brief  Definition of the Class INZ_project::TcpStandardModule::SessionPart
 * @date  08-12-2013 13:56:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "MessageSink.h"
#include "SessionInitializer.h"
#include "MessageFormer.h"
#include "MessageCodes.h"
#include "LogEntry.h"
#include "Session.h"

namespace INZ_project {
namespace TcpStandardModule {

SessionInitializer::SessionInitializer(
        const QMap<QString, QList<boost::shared_ptr<SessionPart> > >& allowedVersions,
        Session *session, int milis)
        : SessionPart(session, milis), m_isStarted(false),
                m_allowedVersions(allowedVersions)
{
    m_timer.setInterval(milis);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(requestTimedOut()));

}

SessionInitializer::~SessionInitializer()
{

}

SessionPart *SessionInitializer::clone()
{
    return m_isStarted ? 0L : new SessionInitializer(m_allowedVersions, m_session,
            m_timer.interval());
}

void SessionInitializer::execute()
{
    if (m_isStarted) {
        return;
    }

    m_isStarted = true;

    MessageSink *newSink = new MessageFormer(m_session->getDestination());
    m_session->setDestination(boost::shared_ptr<MessageSink>(newSink));
    boost::shared_ptr<MessageSink> destination = m_session->getDestination();

    connect(destination.get(), SIGNAL(readyRead()), this,
            SLOT(readNextMessage()));
    connect(destination.get(), SIGNAL(error(MessageSink::SinkError)), this,
            SLOT(sinkError(MessageSink::SinkError)));
    connect(destination.get(), SIGNAL(sinkClosed()), this, SLOT(sinkClosed()));

    QByteArray message = MessageCodes::getMessageCode(MessageCodes::HELLO);
    destination->write(message);

    m_timer.start();

}

void SessionInitializer::terminate()
{
    disconnectAll();

    emit finished(false);
}

void SessionInitializer::sinkError(MessageSink::SinkError error)
{
    LOG_ENTRY(MyLogger::DEBUG, "Error #"<<error<<"in MessageSink.");
    disconnectAll();

    emit finished(false);
}

void SessionInitializer::readNextMessage()
{
    m_timer.stop();
    boost::shared_ptr<MessageSink> destination = m_session->getDestination();
    QByteArray message = destination->read();

    switch (MessageCodes::getMessageType(message)) {
        case MessageCodes::REQUEST_PROTOCOL: {
            //we got the message
            QString protocol = message.right(message.length() - 1);

            //check if this version is supported
            if (m_allowedVersions.contains(protocol)) {
                //Correct version so let's prepare rest of the session
                foreach(boost::shared_ptr<SessionPart> part, m_allowedVersions[protocol]) {
                    m_session->appendSessionPart(part->clone());
                }

                //let's notify client that everything is ok and this session part is done
                destination->write(
                        MessageCodes::getMessageCode(MessageCodes::ACK));
                //close the session
                disconnectAll();
                emit finished(true);
            } else {
                //Incorrect version, try once again
                LOG_ENTRY(MyLogger::DEBUG,
                        "Unsupported protocol version: "<<protocol<<" Trying again.");
                destination->write(
                        MessageCodes::getMessageCode(MessageCodes::NACK));
                m_timer.start();
            }
            break;
        }
        default:
            LOG_ENTRY(MyLogger::DEBUG,
                    "Unexpected message: #"<<MessageCodes::getMessageType(message) <<" received from client.");
            //close the session
            disconnectAll();
            emit finished(false);
            break;
    }
}

void SessionInitializer::sinkClosed()
{
    LOG_ENTRY(MyLogger::DEBUG, "Sink closed unexpectedly.");
    disconnectAll();

    emit finished(false);
}

void SessionInitializer::requestTimedOut()
{
    LOG_ENTRY(MyLogger::DEBUG, "No response from client. Timeout.");
    disconnectAll();

    emit finished(false);
}

void SessionInitializer::disconnectAll()
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

void SessionInitializer::setTimeout(int milis)
{
    if(!m_isStarted) {
        m_timer.setInterval(milis);
    }
}

} //namespace TcpStandardModule
} //namespace INZ_project
