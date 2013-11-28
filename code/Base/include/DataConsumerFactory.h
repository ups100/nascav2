/**
 * @file  DataConsumerFactory.h
 * @brief  Definition of the Class INZ_project::Base::DataConsumerFactory
 * @date  13-08-2013 17:38:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_9F993516_AE1A_4dc6_AB52_3495ECADF809__INCLUDED_)
#define EA_9F993516_AE1A_4dc6_AB52_3495ECADF809__INCLUDED_

#include <QString>
#include <QList>
#include <QHash>
#include <QMutex>
#include <boost/function.hpp>

namespace INZ_project {
namespace Base {

class DataConsumer;

class DataConsumerFactory
{

public:
    /**
     * @brief Destructor
     */
    virtual ~DataConsumerFactory();

    /**
     * @brief Gets the instance of this object.
     * @return Instance of this object
     */
    static DataConsumerFactory* getInstance();

    /**
     * @brief Gets the DataConsumer registered with passed id
     * @param[in] id of the object
     * @return Object instance or NULL if no such object registered.
     */
    static DataConsumer* getDataConsumer(const QString& id);

    /**
     * @brief Gets the list of accessible Data Consumer types
     * @return List of accessible Data Consumer types
     */
    static QList<QString> getDataConsumerList();

    /**
     * @brief Registers a passed function as creator function for object with this id
     * @param[in] function to be registered
     * @param[in] id of object to be registered
     * @note If object with such id already exist, the creator function
     *  is being overwritten and log is created
     */
    static void registerFunc(boost::function<DataConsumer* ()> function,
            const QString& id);

private:
    /**
     * @brief Constructor
     */
    DataConsumerFactory();

    /**
     * @brief Helper function to provide class instance
     */
    static DataConsumerFactory *getInstanceHelper();

    /**
     * @brief Mutex for instance getter synchronization
     */
    static QMutex m_mutex;

    /**
     * @brief Mutex for data consumers collection
     */
    QMutex m_consumersMutex;

    /**
     * @brief Instance of this class.
     */
    static DataConsumerFactory* m_instance;

    /**
     * @brief Registered data consumers
     */
    QHash<QString, boost::function<DataConsumer* ()> > m_dataConsumers;
};

} //namespace Base
} //namespace INZ_project
#endif // !defined(EA_9F993516_AE1A_4dc6_AB52_3495ECADF809__INCLUDED_)
