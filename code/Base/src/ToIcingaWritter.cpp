/**
 * @file  ToIcingaWritter.cpp
 * @brief Implementation of the Class ToIcingaWritter
 * @date  20-12-2013 21:21:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "ToIcingaWritter.h"
#include "LogEntry.h"
#include <boost/bind.hpp>
#include <QObject>
#include <QMetaObject>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "ReadPortion.h"

namespace INZ_project {
namespace Base {

ToIcingaWritter::ToIcingaWritter()
        : m_thread(boost::bind(&ToIcingaWritter::run, this)),
                m_pipeFile("/usr/local/icinga/var/rw/icinga.cmd")
{

}

ToIcingaWritter::~ToIcingaWritter()
{

}

void ToIcingaWritter::close()
{
    m_thread.endThread();
}

void ToIcingaWritter::consumeDataPortion(const ReadPortion *portion,
        QObject* toConfirm, QString confirmMethod)
{
    LOG_ENTRY(MyLogger::INFO,
            " Writing portion of "<<portion->getLogs().size()<<" logs.");
    foreach(QString log, portion->getLogs()){
    std::string logStd = log.toStdString();
    const char *buf = logStd.c_str();

    for(int toWrite = log.size(); toWrite > 0; ) {
        int written = write(m_pipeDesc, buf + log.size() - toWrite, toWrite);
        if(written < 0) {
            LOG_ENTRY(MyLogger::FATAL, "Unable to write to "<<m_pipeFile);
            //TODO add error handling
            return;
        } else {
            toWrite -= written;
        }
    }
}

    QMetaObject::invokeMethod(toConfirm, confirmMethod.toStdString().c_str(),
            Qt::QueuedConnection, Q_ARG(const ReadPortion*, portion));
}

int ToIcingaWritter::initImpl(const QString& additionalData,
        const QString& consumerId, const QString& consumerType)
{

    if (parseOptions(additionalData) != 0) {
        LOG_ENTRY(MyLogger::FATAL, "Unable to parse given options.");
        return -1;
    }

    m_thread.startThread();
    this->moveToThread(&m_thread);
    return 0;
}

int ToIcingaWritter::parseOptions(const QString& options)
{
    static const QString pipeString = "PIPE:";
    int pos, nlPos;

    if ((pos = options.indexOf(pipeString)) != -1) {
        nlPos = options.indexOf("\n", pos);
        QString path = options.mid(pos + pipeString.size(),
                nlPos - pos - pipeString.size());
        m_pipeFile = path;
        LOG_ENTRY(MyLogger::INFO, "Command file path provided: "<<m_pipeFile);
    } else {
        LOG_ENTRY(MyLogger::INFO,
                "Command file path has not been provided. Default: "<<m_pipeFile<<" used.");
    }

    return 0;
}

int ToIcingaWritter::run()
{
    m_pipeDesc = open(m_pipeFile.toStdString().c_str(), O_WRONLY);
    if (m_pipeDesc < 0) {
        LOG_ENTRY(MyLogger::FATAL, "Unable to open file "<<m_pipeFile);
        //TODO error handling
    }

    LOG_ENTRY(MyLogger::INFO, "ToIcingaWritter Started");
    return m_thread.exec();
}

} //namespace Base
} //namespace INZ_project
