/**
 * @file  RSASSA_PKCSv15_SHA256.h
 * @brief  Definition of the Class INZ_project::Cryptographic::RSASSA_PKCSv15_SHA256
 * @date  21-11-2013 21:38:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __CRYPTOGRAPHIC_RSASSA_PKCSv15_SHA256_ALGORITHM_H__
#define __CRYPTOGRAPHIC_RSASSA_PKCSv15_SHA256_ALGORITHM_H__

#include "SignAlgorithmRegister.h"
#include <crypto++/rsa.h>
#include <crypto++/sha.h>

namespace INZ_project {
namespace Cryptographic {

/**
 * @brief Sign algorithm using RSA with PKCSv15 padding and SHA256
 */
SIGN_ALGORITHM( RSASSA_PKCSv15_SHA256 , RSASSA_PKCSv15_SHA256)
{

public:
    /**
     * @brief Constructor
     */
    RSASSA_PKCSv15_SHA256();

    RSASSA_PKCSv15_SHA256(const QByteArray& publicKey,
            const QByteArray& privateKey);

    RSASSA_PKCSv15_SHA256(const QByteArray& key,
            bool isPublic);

    /**
     * @brief Destructor
     */
    virtual ~RSASSA_PKCSv15_SHA256();

    virtual SignAlgorithm* clone();

    virtual QByteArray sign(const QByteArray& message);

    virtual bool verify(const QByteArray& message, const QByteArray& sign);

    virtual void setPublicKey(const QByteArray& partnerKey);

    virtual void setPrivateKey(const QByteArray& partnerKey);

private:
    CryptoPP::RSA::PrivateKey m_rawPrivateKey;
    CryptoPP::RSA::PublicKey m_rawPublicKey;

    CryptoPP::RSASS<CryptoPP::PKCS1v15, CryptoPP::SHA256>::Signer m_signer;
    CryptoPP::RSASS<CryptoPP::PKCS1v15, CryptoPP::SHA256>::Verifier m_verifier;
};

} //namespace Cryptographic
} //namespace INZ_project
#endif // __CRYPTOGRAPHIC_SHA256_ALGORITHM_H__
