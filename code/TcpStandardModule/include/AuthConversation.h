/**
 * @file  AuthConversation.h
 * @brief  Definition of the Class INZ_project::TcpStandardModule::AuthConversation
 * @date  13-12-2013 13:56:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __AUTH_CONVERSATION_H__
#define __AUTH_CONVERSATION_H__

#include "MessageSink.h"
#include "ConversationInterface.h"

#include <QQueue>

namespace INZ_project {
namespace TcpStandardModule {

/**
 * @brief Session Part responsible for establishing cryptography
 */
class AuthConversation : public AAA::ConversationInterface
{
Q_OBJECT
public:
    /**
     * @brief Constructor
     */
    AuthConversation();

    /**
     * @brief Destructor
     */
    virtual ~AuthConversation();

    int sendMessage(const QByteArray& message);

    virtual QByteArray getMessage();

    /**
     * @brief Appends new message to a queue of messages
     * @param[in] data which has just arrived
     */
    void appendNewData(const QByteArray& data);

    /**
     * @brief Gets next data to be written
     * @return next message to be written or empty QByteArray if no such message
     */
    QByteArray getNextDataToWrite();

signals:
    /**
     * @brief Signal emitted when there is new portion of data to be written
     */
    void newDataToWrite();

private:
    /**
     * @brief Buffer for incomming messages
     */
    QQueue<QByteArray> m_buffer;

    /**
     * @brief Buffer for outgoing messages
     */
    QQueue<QByteArray> m_out;
};

} //namespace TcpStandardModule
} //namespace INZ_project

#endif //__CRYPTOGRAPHY_INITIALIZER_H__
