/**
 * @file  AuthConversation.cpp
 * @brief  Definition of the Class INZ_project::TcpStandardModule::AuthConversation
 * @date  13-12-2013 13:56:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "AuthConversation.h"
#include "LogEntry.h"

namespace INZ_project {
namespace TcpStandardModule {

AuthConversation::AuthConversation()
{

}

AuthConversation::~AuthConversation()
{
    if (!m_out.empty()) {
        LOG_ENTRY(MyLogger::DEBUG,
                "Destroying object with unsent outgoing messages.");
        m_out.clear();
    }

    if (!m_buffer.empty()) {
        LOG_ENTRY(MyLogger::DEBUG,
                "Destroying object with unread incoming messages.");
        m_buffer.clear();
    }
}

int AuthConversation::sendMessage(const QByteArray& message)
{
    m_out.enqueue(message);
    emit newDataToWrite();
    return message.size();
}

QByteArray AuthConversation::getNextDataToWrite()
{
    return m_out.empty() ? QByteArray() : m_out.dequeue();
}

QByteArray AuthConversation::getMessage()
{
    return m_buffer.empty() ? QByteArray() : m_buffer.dequeue();
}

void AuthConversation::appendNewData(const QByteArray& data)
{
    m_buffer.enqueue(data);
    emit newDataArrived();
}

} //namespace TcpStandardModule
} //namespace INZ_project
