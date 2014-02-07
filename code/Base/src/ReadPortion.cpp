/**
 * @file  ReadPortion.cpp
 * @brief Implementation of the Class INZ_project::Base::ReadPortion
 * @date  06-10-2013 21:21:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "ReadPortion.h"

namespace INZ_project {
namespace Base {

ReadPortion::ReadPortion(const QList<QString>& logs, const QString& client,
        const QString& provider)
:m_client(client), m_provider(provider)
{
    qRegisterMetaType<const ReadPortion*>("const ReadPortion*");
    m_logs = boost::shared_ptr<QList<QString> >(new QList<QString>(logs));
}

ReadPortion::ReadPortion(QList<QString>* logs, const QString& client,
        const QString& provider)
:m_client(client), m_provider(provider), m_logs(logs)
{

}


const QList<QString>& ReadPortion::getLogs() const
{
    return *m_logs;
}

const QString& ReadPortion::getClient() const
{
    return m_client;
}

const QString& ReadPortion::getProvider() const
{
    return m_provider;
}

} //namespace Base
} //namespace INZ_project
