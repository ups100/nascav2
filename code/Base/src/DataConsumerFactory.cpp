/**
 * @file  DataConsumerFactory.h
 * @brief  Implementation of the Class DataConsumerFactory
 * @date  Created on:      13-sie-2013 17:38:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "DataConsumerFactory.h"

using INZ_project::Base::DataConsumerFactory;

DataConsumerFactory::DataConsumerFactory()
{

}

DataConsumerFactory::~DataConsumerFactory()
{

}

DataConsumer* DataConsumerFactory::getDataConsumer()
{

    return NULL;
}

DataConsumerFactory* DataConsumerFactory::getInstance()
{

    return NULL;
}

void DataConsumerFactory::registerDataConsumer(DataConsumer* consumer,
        const QString& id)
{

}