/**
 * @file  SHA256Algorithm.h
 * @brief  Definition of the Class INZ_project::Cryptographic::SHA256Algorithm
 * @date  21-11-2013 21:38:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __CRYPTOGRAPHIC_SHA256_ALGORITHM_H__
#define __CRYPTOGRAPHIC_SHA256_ALGORITHM_H__

#include "HashAlgorithmRegister.h"
#include <crypto++/sha.h>

namespace INZ_project {
namespace Cryptographic {

/**
 * @brief Base class for all hash algorithms
 * @warning This class is not thread-safe.
 */
HASH_ALGORITHM( SHA256Algorithm , SHA_256)
{

public:
    /**
     * @brief Constructor
     */
    SHA256Algorithm();

    /**
     * @brief Destructor
     */
    virtual ~SHA256Algorithm();

    virtual HashAlgorithm* clone();

    virtual QByteArray generateHash(const QByteArray& message);

    virtual bool verifyHash(const QByteArray& message, const QByteArray& hash);

private:
    CryptoPP::SHA256 m_hash;
};

} //namespace Cryptographic
} //namespace INZ_project
#endif // __CRYPTOGRAPHIC_SHA256_ALGORITHM_H__
