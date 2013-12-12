/**
 * @file  DefaultTcpProvider.cpp
 * @brief Implementation of the Class DefaultTcpProvider
 * @date  28-11-2013 21:21:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "DefaultTcpProvider.h"
#include "LogEntry.h"
#include "TcpServer.h"
#include "SessionPart.h"
#include "ConnectionManager.h"
#include "SessionInitializer.h"
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

using namespace INZ_project::TcpStandardModule;
using boost::shared_ptr;

namespace INZ_project {
namespace Base {

DefaultTcpProvider::DefaultTcpProvider()
        : m_thread(boost::bind(&DefaultTcpProvider::run, this)),
                m_address(QHostAddress::Any), m_port(0)
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
    if (parseOptions(additionalData) != 0) {
        LOG_ENTRY(MyLogger::FATAL, "Unable to parse given options.");
        return -1;
    }
    m_thread.startThread();
    return 0;
}

int DefaultTcpProvider::run()
{
    //prepare server
    TcpServer server;
    if (!server.listen(m_address, m_port)) {
        LOG_ENTRY(MyLogger::FATAL,
                "Unable to listen at "<<m_address.toString()<<":"<<m_port);
        ///////////////////////////
        // TODO add error handling
        //////////////////////////
        return -1;
    } else {
        LOG_ENTRY(MyLogger::INFO,
                "Listening at "<<m_address.toString()<<":"<<server.serverPort())
    }

    //prepare session schema
    QMap<QString, QList<shared_ptr<SessionPart> > > versions;
    QList<shared_ptr<SessionPart> > parts;
    parts.append(boost::shared_ptr<SessionPart>(new SessionInitializer(versions)));

    //prepare connection manager
    ConnectionManager manager(&server, parts);

    //let's roll the ball
    return m_thread.exec();
}

int DefaultTcpProvider::parseOptions(const QString& options)
{
    static const QString ipString = "IP:";
    static const QString portString = "Port:";

    int pos = -1, nlPos = -1;

    //check if there is IP to set
    if ((pos = options.indexOf(ipString)) != -1) {
        nlPos = options.indexOf("\n", pos);
        QString address = options.mid(pos + ipString.size(),
                nlPos - pos - ipString.size());

        if (!m_address.setAddress(address)) {
            LOG_ENTRY(MyLogger::ERROR,
                    "Value: "<<address<<" is not a valid IP address.");
            return -1;
        } else {
            LOG_ENTRY(MyLogger::INFO, "IP address provided: "<<address);
        }
    } else {
        LOG_ENTRY(MyLogger::INFO,
                "IP address has not been provided. Default: "<<m_address.toString()<<" used.");

    }

    //check if there is port to set
    if ((pos = options.indexOf(portString)) != -1) {
        nlPos = options.indexOf("\n", pos);
        QString port = options.mid(pos + portString.size(),
                nlPos - pos - portString.size());

        bool ok = false;
        short portValue = port.toShort(&ok);
        if (ok) {
            m_port = portValue;
            LOG_ENTRY(MyLogger::INFO, "Port number provided: "<<m_port);
        } else {
            LOG_ENTRY(MyLogger::ERROR,
                    "Value: "<<port<<" is not a valid IP port.");
            return -1;
        }
    } else {
        LOG_ENTRY(MyLogger::INFO,
                "Port number has not been provided. Default: "<<m_port<<" used.");

    }

    return 0;
}

} //namespace Base
} //namespace INZ_project
