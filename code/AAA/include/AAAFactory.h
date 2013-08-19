/**
 * @file  AAAFactory.h
 * @brief  Definition of the Class INZ_project::AAA::AAAFactory
 * @date  13-sie-2013 17:38:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_DDBCF13E_F8A4_4333_BE7E_7888BEB5EDEA__INCLUDED_)
#define EA_DDBCF13E_F8A4_4333_BE7E_7888BEB5EDEA__INCLUDED_

#include <QString>
#include <QList>
#include <QHash>
#include <QMutex>
#include <boost/function.hpp>

namespace INZ_project {
namespace AAA {

class AAAModule;

/**
 * @brief Factory class for AAA modules
 */
class AAAFactory
{

public:
    /**
     * @brief Destructor
     */
    virtual ~AAAFactory();

    /**
     * @brief Gets the AAAModule registered with passed id
     * @param[in] id of the object
     * @return Object instance or NULL if no such object registered.
     */
    static AAAModule* getAAAModule(const QString& id);

    /**
     * @brief Gets the list of accessible AAAModules
     * @return List of accessible AAAModules
     */
    static QList<QString> getAAAModulesList();

    /**
     * @brief Gets the instance of this object.
     * @return Instance of this object
     */
    static AAAFactory* getInstance();

    /**
     * @brief Registers a passed function as creator function for object with this id
     * @param[in] function to be registered
     * @param[in] id of object to be registered
     * @note If object with such id already exist, the creator function
     *  is being overwritten and log is created
     */
    static void registerFunc(boost::function<AAAModule* ()> function,
            const QString& id);
private:
    /**
     * @brief Constructor
     */
    AAAFactory();

    /**
     * @brief Helper function to provide class instance
     */
    static AAAFactory *getInstanceHelper();

    /**
     * @brief Mutex for instance getter synchronization
     */
    static QMutex m_mutex;

    /**
     * @brief Mutex for AAAModules collection synchronization
     */
    QMutex m_mutexAAAModules;

    /**
     * @brief Instance of this class.
     */
    static AAAFactory* m_instance;

    /**
     * @brief Registered AAAModules
     */
    QHash<QString, boost::function<AAAModule* ()> > m_aaamodules;
};

} //namespace AAA
} //namespace INZ_project
#endif // !defined(EA_DDBCF13E_F8A4_4333_BE7E_7888BEB5EDEA__INCLUDED_)
