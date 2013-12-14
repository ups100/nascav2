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
#include "CryptographyInitializer.h"
#include "ClientAuthorizer.h"
#include "LogsTransmitter.h"
#include "CryptographicFactory.h"
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

using namespace INZ_project::TcpStandardModule;
using namespace INZ_project::Cryptographic;
using boost::shared_ptr;

namespace INZ_project {
namespace Base {

DefaultTcpProvider::DefaultTcpProvider()
        : m_thread(boost::bind(&DefaultTcpProvider::run, this)),
                m_address(QHostAddress::Any), m_port(0)
{
    m_algorithm = shared_ptr<AsymetricAlgorithm>(
            CryptographicFactory::getAsymAlgorithm("RSA_OAEP_SHA"));
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
    QList<shared_ptr<SessionPart> > version1point0;

    //set of allowed symmetric algorithms
    QSet<QString> allowedAlgorithms;
    allowedAlgorithms.insert("AES_CBC");

    HashAlgorithm *hash = CryptographicFactory::getHashAlgorithm("SHA_256");

    //append cryptographic initialization
    version1point0.append(
            shared_ptr<SessionPart>(
                    new CryptographyInitializer(allowedAlgorithms, m_algorithm.get(),
                            this, hash)));

    //append client authorization
    version1point0.append(shared_ptr<SessionPart>(new ClientAuthorizer()));

    //append logs transission
    version1point0.append(shared_ptr<SessionPart>(new LogsTransmitter(this)));

    versions["v1.0"] = version1point0;

    QList<shared_ptr<SessionPart> > parts;
    parts.append(
            boost::shared_ptr<SessionPart>(new SessionInitializer(versions)));

    //prepare connection manager
    ConnectionManager manager(&server, parts);

    //let's roll the ball
    return m_thread.exec();
}

int DefaultTcpProvider::parseOptions(const QString& options)
{
    static const QString ipString = "IP:";
    static const QString portString = "Port:";
    static const QString keyFileString = "PrivateKey:";

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

    //check if private key file has been set
    if ((pos = options.indexOf(keyFileString)) != -1) {
        nlPos = options.indexOf("\n", pos);

        QString fileName = options.mid(pos + keyFileString.size(),
                nlPos - pos - keyFileString.size());

        QFile file(fileName.trimmed());
        if (file.exists() && file.open(QIODevice::ReadOnly)) {
            QByteArray key = file.readAll();

            try {
                m_algorithm->setPrivateKey(key);
                LOG_ENTRY(MyLogger::INFO,
                                    "Content of "<<fileName.trimmed()<<" used as key file.");
            } catch (AsymetricAlgorithm::AsymmetricAlgorithmException& e) {
                LOG_ENTRY(MyLogger::ERROR,
                        "Unable to use content of file: "<<fileName<<" as key: "<<e.what());
            }
        } else {
            LOG_ENTRY(MyLogger::ERROR,
                    "Unable to use file: "<<fileName<<" as key file");
            return -1;
        }

    } else {
        LOG_ENTRY(MyLogger::INFO,
                "File with private key has not been provided. Generating new pair private, public keys.");

        QFile privateKey(this->getProviderId() + ".priv");
        QFile publicKey(this->getProviderId() + ".pub");

        if (privateKey.exists() || publicKey.exists()
                || !privateKey.open(QIODevice::WriteOnly)
                || !publicKey.open(QIODevice::WriteOnly)) {
            LOG_ENTRY(MyLogger::ERROR,
                    "Unable to generate new public and private key pair.");
            return -1;
        }

        if (generateKeysToFiles(&privateKey, &publicKey) != 0) {
            return -1;
        }

    }
    return 0;
}

int DefaultTcpProvider::generateKeysToFiles(QFile *privateKey, QFile *publicKey)
{
    try {
        m_algorithm->generateKeys(-1);
        privateKey->write(m_algorithm->getPrivateKey());
        publicKey->write(m_algorithm->getPublicKey());
        privateKey->close();
        publicKey->close();

        LOG_ENTRY(MyLogger::INFO,
                "Generated key pair: "<<privateKey->fileName()<<" "<< publicKey->fileName());
    } catch (AsymetricAlgorithm::AsymmetricAlgorithmException& e) {
        privateKey->close();
        publicKey->close();
        privateKey->remove();
        publicKey->remove();

        LOG_ENTRY(MyLogger::ERROR,
                "Unable to generate new public and private key pare: "<<e.what());

        return -1;
    }
    return 0;
}

} //namespace Base
} //namespace INZ_project
