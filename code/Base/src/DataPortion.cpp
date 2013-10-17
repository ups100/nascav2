/**
 * @file  DataPortion.h
 * @brief  Implementation of the Class DataPortion
 * @date  Created on:      13-sie-2013 17:38:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "DataPortion.h"
#include "ClientSession.h"
#include "DataProvider.h"

namespace INZ_project {
namespace Base {

DataPortion::DataPortion(QList<QString>* logs, const ClientSession* client,
        const DataProvider* provider)
:m_client(client->getClientId()), m_provider(provider->getProviderId()), m_logs(logs)
{
    for(QList<QString>::iterator it = m_logs->begin();
            it != m_logs->end(); ++it) {
        if(!it->endsWith("\n")) {
            it->append("\n");
        }
        m_size += it->size();
    }

}

DataPortion::~DataPortion()
{

}

const QList<QString>& DataPortion::getIcingaFormated() const
{

    return *m_logs;
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
