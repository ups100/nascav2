/**
 * @file  DataConsumer.cpp
 * @brief  Implementation of the Class INZ_project::Base::DataConsumer
 * @date   13-09-2013 17:38:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "DataConsumer.h"

namespace INZ_project {
namespace Base {


DataConsumer::DataConsumer()
{

}

DataConsumer::~DataConsumer()
{

}

int DataConsumer::init(const QString& additionalData, const QString& consumerId,
        const QString& consumerType)
{
    m_consumerType = consumerType;
    m_consumerId = consumerId;
    return initImpl(additionalData, consumerId, consumerType);
}

int DataConsumer::initImpl(const QString& additionalData, const QString& consumerId,
        const QString& consumerType)
{
    return 0;
}

const QString& DataConsumer::getConsumerType() const
{
    return m_consumerType;
}

const QString& DataConsumer::getConsumerId() const
{
    return m_consumerId;
}
} //namespace Base
} //namespace INZ_project
