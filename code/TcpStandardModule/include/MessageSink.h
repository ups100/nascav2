/**
 * @file  MessageSink.h
 * @brief  Definition of the Class INZ_project::TcpStandardModule::MessageSink
 * @date  17-11-2013 10:56:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __MESSAGE_SINK_H__
#define __MESSAGE_SINK_H__

#include <QObject>
#include <QByteArray>

namespace INZ_project {
namespace TcpStandardModule {

class MessageSink : public QObject
{
Q_OBJECT
    ;
public:
    /**
     * @brief Constructor
     */
    MessageSink();

    /**
     * @brief Destructor
     */
    virtual ~MessageSink();

    enum SinkError
    {
        WrongFormat,
        WrongData,
        MessageCorrupted
    };

    /**
     * @brief Reads the content of the sink
     * @return Content of the sink or empty QByteArray if no data or error occurred.
     */
    virtual QByteArray read() = 0;

    /**
     * @brief Writes the content of message to a sink
     * @return Number of bytes written or value below 0 if an error occurred.
     */
    virtual qint64 write(const QByteArray& message) = 0;

    /**
     * @brief Indicates weather there is a message to be read
     * @return Number of messages to be read
     */
    virtual qint64 messageAvailable() = 0;

public slots:
    /**
     * @brief Closes the sink
     */
    virtual void close() = 0;

signals:
    /**
     * @brief Signal emitted when a sink has been closed
     */
    void sinkClosed();
    /**
     * @brief Signal emitted if an error occurred.
     */
    void error(MessageSink::SinkError error);
    /**
     * @brief Signal emitted once every time new data is available.
     */
    void readyRead();
};

} //namespace TcpStandardModule
} //namespace INZ_project

#endif // __MESSAGE_SINK_H__
