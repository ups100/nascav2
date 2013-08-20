/**
 * @file  SymetricAlgorithm.h
 * @brief  Definition of the Class INZ_project::Cryptographic::SymetricAlgorithm
 * @date  13-sie-2013 17:38:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_7969CDE2_2366_4739_9A40_DB426E0C42FC__INCLUDED_)
#define EA_7969CDE2_2366_4739_9A40_DB426E0C42FC__INCLUDED_

#include <QByteArray>

namespace INZ_project {
namespace Cryptographic {

/**
 * @brief Base class for all symmetric algorithms
 * @warning This class is not thread-safe.
 */
class SymetricAlgorithm
{

public:
    /**
     * @brief Constructor
     */
    SymetricAlgorithm();

    /**
     * @brief Destructor
     */
    virtual ~SymetricAlgorithm();

    /**
     * @brief Creates a deep copy of this object
     * @return deep copy of this object
     */
    virtual SymetricAlgorithm* clone() = 0;

    /**
     * @brief Decrypt a message using set key
     * @param[in] encrypted message
     * @return decrypted message
     */
    virtual QByteArray decrypt(const QByteArray& encrypted) = 0;

    /**
     * @brief Encrypts a message using set public key
     * @param[in] plain text message to be encrypted
     * @return encrypted message
     */
    virtual QByteArray encrypt(const QByteArray& plain) = 0;

    /**
     * @brief Generates a key for algorithm and stores it in object
     * @return Generated key
     * @note Each subclass should write key value to m_key
     */
    virtual const QByteArray& generateKey() = 0;

    /**
     * @brief Gets the key
     * @return Generated key or
     * empty object if key has not been generated
     */
    const QByteArray& getKey();

    /**
     * @brief Sets key
     * @param[in] key to be set
     * @note Default implementation sets only m_publicKey to new value
     * if subclass need to take an action when key has changed override this method
     */
    virtual void setKey(const QByteArray& key);

protected:
    /**
     * @brief Key generated for algorithm
     */
    QByteArray m_key;
};

} //namespace Cryptographic
} //namespace INZ_project
#endif // !defined(EA_7969CDE2_2366_4739_9A40_DB426E0C42FC__INCLUDED_)
