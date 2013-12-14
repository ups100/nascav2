/**
 * @file  RSAAlgorithmOAEPUsingSHA.h
 * @brief  Definition of the Class INZ_project::Cryptographic::RSAAlgorithmOAEPUsingSHA
 * @date  27-10-2013 10:13:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __CRYPTOGRAPHIC_AES_H__
#define __CRYPTOGRAPHIC_AES_H__

#include "AsymetricAlgorithmRegister.h"
#include <crypto++/rsa.h>
#include <crypto++/sha.h>
#include <crypto++/filters.h>
#include <crypto++/cryptlib.h>

namespace INZ_project {
namespace Cryptographic {

/**
 * @class RSAAlgorithmOAEPUsingSHA
 * @brief AES encryption using CBC method
 */
ASYMETRIC_ALGORITHM(RSAAlgorithmOAEPUsingSHA, RSA_OAEP_SHA)
{
public:
    /**
     * @brief Constructor
     */
    RSAAlgorithmOAEPUsingSHA();

    /**
     * @brief Destructor
     */
    ~RSAAlgorithmOAEPUsingSHA();

    virtual AsymetricAlgorithm* clone();

    virtual QByteArray decrypt(const QByteArray& encrypted, bool usePublic=false);

    virtual QByteArray encrypt(const QByteArray& plain, bool usePrivate=false);

    virtual const QByteArray& generateKeys(int keySize=-1);

    const QByteArray& getPrivateKey();

    const QByteArray& getPublicKey();

    virtual void setPublicKey(const QByteArray& partnerKey);

    virtual void setPrivateKey(const QByteArray& partnerKey);

protected:

    /**
     * @brief Creates an object which uses given keys
     * @param[in] publicKey to be used.
     * @param[in] privateKey  to be used
     * @throws AsymmetricAlgorithmException if arguments are not suitable
     */
    RSAAlgorithmOAEPUsingSHA(const QByteArray& publicKey, const QByteArray& privateKey);

    /**
     * @brief Constructs an object using private or public key
     * @param[in] key to be used
     * @param[in] isPublic indicates if key is public or private
     */
    RSAAlgorithmOAEPUsingSHA(const QByteArray& key, bool isPublic);

private:
    /**
     * @brief Copy constructor is disallowed. Use clone() method instead.
     */
    RSAAlgorithmOAEPUsingSHA(const RSAAlgorithmOAEPUsingSHA& other);

    CryptoPP::RSA::PrivateKey m_rawPrivateKey;

    CryptoPP::RSA::PublicKey m_rawPublicKey;

    CryptoPP::RSAES_OAEP_SHA_Encryptor m_encryptor;
    CryptoPP::RSAES_OAEP_SHA_Decryptor m_decryptor;
};

} //namespace Cryptographic
} //namespace INZ_project

#endif // __CRYPTOGRAPHIC_AES_H__
