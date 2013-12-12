/**
 * @file  MessageCodes.h
 * @brief  Definition of the Class INZ_project::TcpStandardModule::MessageCodes
 * @date  08-12-2013 13:56:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __MESSAGE_CODES_H__
#define __MESSAGE_CODES_H__

#include "SessionPart.h"

#include <QTimer>

namespace INZ_project {
namespace TcpStandardModule {

/**
 * @brief Stores byte codes of messages
 */
class MessageCodes
{
public:
    enum MessageType
    {
        HELLO,
        REQUEST_PROTOCOL,
        NACK,
        ACK,
        CLIENT_ID,
        CHOOSE_ALGORITHM,
        CHOOSEN_ALGORITHM,
        ESTABLISH_ENCRYPTION,
        CHOOSE_AUTH_MODULE,
        CHOOSEN_AUTH_MODULE,
        AUTH_DATA,
        WAITING_FOR_LOGS,
        LOGS_PORTION,
        END,
        INVALID_MESSAGE_TYPE
    };

    /**
     * @brief Gets the code corresponding to given message type
     * @param[in] type of the message
     * @return Message code
     */
    static QByteArray getMessageCode(enum MessageType type);

    /**
     * @brief Gets the message type using given code
     * @param[in] code of message
     * @return Message type or INVALID_MESSAGE_TYPE if no message type associated
     * with given code
     */
    static MessageType getMessageType(const QByteArray& code);

private:
    /**
     * @brief Constructor
     * @details There is no point in creating instance of this class. Use static methods
     * instead
     */
    MessageCodes();

    /**
     * @brief Codes of communicates
     */
    static char m_codes[INVALID_MESSAGE_TYPE];

};

} //namespace TcpStandardModule
} //namespace INZ_project

#endif //__MESSAGE_CODES_H__
