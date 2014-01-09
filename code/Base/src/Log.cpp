/**
 * @file  Log.cpp
 * @brief  Definition of the Class Log
 * @date  09-01-2014 18:55:36
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include <Log.h>

namespace INZ_project {
namespace Base {

Log::Log(time_t time, const QString& hostName, const QString& serviceName,
        int state, const QString output)
        : m_time(time), m_hostName(hostName), m_serviceName(serviceName),
                m_state(state), m_pluginOutput(output)
{
    generateFormatedOutput(time, hostName, serviceName, state, output);
}

Log::~Log()
{

}

time_t Log::getTime() const
{
    return m_time;
}

const QString& Log::getHostName() const
{
    return m_hostName;
}

const QString& Log::getServiceName() const
{
    return m_serviceName;
}

int Log::getState() const
{
    return m_state;
}

const QString& Log::getPluginOutput() const
{
    return m_pluginOutput;
}

const QString& Log::getFormatedOutput() const
{
    return m_formated;
}

void Log::generateFormatedOutput(time_t time, const QString& hostName,
        const QString& serviceName, int state, const QString output)
{
    //check for commands injection
    if(hostName.contains(";") || serviceName.contains(";") || output.contains("[")) {
        throw LogWrongFormatException("Not allowed ; or [ sign");
    }

    if (serviceName == QString()) {
        //we have host check
        QString hostCheckFormat("[%1] PROCESS_HOST_CHECK_RESULT;%2;%3;%4\n");
        m_formated = hostCheckFormat.arg(QString().setNum((long) time),
                hostName, QString().setNum(state), output);
    } else {
        //we have service check
        QString serviceCheckFormat(
                "[%1] PROCESS_SERVICE_CHECK_RESULT;%2;%3;%4;%5\n");
        m_formated = serviceCheckFormat.arg(QString().setNum((long) time),
                        hostName, serviceName, QString().setNum(state), output);
    }
}

} //namespace Base
} //namespace INZ_project
