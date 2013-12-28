/**
 * @file  MessageFormer.cpp
 * @brief  Implementation of the Class INZ_project::TcpStandardModule::MessageFormer
 * @date  17-11-2013 11:09:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "MessageFormer.h"
#include <QtEndian>
#include "LogEntry.h"

namespace INZ_project {
namespace TcpStandardModule {

MessageFormer::MessageFormer(boost::shared_ptr<MessageSink> messageSink)
        : MessageDecorator(messageSink), m_size(-1)
{

}

void MessageFormer::transformReadData(const QByteArray& message)
{
    QByteArray buffer = message;
    while (buffer.size() != 0) {
        //Check if we know a size of this message or not
        if (m_size == -1) {
            if (m_receiveBuffer.size() + buffer.size() >= 4) {
                int size = 4 - m_receiveBuffer.size();
                m_receiveBuffer.append(buffer.left(size));
                buffer = buffer.mid(size);

                //now lets set the size
                m_size = qFromBigEndian<qint32>((const uchar*)m_receiveBuffer.data());
                m_receiveBuffer.clear();
                if(m_size <= 0) {
                    LOG_ENTRY(MyLogger::ERROR, "Wrong message format. Size: "<<m_size);
                    ///////////////////////////////////
                    //emit SocketError() TODO
                    ///////////////////////////////////
                    continue;
                }
            } else {
                m_receiveBuffer.append(buffer);
                return;
            }
        }

        //we know the size of the message but do we have a whole message?
        if(m_receiveBuffer.size() + buffer.size() >= m_size) {
            //we have the whole message so let's save it
            int size = m_size - m_receiveBuffer.size();
            m_receiveBuffer.append(buffer.left(size));
            m_size = -1;
            appendToBuffer(m_receiveBuffer);
            buffer = buffer.mid(size);

            m_receiveBuffer.clear();
        } else {
            //we don't have the whole message so let's store what we have
            m_receiveBuffer.append(buffer);
            buffer.clear();
        }
    }
}

QByteArray MessageFormer::transformWriteData(const QByteArray& message)
{
    qint32 size = message.size();
    size = qToBigEndian(size);
    return QByteArray((char *)&size, 4) + message;
}

void MessageFormer::closeThisSink()
{
    if(m_size != -1) {
        LOG_ENTRY(MyLogger::ERROR, "Closing a sink while receiving a message. Buffer :"<<m_receiveBuffer);
    }
    m_receiveBuffer.clear();
    m_size = -1;
}

} //namespace TcpStandardModule
} //namespace INZ_project
