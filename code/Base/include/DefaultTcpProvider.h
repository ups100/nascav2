/**
 * @file  DefaultTcpProvider.h
 * @brief  Definition of the Class INZ_project::Base::DefaultTcpProvider
 * @date  26-11-2013 22:38:35
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include <QThread>
#include <QHostAddress>
#include <QFile>
#include <boost/shared_ptr.hpp>
#include "AdditionalThread.h"
#include "DataProviderRegister.h"

namespace INZ_project {
namespace Cryptographic {
class AsymetricAlgorithm;
class SignAlgorithm;
class HashAlgorithm;
}

namespace TcpStandardModule {
class ConnectionManager;
}

namespace Base {

/**
 * @brief Base data provider which use TCP.
 * @details This class starts the new thread.
 */
DATA_PROVIDER(DefaultTcpProvider, DefaultTcpProvider)
{
public:
    /**
     * @brief Constructor
     */
    DefaultTcpProvider();

    /**
     * @brief Destructor
     */
    virtual ~DefaultTcpProvider();

    virtual void close();

protected:
    virtual int initImpl(const QString& additionalData,
            const QString& providerId, const QString& providerType);

private:

    /**
     * @brief Parses the options and search for values like ip or port
     * @details The format should be IP: IP_ADDR \n and Port: PORT \n
     * @param[in] options string from which options should be parsed
     */
    int parseOptions(const QString& options);

    int generateKeysToFiles(QFile *privateKey, QFile *publicKey);

    /**
     * @brief Main function of the additional thread
     */
    int run();

    /**
     * @brief Additional thread for logic of this class
     */
    TcpStandardModule::AdditionalThread m_thread;

    /**
     * @brief Address which should be used to listen for incoming connections
     */
    QHostAddress m_address;

    /**
     * @brief Address which should be used to listen for incoming connections
     */
    qint16 m_port;

    boost::shared_ptr<Cryptographic::AsymetricAlgorithm> m_algorithm;

    boost::shared_ptr<Cryptographic::SignAlgorithm> m_signer;

    TcpStandardModule::ConnectionManager *m_connectionManager;
};

} //namespace Base
} //namespace INZ_project
