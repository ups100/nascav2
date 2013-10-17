/**
 * @file  DataProvider.cpp
 * @brief  Implementation of the Class INZ_project::Base::DataProvider
 * @date   13-08-2013 17:38:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "DataProvider.h"

namespace INZ_project {
namespace Base {

DataProvider::DataProvider()
{

}

DataProvider::~DataProvider()
{

}

int DataProvider::init(const QString& additionalData, const QString& providerId,
        const QString& providerType)
{
    m_providerType = providerType;
    m_providerId = providerId;
    return initImpl(additionalData, providerId, providerType);
}

int DataProvider::initImpl(const QString& additionalData, const QString& providerId,
        const QString& providerType)
{
    return 0;
}

const QString& DataProvider::getProviderType() const
{
    return m_providerType;
}

const QString& DataProvider::getProviderId() const
{
    return m_providerId;
}

} //namespace Base
} //namespace INZ_project
