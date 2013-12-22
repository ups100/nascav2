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
 * @brief Base class for all sign algorithms
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
     * @brief Generates a Hash for a given message
     * @param[in] message
     * @return hash of the message
     */
    virtual QByteArray sign(const QByteArray& message) = 0;

    /**
     * @brief Encrypts a message using set public key
     * @param[in] message
     * @param[in] hash provided by sender
     * @return true if a hash is suitable for a given message, false otherwise
     */
    virtual bool verify(const QByteArray& message, const QByteArray& sign) = 0;

    /**
     * @brief Gets the private key
     * @return Generated private key or
     * empty object if key has not been generated
     */
    const QByteArray& getPrivateKey();

    /**
     * @brief Gets the public key
     * @return Generated public key or
     * empty object if key has not been generated
     */
    const QByteArray& getPublicKey();

    /**
     * @brief Sets public key
     * @param[in] partnerKey key to be set
     * @note Default implementation sets only m_publicKey to new value
     * if subclass need to take an action when key has changed override this method
     */
    virtual void setPublicKey(const QByteArray& partnerKey);

    /**
     * @brief Sets private key
     * @param[in] partnerKey key to be set
     * @note Default implementation sets only m_publicKey to new value
     * if subclass need to take an action when key has changed override this method
     */
    virtual void setPrivateKey(const QByteArray& partnerKey);

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

protected:
    /**
     * @brief Public key
     */
    QByteArray m_publicKey;

    /**
     * @brief Private key
     */
    QByteArray m_privateKey;
};

} //namespace Cryptographic
} //namespace INZ_project
#endif // __CRYPTOGRAPHIC_Hash_ALGORITHM__
