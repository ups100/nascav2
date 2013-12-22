/**
 * @file  AsymetricAlgorithm.h
 * @brief  Definition of the Class INZ_project::Cryptographic::AsymetricAlgorithm
 * @date  13-sie-2013 17:38:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_EB7D702C_2C96_4fe9_AC1C_5DC5DDE2C809__INCLUDED_)
#define EA_EB7D702C_2C96_4fe9_AC1C_5DC5DDE2C809__INCLUDED_

#include <QByteArray>
#include <stdexcept>

namespace INZ_project {
namespace Cryptographic {

/**
 * @brief Base class for all asymmetric algorithms
 * @warning This class is not thread-safe.
 */
class AsymetricAlgorithm
{

public:
    /**
     * @brief Constructor
     */
    AsymetricAlgorithm();

    /**
     * @brief Destructor
     */
    virtual ~AsymetricAlgorithm();

    /**
     * @brief Creates a deep copy of this object
     * @return deep copy of this object
     */
    virtual AsymetricAlgorithm* clone() = 0;

    /**
     * @brief Decrypt a message using set private key
     * @param[in] encrypted message
     * @param[in] usePublic use public key instead of private
     * @return decrypted message
     */
    virtual QByteArray decrypt(const QByteArray& encrypted) = 0;

    /**
     * @brief Encrypts a message using set public key
     * @param[in] plain text message to be encrypted
     * @param[in] usePrivate key instead of public
     * @return encrypted message
     */
    virtual QByteArray encrypt(const QByteArray& plain) = 0;

    /**
     * @brief Generates a pair of public and private key
     * @param[in] keySize - size of the key to be generated (in bytes)
     * @return Public key
     * @note For proper functionality all subclass should
     * write keys value to m_privateKey and m_publicKey
     */
    virtual const QByteArray& generateKeys(int keySize = -1) = 0;

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
     * @brief Exception class for errors
     */
    class AsymmetricAlgorithmException : public std::runtime_error
    {
    public:
        /**
         * @brief Constructor
         * @param[in] arg exception string
         */
        explicit AsymmetricAlgorithmException(const std::string& arg)
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
#endif // !defined(EA_EB7D702C_2C96_4fe9_AC1C_5DC5DDE2C809__INCLUDED_)
