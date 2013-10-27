/**
 * @file  AESAlgorithmCBCMode.h
 * @brief  Definition of the Class INZ_project::Cryptographic::AESAlgorithmCBCMode
 * @date  19-10-2013 21:43:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __CRYPTOGRAPHIC_AES_ALGORITHM_CBC_MODE_H__
#define __CRYPTOGRAPHIC_AES_ALGORITHM_CBC_MODE_H__

#include "SymetricAlgorithmRegister.h"
#include <boost/shared_ptr.hpp>
#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/ccm.h>
#include <crypto++/filters.h>

namespace INZ_project {
namespace Cryptographic {

/**
 * @class AESAlgorithmCBCMode
 * @brief AES encryption using CBC method
 */
SYMETRIC_ALGORITHM(AESAlgorithmCBCMode, AES_CBC)
{
public:
    /**
     * @brief Constructor
     */
    AESAlgorithmCBCMode();

    /**
     * @brief Destructor
     */
    ~AESAlgorithmCBCMode();

    virtual SymetricAlgorithm* clone();

    virtual QByteArray decrypt(const QByteArray& encrypted);

    virtual QByteArray encrypt(const QByteArray& plain);

    virtual const QByteArray& generateKey(int keySize = -1, int blockSize = -1);

    virtual QList<int> getSuitableKeySizes();

    virtual QList<int> getSuitableBlockSizes();

    virtual void setKey(const QByteArray& key);

    virtual const QByteArray& getIv();

    virtual void setIv(const QByteArray& iv);

protected:

    /**
     * @brief Creates an object which uses given key and iv
     * @param[in] key to be used.
     * @param[in] iv Inital value vector to be used
     * @throws SymmetricAlgorithmException if arguments are not suitable
     */
    AESAlgorithmCBCMode(const QByteArray& key, const QByteArray& iv);

private:
    /**
     * @brief Copy constructor is disallowed. Use clone() method instead.
     */
    AESAlgorithmCBCMode(const AESAlgorithmCBCMode& other);


    /**
     * @brief Initial values vector
     */
    QByteArray m_iv;
    /**
     * @brief AES encryption
     */
    CryptoPP::CBC_Mode< CryptoPP::AES >::Encryption m_aesEncryption;
    /**
     * @brief AES decryption
     */
    CryptoPP::CBC_Mode< CryptoPP::AES >::Decryption m_aesDecryption;
    /**
     * @brief List of allowed key sizes
     */
    QList<int> m_allowedKeySizes;
    /**
     * @brief List of allowed blocks (and iv) sizes
     */
    QList<int> m_allowedBlockSizes;
};

} //namespace Cryptographic
} //namespace INZ_project

#endif // __CRYPTOGRAPHIC_AES_ALGORITHM_CBC_MODE_H__
