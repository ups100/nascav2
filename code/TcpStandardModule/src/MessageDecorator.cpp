/**
 * @file  MessageDecorator.cpp
 * @brief  Implementation of the Class INZ_project::TcpStandardModule::MessageDecorator
 * @date  17-11-2013 11:09:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "MessageDecorator.h"
#include "MessageSink.h"

namespace INZ_project {
namespace TcpStandardModule {

MessageDecorator::MessageDecorator(boost::shared_ptr<MessageSink> messageSink)
        : m_child(messageSink)
{
    connect(m_child.get(), SIGNAL(readyRead()), this, SLOT(readDataFromChild()));
    connect(m_child.get(), SIGNAL(sinkClosed()), this, SLOT(childClosed()));
    connect(m_child.get(), SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(translateError(QAbstractSocket::SocketError)));
}

MessageDecorator::~MessageDecorator()
{

}

QByteArray MessageDecorator::read()
{
    return (m_buffer.empty()) ? QByteArray() : m_buffer.dequeue();
}

qint64 MessageDecorator::write(const QByteArray& message)
{
    qint64 ret = m_child->write(transformWriteData(message));
    return ret <= 0 ? ret : message.size();
}

qint64 MessageDecorator::messageAvailable()
{
    return m_buffer.size();
}

void MessageDecorator::close()
{
    m_child->close();
}

void MessageDecorator::readDataFromChild()
{
    transformReadData(m_child->read());
}

void MessageDecorator::childClosed()
{
    closeThisSink();
    emit sinkClosed();
}

void MessageDecorator::appendToBuffer(const QByteArray& message)
{
    m_buffer.append(message);
    emit readyRead();
}

} //namespace TcpStandardModule
} //namespace INZ_project
