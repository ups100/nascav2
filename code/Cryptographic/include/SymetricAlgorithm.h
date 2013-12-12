/**
 * @file  SymetricAlgorithm.h
 * @brief  Definition of the Class INZ_project::Cryptographic::SymetricAlgorithm
 * @date  13-sie-2013 17:38:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_7969CDE2_2366_4739_9A40_DB426E0C42FC__INCLUDED_)
#define EA_7969CDE2_2366_4739_9A40_DB426E0C42FC__INCLUDED_

#include <QByteArray>
#include <stdexcept>

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
     * @param[in] keySize size of a key to be generated (in bytes)
     * @param[in] blockSize to be used (in bytes)
     * @return Generated key
     * @note Each subclass should write key value to m_key
     * @see getSuitableKeySizes()
     * @see getSuitableBlockSizes()
     */
    virtual const QByteArray& generateKey(int keySize = -1,
            int blockSize = -1) = 0;

    /**
     * @brief Gets the list of suitable key sizes.
     * @return List of suitable key sizes.
     * List should have at least one item
     */
    virtual QList<int> getSuitableKeySizes() = 0;

    /**
     * @brief Gets the list of suitable block sizes.
     * @details If mode does not use blocks empty list is returned.
     * @return List if suitable block sizes
     */
    virtual QList<int> getSuitableBlockSizes() = 0;

    /**
     * @brief Gets the key
     * @return Key or empty QByteArray if key has not been generated
     */
    const QByteArray& getKey();

    /**
     * @brief Gets the initial vector
     * @return Initial vector or empty QByteArray if key has not been generated
     * @note Implement this function in each subclass. If encryption method
     * do not use Iv you should throw an instance of SymmetricAlgorithmException
     */
    virtual const QByteArray& getIv() = 0;

    /**
     * @brief Sets key
     * @param[in] key to be set
     * @note Default implementation sets only stored key to new value
     * if subclass need to take an action when key has changed override this method
     */
    virtual void setKey(const QByteArray& key);

    /**
     * @brief Sets initial vector
     * @param[in] iv to be set
     * @note Implement this function in each subclass. If encryption method
     * do not use Iv you should left this implementation empty
     */
    virtual void setIv(const QByteArray& iv) = 0;

    /**
     * @brief Exception class for errors occurred while parsing
     */
    class SymmetricAlgorithmException : public std::runtime_error
    {
    public:
        /**
         * @brief Constructor
         * @param[in] arg exception string
         */
        explicit SymmetricAlgorithmException(const std::string& arg)
                : std::runtime_error(arg)
        {
        }
    };

protected:
    /**
     * @brief Key generated for algorithm
     */
    QByteArray m_key;
};

} //namespace Cryptographic
} //namespace INZ_project
#endif // !defined(EA_7969CDE2_2366_4739_9A40_DB426E0C42FC__INCLUDED_)
