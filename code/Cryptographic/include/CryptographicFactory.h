/**
 * @file  CryptographicFactory.h
 * @brief  Definition of the Class INZ_project::Cryptographic::CryptographicFactory
 * @date  13-sie-2013 17:38:10
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_27CC8965_95B9_4981_8DB5_2701B8B1050C__INCLUDED_)
#define EA_27CC8965_95B9_4981_8DB5_2701B8B1050C__INCLUDED_

#include <QString>
#include <QList>
#include <QHash>
#include <QMutex>
#include <boost/function.hpp>

namespace INZ_project {
namespace Cryptographic {

class SymetricAlgorithm;
class AsymetricAlgorithm;
class HashAlgorithm;

/**
 * @brief Factory class for cryptographic modules
 */
class CryptographicFactory
{

public:
    /**
     * @brief Destructor
     */
    virtual ~CryptographicFactory();

    /**
     * @brief Gets the list of accessible Asymmetric algorithms
     * @return List of accessible Asymmetric algorithms
     */
    static QList<QString> getAsymAlgorithmList();

    /**
     * @brief Gets the Asymmetric registered with passed id
     * @param[in] id of the object
     * @return Object instance or NULL if no such object registered.
     */
    static AsymetricAlgorithm* getAsymAlgorithm(const QString& id);

    /**
     * @brief Gets the instance of this object.
     * @return Instance of this object
     */
    static CryptographicFactory* getInstance();

    /**
     * @brief Gets the list of accessible Asymmetric algorithms
     * @return List of accessible Asymmetric algorithms
     */
    static QList<QString> getSymAlgorithmList();

    /**
     * @brief Gets the Symmetric registered with passed id
     * @param[in] id of the object
     * @return Object instance or NULL if no such object registered.
     */
    static SymetricAlgorithm* getSymAlgorithm(const QString& id);

    /**
     * @brief Gets the list of accessible Asymmetric algorithms
     * @return List of accessible Asymmetric algorithms
     */
    static QList<QString> getHashAlgorithmList();

    /**
     * @brief Gets the Hash algorithm registered with passed id
     * @param[in] id of the object
     * @return Object instance or NULL if no such object registered.
     */
    static HashAlgorithm* getHashAlgorithm(const QString& id);

    /**
     * @brief Registers a passed function as creator function for object with this id
     * @param[in] function to be registered
     * @param[in] id of object to be registered
     * @note If object with such id already exist, the creator function
     *  is being overwritten and log is created
     */
    static void registerFunc(boost::function<AsymetricAlgorithm* ()> function,
            const QString& id);

    /**
     * @brief Registers a passed function as creator function for object with this id
     * @param[in] function to be registered
     * @param[in] id of object to be registered
     * @note If object with such id already exist, the creator function
     *  is being overwritten and log is created
     */
    static void registerFunc(boost::function<SymetricAlgorithm* ()> function,
            const QString& id);

    /**
     * @brief Registers a passed function as creator function for object with this id
     * @param[in] function to be registered
     * @param[in] id of object to be registered
     * @note If object with such id already exist, the creator function
     *  is being overwritten and log is created
     */
    static void registerFunc(boost::function<HashAlgorithm* ()> function,
            const QString& id);
private:
    /**
     * @brief Constructor
     */
    CryptographicFactory();

    /**
     * @brief Helper function to provide class instance
     */
    static CryptographicFactory *getInstanceHelper();

    /**
     * @brief Mutex for instance getter synchronization
     */
    static QMutex m_mutex;

    /**
     * @brief Mutex for Asymmetric Algorithms collection
     */
    QMutex m_mutexAsym;

    /**
     * @brief Mutex for Symmetric Algorithms collection
     */
    QMutex m_mutexSym;

    /**
     * @brief Mutex for Hash Algorithms collection
     */
    QMutex m_mutexHash;

    /**
     * @brief Instance of this class.
     */
    static CryptographicFactory* m_instance;

    /**
     * @brief Registered Asymmetric Algorithms
     */
    QHash<QString, boost::function<AsymetricAlgorithm* ()> > m_asym;

    /**
     * @brief Registered Symmetric Algorithms
     */
    QHash<QString, boost::function<SymetricAlgorithm* ()> > m_sym;

    /**
     * @brief Registered Hash Algorithms
     */
    QHash<QString, boost::function<HashAlgorithm* ()> > m_hash;
};

} //namespace Cryptographic
} //namespace INZ_project
#endif // !defined(EA_27CC8965_95B9_4981_8DB5_2701B8B1050C__INCLUDED_)
