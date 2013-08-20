/**
 * @file  DataConsumerFactory.cpp
 * @brief  Implementation of the Class INZ_project::Base::DataConsumerFactory
 * @date   13-sie-2013 17:38:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "DataConsumerFactory.h"
#include "LogEntry.h"

namespace INZ_project {
namespace Base {

QMutex DataConsumerFactory::m_mutex;
DataConsumerFactory* DataConsumerFactory::m_instance = 0L;

DataConsumerFactory::DataConsumerFactory()
{

}

DataConsumerFactory::~DataConsumerFactory()
{

}

DataConsumerFactory* DataConsumerFactory::getInstance()
{
    if (m_instance == 0L) {
        QMutexLocker locker(&m_mutex);
        if (m_instance == 0L) {
            m_instance = DataConsumerFactory::getInstanceHelper();
        }
    }
    return m_instance;
}

DataConsumer* DataConsumerFactory::getDataConsumer(const QString& id)
{
    DataConsumerFactory *instance = DataConsumerFactory::getInstance();
    QMutexLocker locker(&instance->m_consumersMutex);
    return instance->m_dataConsumers.contains(id) ?
            instance->m_dataConsumers[id]() : 0L;
}

void DataConsumerFactory::registerFunc(
        boost::function<DataConsumer* ()> function, const QString& id)
{
    DataConsumerFactory *instance = DataConsumerFactory::getInstance();
    QMutexLocker locker(&instance->m_consumersMutex);
    if (instance->m_dataConsumers.contains(id)) {
        LOG_ENTRY(MyLogger::ERROR,
                "Object with id=" << id << " has been already registered."
                        << " Previous object overwritten.");
    }

    instance->m_dataConsumers.insert(id, function);
}

DataConsumerFactory *DataConsumerFactory::getInstanceHelper()
{
    static DataConsumerFactory instance;
    return &instance;
}

} //namespace Base
} //namespace INZ_project
