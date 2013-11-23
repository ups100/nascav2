/**
 * @file  MessageFormer.h
 * @brief  Definition of the Class INZ_project::TcpStandardModule::MessageFormer
 * @date  17-11-2013 11:09:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __MESSAGE_FORMER_H__
#define __MESSAGE_FORMER_H__

#include <QObject>
#include <QByteArray>
#include <boost/shared_ptr.hpp>

#include "MessageDecorator.h"

namespace INZ_project {
namespace TcpStandardModule {

class MessageFormer : public MessageDecorator
{
Q_OBJECT
    /**
     * @brief Constructor
     * @param[in] messageSink sink directly below this object
     */
    MessageFormer(boost::shared_ptr<MessageSink> messageSink);

protected:
    virtual void transformReadData(const QByteArray& message);

    virtual QByteArray transformWriteData(const QByteArray& message);

    virtual void closeThisSink();

private:
    int m_size;
    QByteArray m_receiveBuffer;
};

} //namespace TcpStandardModule
} //namespace INZ_project

#endif //__MESSAGE_FORMER_H__
