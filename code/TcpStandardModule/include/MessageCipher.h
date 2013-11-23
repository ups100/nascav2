/**
 * @file  MessageCipher.h
 * @brief  Definition of the Class INZ_project::TcpStandardModule::MessageCipher
 * @date  17-11-2013 11:09:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __MESSAGE_CIPHER_H__
#define __MESSAGE_CIPHER_H__

#include <QObject>
#include <boost/shared_ptr.hpp>

#include "AsymetricAlgorithm.h"
#include "SymetricAlgorithm.h"

#include "MessageDecorator.h"

namespace INZ_project {
namespace TcpStandardModule {

class MessageCipher : public MessageDecorator
{
Q_OBJECT
    /**
     * @brief Constructor
     * @param[in] messageSink sink directly below this object
     * @param[in] alg algorithm to be used for encryption
     * @note This object takes the ownership of passed object
     */
    MessageCipher(boost::shared_ptr<MessageSink> messageSink,
            Cryptographic::AsymetricAlgorithm *alg);

    /**
     * @brief Constructor
     * @param[in] messageSink sink directly below this object
     * @param[in] alg algorithm to be used for encryption
     * @note This object takes the ownership of passed object
     */
    MessageCipher(boost::shared_ptr<MessageSink> messageSink,
            Cryptographic::SymetricAlgorithm *alg);

protected:
    virtual void transformReadData(const QByteArray& message);

    virtual QByteArray transformWriteData(const QByteArray& message);

    virtual void closeThisSink();

private:
    /**
     * @brief Valid if asymmetric algorithm is used;
     */
    boost::shared_ptr<Cryptographic::AsymetricAlgorithm> m_asym;
    /**
     * @brief Valid if symmetric algorithm is used
     */
    boost::shared_ptr<Cryptographic::SymetricAlgorithm> m_sym;
};

} //namespace TcpStandardModule
} //namespace INZ_project

#endif //__MESSAGE_CIPHER_H__
