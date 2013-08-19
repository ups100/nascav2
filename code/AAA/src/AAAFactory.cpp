/**
 * @file  AAAFactory.cpp
 * @brief  Implementation of the Class INZ_project::AAA::AAAFactory
 * @date  Created on:      13-sie-2013 17:38:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "AAAFactory.h"
#include "LogEntry.h"
#include <QMutexLocker>

namespace INZ_project {
namespace AAA {

QMutex AAAFactory::m_mutex;
AAAFactory* AAAFactory::m_instance = 0L;

AAAFactory::AAAFactory()
{

}

AAAFactory::~AAAFactory()
{

}

AAAModule* AAAFactory::getAAAModule(const QString& id)
{

    AAAFactory *instance = AAAFactory::getInstance();
    QMutexLocker locker(&instance->m_mutexAAAModules);
    return instance->m_aaamodules.contains(id) ?
            instance->m_aaamodules[id]() : 0L;
}

QList<QString> AAAFactory::getAAAModulesList()
{
    QMutexLocker locker(&getInstance()->m_mutexAAAModules);
    return getInstance()->m_aaamodules.keys();
}

AAAFactory* AAAFactory::getInstance()
{
    if (m_instance == 0L) {
        QMutexLocker locker(&m_mutex);
        if (m_instance == 0L) {
            m_instance = AAAFactory::getInstanceHelper();
        }
    }
    return m_instance;
}

void AAAFactory::registerFunc(boost::function<AAAModule* ()> function,
        const QString& id)
{
    AAAFactory *instance = AAAFactory::getInstance();
    QMutexLocker locker(&instance->m_mutexAAAModules);
    if(instance->m_aaamodules.contains(id)) {
        LOG_ENTRY(MyLogger::ERROR, "Object with id="<<id<<" has been already registered."
                <<" Previous object overwritten.");
    }

    instance->m_aaamodules.insert(id, function);
}

AAAFactory *AAAFactory::getInstanceHelper()
{
    static AAAFactory instance;
    return &instance;
}

} //namespace AAA
} //namespace INZ_project
