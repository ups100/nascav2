/**
 * @file  TcpServer.h
 * @brief  Definition of the Class INZ_project::TcpStandardModule::TcpServer
 * @date  17-11-2013 10:56:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include <QTcpServer>

namespace INZ_project {
namespace TcpStandardModule {

/**
 * @brief Server class to listen for incoming connections.
 */
class TcpServer : public QTcpServer
{
public:
    /**
     * @brief Constructor
     */
    TcpServer();

    /**
     * @brief Destructor
     */
    virtual ~TcpServer();

protected:
    virtual void incomingConnection(qintptr socketDescriptor);
};

} //namespace TcpStandardModule
} //namespace INZ_project
