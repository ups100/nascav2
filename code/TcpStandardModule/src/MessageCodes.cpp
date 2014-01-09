/**
 * @file  MessageCodes.cpp
 * @brief  Implementation of the Class INZ_project::TcpStandardModule::MessageCodes
 * @date  08-12-2013 13:56:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "MessageCodes.h"

namespace INZ_project {
namespace TcpStandardModule {

char MessageCodes::m_codes[INVALID_MESSAGE_TYPE] =
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
};

char MessageCodes::m_logCodes[INVALID_LOG_TYPE] =
{
        HOST_CHECK, SERVICE_CHECK
};

QByteArray MessageCodes::getMessageCode(enum MessageCodes::MessageType type)
{
    return type != INVALID_MESSAGE_TYPE ? QByteArray(&(m_codes[type]), 1) : QByteArray();
}

MessageCodes::MessageType MessageCodes::getMessageType(const QByteArray& code)
{
    MessageType ret = INVALID_MESSAGE_TYPE;
    for(int i = 0; i < INVALID_MESSAGE_TYPE; ++i) {
        if(*(code.data()) == m_codes[i]) {
            ret = (MessageType)i;
            break;
        }
    }

    return ret;
}

MessageCodes::LogType MessageCodes::getLogType(const QByteArray& type)
{
    LogType ret = INVALID_LOG_TYPE;
    for (int i = 0; i < INVALID_LOG_TYPE; ++i) {
        if (*(type.data()) == m_logCodes[i]) {
            ret = (LogType) i;
            break;
        }
    }

    return ret;
}

} //namespace TcpStandardModule
} //namespace INZ_project
