/**
 * @file  ClientAuthorizer.h
 * @brief  Definition of the Class INZ_project::TcpStandardModule::ClientAuthorizer
 * @date  12-12-2013 13:56:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __CLIENT_AUTHORIZER_H__
#define __CLIENT_AUTHORIZER_H__

#include "SessionPart.h"
#include "MessageSink.h"
#include "AuthConversation.h"

#include <boost/shared_ptr.hpp>
#include <QTimer>

namespace INZ_project {
namespace Base {
class ClientSession;
class DataProvider;
}
namespace TcpStandardModule {

/**
 * @brief Session Part responsible for client authorization
 */
class ClientAuthorizer : public SessionPart
{
Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param[in] session to which this part belongs to
     * @param[in] milis timeout for communication with client
     */
    ClientAuthorizer(Session *session = 0L, int milis = 10000);

    /**
     * @brief Destructor
     */
    ~ClientAuthorizer();

    virtual SessionPart * clone();

    virtual void execute();

    virtual void terminate();

    virtual void setTimeout(int milis);

private slots:

    /**
     * @brief Slot executed when error occurred in destination
     */
    void sinkError(MessageSink::SinkError error);

    /**
     * @brief Slot executed when new message has arrived
     */
    void readNextMessage();

    /**
     * @brief Slot executed when destination has been closed unexpectedly
     */
    void sinkClosed();

    /**
     * @brief Slot executed when client didn't response in given time
     */
    void requestTimedOut();

    /**
     * @brief Slot executed when authorization process has been finished
     */
    void authorizationFinished(bool result);

    /**
     * @brief Slot executed when authorization data should be passed to client
     */
    void writeAuthData();

private:
    enum
    {
        WAITING_FOR_MODULE_ID, AUTH_IN_PROGRESS, NONE
    } m_state;

    /**
     * @brief Stops the timer and disconnects all signals from destination
     */
    void disconnectAll();

    /**
     * @brief Function executed when message has been received in WAITING_FOR_MODULE_ID state.
     * @param[in] message which has been received
     */
    void moduleIdReceived(const QByteArray& message);

    /**
     * @brief Function executed when message has been received in AUTH_IN_PROGRESS state.
     * @param[in] message which has been received
     */
    void authDataReceived(const QByteArray& message);

    /**
     * @brief Informs if session part has been started
     */
    bool m_isStarted;

    /**
     * @brief Timer for waiting for response
     */
    QTimer m_timer;

    AuthConversation m_coversationInterface;
};

} //namespace TcpStandardModule
} //namespace INZ_project

#endif //__CRYPTOGRAPHY_INITIALIZER_H__
