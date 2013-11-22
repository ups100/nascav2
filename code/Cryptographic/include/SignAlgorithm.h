/**
 * @file  SignAlgorithm.h
 * @brief  Definition of the Class INZ_project::Cryptographic::SignAlgorithm
 * @date  21-11-2013 17:38:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __CRYPTOGRAPHIC_SIGN_ALGORITHM_H__
#define __CRYPTOGRAPHIC_SIGN_ALGORITHM_H__

#include <QByteArray>
#include <stdexcept>

namespace INZ_project {
namespace Cryptographic {

/**
 * @brief Base class for all hash algorithms
 * @warning This class is not thread-safe.
 */
class SignAlgorithm
{

public:
    /**
     * @brief Constructor
     */
    SignAlgorithm();

    /**
     * @brief Destructor
     */
    virtual ~SignAlgorithm();

    /**
     * @brief Creates a deep copy of this object
     * @return deep copy of this object
     */
    virtual SignAlgorithm* clone() = 0;

    /**
     * @brief Generates a sign for a given message
     * @param[in] message
     * @return hash of the message
     */
    virtual QByteArray generateSign(const QByteArray& message) = 0;

    /**
     * @brief Encrypts a message using set public key
     * @param[in] plain text message to be encrypted
     * @param[in] usePrivate key instead of public
     * @return true if a signs is suitable for a given message, false otherwise
     */
    virtual bool verifySign(const QByteArray& message, const QByteArray& sign) = 0;

    /**
     * @brief Exception class for errors occurred while parsing
     */
    class SignAlgorithmException : public std::runtime_error
    {
    public:
        /**
         * @brief Constructor
         * @param[in] arg exception string
         */
        explicit SignAlgorithmException(const std::string& arg)
                : std::runtime_error(arg)
        {
        }
    };

};

} //namespace Cryptographic
} //namespace INZ_project
#endif // __CRYPTOGRAPHIC_SIGN_ALGORITHM__
