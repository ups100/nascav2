/**
 * @file  MessageDecorator.h
 * @brief  Definition of the Class INZ_project::TcpStandardModule::MessageDecorator
 * @date  17-11-2013 11:09:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __MESSAGE_DECORATOR_H__
#define __MESSAGE_DECORATOR_H__

#include <QObject>
#include <QQueue>
#include <QByteArray>
#include <boost/shared_ptr.hpp>

#include "MessageSink.h"

namespace INZ_project {
namespace TcpStandardModule {

class MessageDecorator : public MessageSink
{
Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param[in] messageSink sink directly below this object
     */
    MessageDecorator(boost::shared_ptr<MessageSink> messageSink);

    /**
     * @brief Destructor
     */
    ~MessageDecorator();

    virtual QByteArray read();

    virtual qint64 write(const QByteArray& message);

    virtual qint64 messageAvailable();

    virtual void close();

protected slots:
    /**
     * @brief Slot used to read data from sink directly below this one
     */
    void readDataFromChild();

private slots:

    /**
     * @brief Slot executed when child has been closed.
     */
    void childClosed();

protected:
    /**
     * @brief Transforms data read from direct child to form suitable for read() function.
     * @param[in] message to be transformed
     * @note Remember to store a message in read buffer using appendToBuffer() function.
     */
    virtual void transformReadData(const QByteArray& message) = 0;

    /**
     * @brief Transforms the message before writing it to next sink
     * @param[in] message to be transformed
     * @return Transformed version of the message
     */
    virtual QByteArray transformWriteData(const QByteArray& message) = 0;

    /**
     * @brief Method called to close current sink before closing the child.
     * @see close();
     */
    virtual void closeThisSink() = 0;

    /**
     * @brief Appends the message to a read buffer.
     * @param[in] message to be stored
     */
    void appendToBuffer(const QByteArray& message);

    /**
     * @brief The next sink
     */
    boost::shared_ptr<MessageSink> m_child;

private:
    /**
     * @brief Read buffer
     * @see appendToBuffer()
     */
    QQueue<QByteArray> m_buffer;
};

} //namespace TcpStandardModule
} //namespace INZ_project

#endif //__MESSAGE_DECORATOR_H__
