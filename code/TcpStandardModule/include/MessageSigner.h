/**
 * @file  MessageSigner.h
 * @brief  Definition of the Class INZ_project::TcpStandardModule::MessageSigner
 * @date  17-11-2013 11:09:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __MESSAGE_SIGNER_H__
#define __MESSAGE_SIGNER_H__

#include <QObject>
#include <boost/shared_ptr.hpp>

#include "SignAlgorithm.h"

#include "MessageDecorator.h"

namespace INZ_project {
namespace TcpStandardModule {

class MessageSigner : public MessageDecorator
{
Q_OBJECT
    /**
     * @brief Constructor
     * @param[in] messageSink sink directly below this object
     * @param[in] signer object which signs the messages
     * @note This object takes the ownership of passed object
     */
    MessageSigner(boost::shared_ptr<MessageSink> messageSink,
            Cryptographic::SignAlgorithm *signer);

protected:
    virtual void transformReadData(const QByteArray& message);

    virtual QByteArray transformWriteData(const QByteArray& message);

    virtual void closeThisSink();

private:
    /**
     * @brief Used signature algorithm
     */
    boost::shared_ptr<Cryptographic::SignAlgorithm > m_signer;
};

} //namespace TcpStandardModule
} //namespace INZ_project

#endif //__MESSAGE_SIGNER_H__
