/**
 * @file  ConnectionManager.cpp
 * @brief  Implementation of the Class INZ_project::TcpStandardModule::ConnectionManager
 * @date  1-12-2013 11:09:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "ConnectionManager.h"
#include "LogEntry.h"
#include "MessageEndpointTcp.h"
#include "Session.h"
#include "TcpServer.h"
#include <QTimer>

namespace INZ_project {
namespace TcpStandardModule {

ConnectionManager::ConnectionManager(TcpServer *server,
        const QList<boost::shared_ptr<SessionPart> >& sessionParts)
        : m_server(server), m_sessionParts(sessionParts)
{
    connect(m_server, SIGNAL(newConnection()), this,
            SLOT(newConnectionArrived()));
}

ConnectionManager::~ConnectionManager()
{

}

void ConnectionManager::close()
{
    disconnect(m_server, SIGNAL(newConnection()), this,
            SLOT(newConnectionArrived()));

    if (!m_activeSessions.empty()) {
        LOG_ENTRY(MyLogger::DEBUG, "Asking all sessions to terminate.");
        connect(this, SIGNAL(lastSessionFinished()), this, SIGNAL(closed()));
    } else {
        emit closed();
    }
}

void ConnectionManager::newConnectionArrived()
{
    QTcpSocket *socket = m_server->nextPendingConnection();

    // Check if the socket is valid
    if (!(socket->state() == QAbstractSocket::ConnectedState)) {
        LOG_ENTRY(MyLogger::ERROR, "Got not connected socket from server");
        delete socket;
        return;
    }

    MessageEndpointTcp *source = new MessageEndpointTcp(socket);
    Session *session = new Session(source, m_sessionParts);

    connect(session, SIGNAL(sessionFinished(bool, Session*)), this,
            SLOT(sessionFinished(bool, Session *)));
    m_activeSessions.insert(session);
    LOG_ENTRY(MyLogger::INFO, "Starting new session.");
    QTimer::singleShot(0, session, SLOT(startSession()));
}

void ConnectionManager::sessionFinished(bool ok, Session *session)
{
    if (ok) {
        LOG_ENTRY(MyLogger::INFO, "Session finished successfully");
    } else {
        LOG_ENTRY(MyLogger::DEBUG, "Session finished with errors");
    }

    m_activeSessions.remove(session);
    session->deleteLater();

    if (m_activeSessions.empty()) {
        emit lastSessionFinished();
    }
}

} //namespace TcpStandardModule
} //namespace INZ_project
