/**
 * @file  Client.h
 * @brief  Implementation of the Class Client
 * @date  Created on:      13-sie-2013 17:38:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "Client.h"
#include "DataChannel.h"

namespace INZ_project {
namespace Base {

Client::Client()
{

}

Client::~Client()
{

}

const QString& Client::getClientId()
{

    return QString();
}

const DataChannel& Client::getDataChannel()
{

    return DataChannel();
}

} //namespace Base
} //namespace INZ_project
