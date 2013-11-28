/**
 * @file  DefaultTcpProvider.cpp
 * @brief Implementation of the Class DefaultTcpProvider
 * @date  28-11-2013 21:21:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "DefaultTcpProvider.h"
#include "LogEntry.h"
#include <boost/bind.hpp>

namespace INZ_project {
namespace Base {

DefaultTcpProvider::DefaultTcpProvider()
        : m_thread(boost::bind(&DefaultTcpProvider::run, this))
{

}

DefaultTcpProvider::~DefaultTcpProvider()
{

}

void DefaultTcpProvider::close()
{
    m_thread.endThread();
}

int DefaultTcpProvider::initImpl(const QString& additionalData,
        const QString& providerId, const QString& providerType)
{
    m_thread.startThread();
    return 0;
}

int DefaultTcpProvider::run()
{
    LOG_ENTRY(MyLogger::INFO, "Zajebiaszczo");
    return m_thread.exec();
}

} //namespace Base
} //namespace INZ_project
