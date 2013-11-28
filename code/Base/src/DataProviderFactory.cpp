/**
 * @file  DataProviderFactory.cpp
 * @brief  Implementation of the Class INZ_project::Base::DataProviderFactory
 * @date  Created on:      13-sie-2013 17:38:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "DataProviderFactory.h"
#include "LogEntry.h"

namespace INZ_project {
namespace Base {

QMutex DataProviderFactory::m_mutex;
DataProviderFactory* DataProviderFactory::m_instance = 0L;

DataProviderFactory::DataProviderFactory()
{

}

DataProviderFactory::~DataProviderFactory()
{

}

DataProviderFactory* DataProviderFactory::getInstance()
{
    if (m_instance == 0L) {
        QMutexLocker locker(&m_mutex);
        if (m_instance == 0L) {
            m_instance = DataProviderFactory::getInstanceHelper();
        }
    }
    return m_instance;
}

DataProvider* DataProviderFactory::getDataProvider(const QString& id)
{
    DataProviderFactory *instance = DataProviderFactory::getInstance();
    QMutexLocker locker(&instance->m_providersMutex);
    return instance->m_dataProviders.contains(id) ?
            instance->m_dataProviders[id]() : 0L;
}

QList<QString> DataProviderFactory::getDataProviderList()
{
    DataProviderFactory *instance = DataProviderFactory::getInstance();
    QMutexLocker locker(&instance->m_providersMutex);
    return instance->m_dataProviders.keys();
}

void DataProviderFactory::registerFunc(
        boost::function<DataProvider* ()> function, const QString& id)
{
    DataProviderFactory *instance = DataProviderFactory::getInstance();
    QMutexLocker locker(&instance->m_providersMutex);
    if (instance->m_dataProviders.contains(id)) {
        LOG_ENTRY(MyLogger::ERROR,
                "Object with id=" << id << " has been already registered."
                        << " Previous object overwritten.");
    }

    instance->m_dataProviders.insert(id, function);
}

DataProviderFactory *DataProviderFactory::getInstanceHelper()
{
    static DataProviderFactory instance;
    return &instance;
}

} //namespace Base
} //namespace INZ_project
