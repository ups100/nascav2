/**
 * @file  ConnectionManager.cpp
 * @brief  Implementation of the Class INZ_project::TcpStandardModule::ConnectionManager
 * @date  1-12-2013 11:09:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "LogEntry.h"
#include "Session.h"
#include "SessionPart.h"
#include "MessageSink.h"
#include "ClientSession.h"
#include <QTimer>

using boost::shared_ptr;

namespace INZ_project {
namespace TcpStandardModule {

Session::Session(MessageSink *destination,
        const QList<boost::shared_ptr<SessionPart> >& parts)
        : m_result(false),m_currentSessionPart(0L), m_destination(destination)
{
    foreach(shared_ptr<SessionPart> part, parts) {
        SessionPart* sessionPart = part->clone();
        sessionPart->setSession(this);
        m_partsLeft.append(sessionPart);
    }
}

Session::~Session()
{

}

boost::shared_ptr<MessageSink> Session::getDestination()
{
    return m_destination;
}

void Session::setDestination(boost::shared_ptr<MessageSink> destination)
{
    m_destination = destination;
}

void Session::appendSessionPart(SessionPart* sessionPart)
{
    sessionPart->setSession(this);
    m_partsLeft.append(sessionPart);
}

void Session::setClient(Base::ClientSession *client)
{
    m_client = boost::shared_ptr<Base::ClientSession>(client);
}

boost::shared_ptr<Base::ClientSession> Session::getClient()
{
    return m_client;
}

void Session::stopSession()
{
    if(m_currentSessionPart) {
        QTimer::singleShot(0, m_currentSessionPart, SLOT(terminate()));
    }
}

void Session::startSession()
{
    m_currentSessionPart = m_partsLeft.takeFirst();
    connect(m_currentSessionPart, SIGNAL(finished(bool)), this,
                        SLOT(sessionPartFinished(bool)));
    QTimer::singleShot(0, m_currentSessionPart, SLOT(execute()));
}

void Session::sessionPartFinished(bool result)
{
    m_currentSessionPart->deleteLater();
    m_currentSessionPart = 0L;
    if (result) {
        if (!m_partsLeft.empty()) {
            m_currentSessionPart = m_partsLeft.takeFirst();
            connect(m_currentSessionPart, SIGNAL(finished(bool)), this,
                    SLOT(sessionPartFinished(bool)));
            QTimer::singleShot(0, m_currentSessionPart, SLOT(execute()));
        } else {
            LOG_ENTRY(MyLogger::INFO,
                    "Session finished successfully. Closing the sink.");
            m_result = true;
            m_destination->close();
        }
    } else {
        LOG_ENTRY(MyLogger::DEBUG,
                "Session finished unsuccessfully. Closing the sink.");
        clearPartsLeft();
        m_result = false;
        m_destination->close();
    }
}

void Session::destinationClosed()
{
    emit sessionFinished(m_result, this);
}

void Session::clearPartsLeft()
{
    foreach(SessionPart* part, m_partsLeft) {
        delete part;
    }
    m_partsLeft.clear();
}

} //namespace TcpStandardModule
} //namespace INZ_project
