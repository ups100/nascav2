/**
 * @file  HashAlgorithm.h
 * @brief  Definition of the Class INZ_project::Cryptographic::HashAlgorithm
 * @date  21-11-2013 17:38:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __CRYPTOGRAPHIC_Hash_ALGORITHM_H__
#define __CRYPTOGRAPHIC_Hash_ALGORITHM_H__

#include <QByteArray>
#include <stdexcept>

namespace INZ_project {
namespace Cryptographic {

/**
 * @brief Base class for all hash algorithms
 * @warning This class is not thread-safe.
 */
class HashAlgorithm
{

public:
    /**
     * @brief Constructor
     */
    HashAlgorithm();

    /**
     * @brief Destructor
     */
    virtual ~HashAlgorithm();

    /**
     * @brief Creates a deep copy of this object
     * @return deep copy of this object
     */
    virtual HashAlgorithm* clone() = 0;

    /**
     * @brief Generates a Hash for a given message
     * @param[in] message
     * @return hash of the message
     */
    virtual QByteArray generateHash(const QByteArray& message) = 0;

    /**
     * @brief Encrypts a message using set public key
     * @param[in] message
     * @param[in] hash provided by sender
     * @return true if a hash is suitable for a given message, false otherwise
     */
    virtual bool verifyHash(const QByteArray& message, const QByteArray& hash) = 0;

    /**
     * @brief Exception class for errors occurred while parsing
     */
    class HashAlgorithmException : public std::runtime_error
    {
    public:
        /**
         * @brief Constructor
         * @param[in] arg exception string
         */
        explicit HashAlgorithmException(const std::string& arg)
                : std::runtime_error(arg)
        {
        }
    };

};

} //namespace Cryptographic
} //namespace INZ_project
#endif // __CRYPTOGRAPHIC_Hash_ALGORITHM__
