/**
 * @file  DataChannel.cpp
 * @brief Implementation of the Class INZ_project::Base::DataChannel
 * @date  13-09-2013 17:38:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "DataChannel.h"
#include "DataPortion.h"
#include "DataFile.h"
#include "MyNscaMain.h"
#include "LogEntry.h"
#include "Log.h"
#include <QObject>

namespace INZ_project {
namespace Base {

DataChannel::DataChannel(DataFile *file, const QSet<QString>& destinations,
        const QString& client, const QString& provider)
        : m_file(file), m_client(client), m_provider(provider)
{
    if (destinations.empty()) {
        throw BadIdException("No destination.");
    }

    foreach(QString dst, destinations) {
        DataConsumer *cons = MyNscaMain::getConsumer(dst);
        if (cons == 0L) {
            LOG_ENTRY(MyLogger::ERROR, "No such data consumer: "<<dst);
            throw BadIdException(dst.toStdString());
        } else {
            m_consumers[dst] = cons;
        }
    }

    if (m_client.at(m_client.size() - 1) != '\n') {
        m_client += "\n";
    }

    if (m_provider.at(m_provider.size() - 1) != '\n') {
        m_provider += "\n";
    }

    //Header is const so let's create one and reuse it
    QTextStream stream(&m_header);
    stream << m_client << m_provider << m_consumers.size() << '\n';
    foreach(QString dest, destinations) {
        stream << dest;
        if (dest.at(dest.size() - 1) != '\n') {
            stream << '\n';
        }
    }

}

DataChannel::~DataChannel()
{
    m_consumers.clear();
}

bool DataChannel::write(const DataPortion& portion)
{
    QList<QString> logs;
    foreach(boost::shared_ptr<Log> log, portion.getLogs()) {
        logs.append(log->getFormatedOutput());
    }

    bool ret = true;
    if (portion.getClient() != m_client) {
        LOG_ENTRY(MyLogger::DEBUG,
                "Trying to write logs from: "<<portion.getClient() <<"to channel which belongs to "<<m_client);
        ret = false;
    }

    if (portion.getProvider() != m_provider) {
        LOG_ENTRY(MyLogger::DEBUG,
                "Trying to write logs received via: "<<portion.getProvider() <<"to channel which starts in "<<m_provider);
        ret = false;
    }

    if (ret && (ret = m_pendingWrites.empty())) {
        ret = m_file->write(m_client, m_provider, m_consumers, m_header, logs);
    }

    return ret;
}

int DataChannel::scheduleForWrite(const DataPortion& portion)
{
    int ret = 0;
    QList<QString> logs;
    foreach(boost::shared_ptr<Log> log, portion.getLogs()) {
        logs.append(log->getFormatedOutput());
    }

    if (portion.getClient().simplified() != m_client.simplified()) {
        LOG_ENTRY(MyLogger::DEBUG,
                "Trying to write logs from: "<<portion.getClient() <<" to channel which belongs to "<<m_client);
        ret = -1;
    }

    if (portion.getProvider().simplified() != m_provider.simplified()) {
        LOG_ENTRY(MyLogger::DEBUG,
                "Trying to write logs received via: "<<portion.getProvider() <<" to channel which starts in "<<m_provider);
        ret = -1;
    }

    if (ret == 0) {
        qint64 id = m_file->scheduleForWrite(m_client, m_provider, m_consumers,
                m_header, logs);

        if (id > 0) {
            m_pendingWrites.enqueue(id);
        }

        ret = id;
    }

    return ret;
}

qint64 DataChannel::getSize(const DataPortion& portion) const
{
    return m_header.size() + portion.getSize();
}

qint64 DataChannel::getBufferSize() const
{
    return m_file->getMaxFileSize();
}

qint64 DataChannel::getFreeSpaceSize() const
{
    return m_file->getFreeSpaceSize();
}

} //namespace Base
} //namespace INZ_project
