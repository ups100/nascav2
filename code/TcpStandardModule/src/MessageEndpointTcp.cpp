#include "MessageEndpointTcp.h"
#include "MessageSink.h"

namespace INZ_project {
namespace TcpStandardModule {

MessageEndpointTcp::MessageEndpointTcp(QTcpSocket *socket)
        : m_socket(socket)
{
    connect(m_socket.get(), SIGNAL(readyRead()), this, SIGNAL(readyRead()));
    connect(m_socket.get(), SIGNAL(disconnected()), this, SLOT(close()));
    connect(m_socket.get(), SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(translateError(QAbstractSocket::SocketError)));
}

QByteArray MessageEndpointTcp::read()
{
    return m_socket->readAll();
}

qint64 MessageEndpointTcp::write(const QByteArray& message)
{
    return m_socket->write(message);
}

qint64 MessageEndpointTcp::messageAvailable()
{
    return m_socket->bytesAvailable();
}

void MessageEndpointTcp::close()
{
    if(m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->disconnect();
    } else {
        m_socket->close();
        emit sinkClosed();
    }
}

void MessageEndpointTcp::disconnected()
{
    m_socket->close();
    emit sinkClosed();
}

void MessageEndpointTcp::translateError(QAbstractSocket::SocketError e)
{

}

} //namespace TcpStandardModule
} //namespace INZ_project
