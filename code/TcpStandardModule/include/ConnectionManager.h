/**
 * @file  ConnectionManager.h
 * @brief  Definition of the Class INZ_project::TcpStandardModule::ConnectionManager
 * @date  16-11-2013 17:38:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include <QList>
#include <QSet>
#include <QObject>
#include <boost/shared_ptr.hpp>

namespace INZ_project {
namespace TcpStandardModule {

class SessionPart;
class Session;
class TcpServer;

class ConnectionManager : public QObject
{
Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param[in] server from which connections comes from
     * @param[in] sessionParts which should be executed in the beginning of each session.
     */
    ConnectionManager(TcpServer *server,
            const QList<boost::shared_ptr<SessionPart> >& sessionParts);

    /**
     * @brief Destructor
     */
    ~ConnectionManager();

public slots:
    /**
     * @brief Closes the connection manager.
     * @details closed() signal is emitted when closing has been finished
     */
    void close();

    /**
     * @brief Slot executed when new connection has arrived to server
     */
    void newConnectionArrived();

    /**
     * @brief Slot executed when one of active sessions has been finished
     */
    void sessionFinished(bool ok, Session *session);

signals:

    /**
     * @brief Signal emitted when last active session has been finished.
     */
    void lastSessionFinished();

    /**
     * @brief Signal emitted when closing of this connection manager has been finished
     */
    void closed();

private:
    /**
     * @brief Server from which incoming connections comes
     */
    TcpServer *m_server;
    /**
     * @brief Parts of session which should be executed in the beginning
     */
    QList<boost::shared_ptr<SessionPart> > m_sessionParts;
    /**
     * @brief Sessions which are currently active
     */
    QSet<Session*> m_activeSessions;
};

} //namespace TcpStandardModule
} //namespace INZ_project
