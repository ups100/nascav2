/**
 * @file  ClientCommunicationFactory.h
 * @brief  Implementation of the Class ClientCommunicationFactory
 * @date  Created on:      13-sie-2013 17:38:10
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "ClientCommunicationFactory.h"

using INZ_project::TcpStandardModule::ClientCommunicationFactory;

ClientCommunicationFactory::ClientCommunicationFactory()
{

}

ClientCommunicationFactory::~ClientCommunicationFactory()
{

}

ClientCommunication* ClientCommunicationFactory::getClientCommunication(
        const QString& version)
{

    return NULL;
}

ClientCommunicationFactory* ClientCommunicationFactory::getInstance()
{

    return NULL;
}

QList<QString> ClientCommunicationFactory::getProtocolVersions()
{

    return NULL;
}