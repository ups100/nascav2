/**
 * @file  MessageHasher.h
 * @brief  Definition of the Class INZ_project::TcpStandardModule::MessageHasher
 * @date  17-11-2013 11:09:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __MESSAGE_HASHER_H__
#define __MESSAGE_HASHER_H__

#include <QObject>
#include <boost/shared_ptr.hpp>

#include "HashAlgorithm.h"

#include "MessageDecorator.h"

namespace INZ_project {
namespace TcpStandardModule {

class MessageHasher : public MessageDecorator
{
Q_OBJECT
    /**
     * @brief Constructor
     * @param[in] messageSink sink directly below this object
     * @param[in] alg algorithm to be used for hashCreation
     * @note This object takes the ownership of passed object
     */
    MessageHasher(boost::shared_ptr<MessageSink> messageSink,
            Cryptographic::HashAlgorithm *alg);

protected:
    virtual void transformReadData(const QByteArray& message);

    virtual QByteArray transformWriteData(const QByteArray& message);

    virtual void closeThisSink();

private:
    /**
     * @brief Used Hash algorithm
     */
    boost::shared_ptr<Cryptographic::HashAlgorithm> m_hash;
};

} //namespace TcpStandardModule
} //namespace INZ_project

#endif //__MESSAGE_HASHER_H__
