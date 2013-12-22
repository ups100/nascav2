/**
 * @file  CryptographicFactory.cpp
 * @brief  Implementation of the Class INZ_project::Cryptographic::CryptographicFactory
 * @date  Created on:      13-sie-2013 17:38:10
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "CryptographicFactory.h"
#include "LogEntry.h"

namespace INZ_project {
namespace Cryptographic {

QMutex CryptographicFactory::m_mutex;
CryptographicFactory* CryptographicFactory::m_instance = 0L;

CryptographicFactory::CryptographicFactory()
{

}

CryptographicFactory::~CryptographicFactory()
{

}

QList<QString> CryptographicFactory::getAsymAlgorithmList()
{
    QMutexLocker locker(&getInstance()->m_mutexAsym);
    return getInstance()->m_asym.keys();
}

AsymetricAlgorithm* CryptographicFactory::getAsymAlgorithm(const QString& id)
{

    CryptographicFactory *instance = CryptographicFactory::getInstance();
    QMutexLocker locker(&instance->m_mutexAsym);
    return instance->m_asym.contains(id) ? instance->m_asym[id]() : 0L;
}

CryptographicFactory* CryptographicFactory::getInstance()
{
    if (m_instance == 0L) {
        QMutexLocker locker(&m_mutex);
        if (m_instance == 0L) {
            m_instance = CryptographicFactory::getInstanceHelper();
        }
    }
    return m_instance;
}

QList<QString> CryptographicFactory::getSymAlgorithmList()
{
    QMutexLocker locker(&getInstance()->m_mutexSym);
    return getInstance()->m_sym.keys();
}

SymetricAlgorithm* CryptographicFactory::getSymAlgorithm(const QString& id)
{

    CryptographicFactory *instance = CryptographicFactory::getInstance();
    QMutexLocker locker(&instance->m_mutexSym);
    return instance->m_sym.contains(id) ? instance->m_sym[id]() : 0L;
}

QList<QString> CryptographicFactory::getHashAlgorithmList()
{
    QMutexLocker locker(&getInstance()->m_mutexHash);
    return getInstance()->m_hash.keys();
}

HashAlgorithm* CryptographicFactory::getHashAlgorithm(const QString& id)
{

    CryptographicFactory *instance = CryptographicFactory::getInstance();
    QMutexLocker locker(&instance->m_mutexHash);
    return instance->m_hash.contains(id) ? instance->m_hash[id]() : 0L;
}

QList<QString> CryptographicFactory::getSignAlgorithmList()
{
    QMutexLocker locker(&getInstance()->m_mutexSign);
    return getInstance()->m_sign.keys();
}

SignAlgorithm* CryptographicFactory::getSignAlgorithm(const QString& id)
{

    CryptographicFactory *instance = CryptographicFactory::getInstance();
    QMutexLocker locker(&instance->m_mutexSign);
    return instance->m_sign.contains(id) ? instance->m_sign[id]() : 0L;
}

void CryptographicFactory::registerFunc(
        boost::function<AsymetricAlgorithm* ()> function, const QString& id)
{
    CryptographicFactory *instance = CryptographicFactory::getInstance();
    QMutexLocker locker(&instance->m_mutexAsym);
    if (instance->m_asym.contains(id)) {
        LOG_ENTRY(MyLogger::ERROR,
                "Object with id="<<id<<" has been already registered." <<" Previous object overwritten.");
    }

    instance->m_asym.insert(id, function);
}

void CryptographicFactory::registerFunc(
        boost::function<SymetricAlgorithm* ()> function, const QString& id)
{
    CryptographicFactory *instance = CryptographicFactory::getInstance();
    QMutexLocker locker(&instance->m_mutexSym);
    if (instance->m_sym.contains(id)) {
        LOG_ENTRY(MyLogger::ERROR,
                "Object with id="<<id<<" has been already registered." <<" Previous object overwritten.");
    }

    instance->m_sym.insert(id, function);
}

void CryptographicFactory::registerFunc(
        boost::function<HashAlgorithm* ()> function, const QString& id)
{
    CryptographicFactory *instance = CryptographicFactory::getInstance();
    QMutexLocker locker(&instance->m_mutexHash);
    if (instance->m_hash.contains(id)) {
        LOG_ENTRY(MyLogger::ERROR,
                "Object with id="<<id<<" has been already registered." <<" Previous object overwritten.");
    }

    instance->m_hash.insert(id, function);
}

void CryptographicFactory::registerFunc(
        boost::function<SignAlgorithm* ()> function, const QString& id)
{
    CryptographicFactory *instance = CryptographicFactory::getInstance();
    QMutexLocker locker(&instance->m_mutexSign);
    if (instance->m_sign.contains(id)) {
        LOG_ENTRY(MyLogger::ERROR,
                "Object with id="<<id<<" has been already registered." <<" Previous object overwritten.");
    }

    instance->m_sign.insert(id, function);
}
CryptographicFactory *CryptographicFactory::getInstanceHelper()
{
    static CryptographicFactory instance;
    return &instance;
}

} //namespace Cryptographic
} //namespace INZ_project
