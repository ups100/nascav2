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

#include "AsymetricAlgorithm.h"
#include "HashAlgorithm.h"

#include "MessageDecorator.h"

namespace INZ_project {
namespace TcpStandardModule {

class MessageSigner : public MessageDecorator
{
Q_OBJECT
    /**
     * @brief Constructor
     * @param[in] messageSink sink directly below this object
     * @param[in] hash algorithm to be used for hash creation
     * @param[in] crypt asymmetric algorithm to be used for encryption
     * @param[in] usePrivate indicates whether private key should be used for encryption
     * @note This object takes the ownership of passed object
     */
    MessageSigner(boost::shared_ptr<MessageSink> messageSink,
            Cryptographic::HashAlgorithm *hash,
            Cryptographic::AsymetricAlgorithm *crypt, bool usePrivate);

protected:
    virtual void transformReadData(const QByteArray& message);

    virtual QByteArray transformWriteData(const QByteArray& message);

    virtual void closeThisSink();

private:
    /**
     * @brief Used Hash algorithm
     */
    boost::shared_ptr<Cryptographic::HashAlgorithm> m_hash;

    /**
     * @brief Used Hash cryptography
     */
    boost::shared_ptr<Cryptographic::AsymetricAlgorithm> m_crypto;

    bool m_usePrivate;
};

} //namespace TcpStandardModule
} //namespace INZ_project

#endif //__MESSAGE_SIGNER_H__
