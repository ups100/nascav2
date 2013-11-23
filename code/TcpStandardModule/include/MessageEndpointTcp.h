/**
 * @file  MessageEndpointTcp.h
 * @brief  Definition of the Class INZ_project::TcpStandardModule::MessageEndpointTcp
 * @date  17-11-2013 12:54:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include <QObject>
#include <QTcpSocket>
#include <boost/shared_ptr.hpp>

#include "MessageSink.h"

namespace INZ_project {
namespace TcpStandardModule {

/**
 * @brief Final sink which sends data using TCP protocol
 */
class MessageEndpointTcp : public MessageSink
{
Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param[in] socket used by this sink
     * @note This object takes the ownership of socket.
     */
    MessageEndpointTcp(QTcpSocket *socket);

    virtual QByteArray read();

    virtual qint64 write(const QByteArray& message);

    virtual qint64 messageAvailable();

    virtual void close();

private slots:
    /**
     * @brief Translates the socket errors to a sink error
     */
    void translateError(QAbstractSocket::SocketError e);

    /**
     * @brief Closes the sink if socket has been disconnected
     */
    void disconnected();

private:
    /**
     * @brief Socket used for communication
     */
    boost::shared_ptr<QTcpSocket> m_socket;
};

} //namespace TcpStandardModule
} //namespace INZ_project
