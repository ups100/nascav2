/**
 * @file  DataPortion.cpp
 * @brief  Implementation of the Class DataPortion
 * @date  Created on:      13-sie-2013 17:38:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "DataPortion.h"
#include "ClientSession.h"
#include "DataProvider.h"
#include <boost/shared_ptr.hpp>
#include "Log.h"

namespace INZ_project {
namespace Base {

DataPortion::DataPortion(const QList<boost::shared_ptr<Log> >& logs, const ClientSession* client,
        const DataProvider* provider)
:m_client(client->getClientId()), m_provider(provider->getProviderId()), m_logs(logs)
{
    m_size = 0;
    foreach(boost::shared_ptr<Log> log, m_logs) {
        if(!client->checkLog(*log)) {
            //no rights to submit this log
            throw NoRightsException("No rights to submit this log.");
        }

        m_size += (log->getFormatedOutput()).size();
    }
}

DataPortion::~DataPortion()
{

}

const QList<boost::shared_ptr<Log> >& DataPortion::getLogs() const
{

    return m_logs;
}

qint64 DataPortion::getSize() const
{
    return m_size;
}
const QString& DataPortion::getClient() const
{
    return m_client;
}
const QString& DataPortion::getProvider() const
{
    return m_provider;
}
} //namespace Base
} //namespace INZ_project
