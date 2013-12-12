/**
 * @file  SessionInitializer.h
 * @brief  Definition of the Class INZ_project::TcpStandardModule::SessionInitializer
 * @date  08-12-2013 13:56:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __SESSION_INITIALIZER_H__
#define __SESSION_INITIALIZER_H__

#include "SessionPart.h"
#include "MessageSink.h"

#include <boost/shared_ptr.hpp>
#include <QTimer>
#include <QMap>
#include <QList>

namespace INZ_project {
namespace TcpStandardModule {

/**
 * @brief Session Part responsible for choosing suitable protocol version
 */
class SessionInitializer : public SessionPart
{
Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param[in] allowedVersions of protocol and required session parts
     * @param[in] session to which this part belongs to
     * @param[in] milis timeout for communication with client
     */
    SessionInitializer(
            const QMap<QString, QList<boost::shared_ptr<SessionPart> > >& allowedVersions,
            Session *session = 0L, int milis = 10000);

    /**
     * @brief Destructor
     */
    ~SessionInitializer();

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

private:
    /**
     * @brief Stops the timer and disconnects all signals from destination
     */
    void disconnectAll();

    /**
     * @brief Informs if session part has been started
     */
    bool m_isStarted;

    /**
     * @brief Timer for waiting for response
     */
    QTimer m_timer;

    /**
     * @brief Allowed protocol versions
     */
    QMap<QString, QList<boost::shared_ptr<SessionPart> > > m_allowedVersions;
};

} //namespace TcpStandardModule
} //namespace INZ_project

#endif //__SESSION_INITIALIZER_H__
