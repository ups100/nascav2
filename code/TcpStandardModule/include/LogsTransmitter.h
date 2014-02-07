/**
 * @file  LogsTransmitter.h
 * @brief  Definition of the Class INZ_project::TcpStandardModule::LogsTransmitter
 * @date  08-12-2013 13:56:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __LOGS_TRANSMITTER_H__
#define __LOGS_TRANSMITTER_H__

#include "SessionPart.h"
#include "MessageSink.h"

#include <boost/shared_ptr.hpp>
#include <QTimer>
#include <QMap>
#include <QList>

namespace INZ_project {
namespace Base {
class DataProvider;
class DataChannel;
}

namespace TcpStandardModule {

/**
 * @brief Session Part responsible for choosing suitable protocol version
 */
class LogsTransmitter : public SessionPart
{
Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param[in] provider to which this object belongs to
     * @param[in] session to which this part belongs to
     * @param[in] milis timeout for communication with client
     */
    LogsTransmitter(Base::DataProvider* provider, Session *session = 0L, int milis = 10000);

    /**
     * @brief Destructor
     */
    ~LogsTransmitter();

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
     * @brief Slot executed when logs has been written to channel
     */
    void portionWritten(qint64 id);

private:

    /**
     * @brief Translate GMT time to local
     * @param[in] time in GMT
     * @return time in local
     */
    time_t translateTime(qint64 time);

    /**
     * @brief Gets the QString with contains left most characters till '\0' sign
     */
    QString getNullTerminatedString(const QByteArray& array);

    /**
     * @brief Function executed when logs portion has arrived
     * @param[in] message received from client
     */
    void logsPortionReceived(const QByteArray& message);

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

    enum {
        WAITING_FOR_LOG,
        WRITTING_LOG,
        NONE
    } m_state;

    Base::DataChannel *m_channel;

    std::pair<QByteArray, qint64> m_request;

    Base::DataProvider *m_provider;
};

} //namespace TcpStandardModule
} //namespace INZ_project

#endif //__LOGS_TRANSMITTER_H__
