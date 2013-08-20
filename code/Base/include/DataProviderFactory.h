/**
 * @file  DataProviderFactory.h
 * @brief  Definition of the Class INZ_project::Base::DataProviderFactory
 * @date  13-sie-2013 17:38:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_23160C1D_5A21_447d_841D_C07CB33C6AC4__INCLUDED_)
#define EA_23160C1D_5A21_447d_841D_C07CB33C6AC4__INCLUDED_

#include <QString>
#include <QList>
#include <QHash>
#include <QMutex>
#include <boost/function.hpp>

namespace INZ_project {
namespace Base {

class DataProvider;

class DataProviderFactory
{

public:
    /**
     * @brief Destructor
     */
    virtual ~DataProviderFactory();

    /**
     * @brief Gets the instance of this object.
     * @return Instance of this object
     */
    static DataProviderFactory* getInstance();

    /**
     * @brief Gets the DataConsumer registered with passed id
     * @param[in] id of the object
     * @return Object instance or NULL if no such object registered.
     */
    static DataProvider* getDataProvider(const QString& id);

    /**
     * @brief Registers a passed function as creator function for object with this id
     * @param[in] function to be registered
     * @param[in] id of object to be registered
     * @note If object with such id already exist, the creator function
     *  is being overwritten and log is created
     */
    static void registerFunc(boost::function<DataProvider* ()> function,
            const QString& id);

private:
    /**
     * @brief Constructor
     */
    DataProviderFactory();

    /**
     * @brief Helper function to provide class instance
     */
    static DataProviderFactory *getInstanceHelper();

    /**
     * @brief Mutex for instance getter synchronization
     */
    static QMutex m_mutex;

    /**
     * @brief Mutex for data providers collection
     */
    QMutex m_providersMutex;

    /**
     * @brief Instance of this class.
     */
    static DataProviderFactory* m_instance;

    /**
     * @brief Registered data consumers
     */
    QHash<QString, boost::function<DataProvider* ()> > m_dataProviders;
};

} //namespace Base
} //namespace INZ_project
#endif // !defined(EA_23160C1D_5A21_447d_841D_C07CB33C6AC4__INCLUDED_)
