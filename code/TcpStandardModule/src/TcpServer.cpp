/**
 * @file  TcpServer.cpp
 * @brief  Implementation of the Class INZ_project::TcpStandardModule::TcpServer
 * @date  17-11-2013 11:09:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "TcpServer.h"
#include <QTcpSocket>

namespace INZ_project {
namespace TcpStandardModule {

TcpServer::TcpServer()
{

}

TcpServer::~TcpServer()
{
    while(hasPendingConnections()) {
        QTcpSocket *socket = nextPendingConnection();
        socket->close();
        delete socket;
    }
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket();
    socket->setSocketDescriptor(socketDescriptor);
    addPendingConnection(socket);
}

} //namespace TcpStandardModule
} //namespace INZ_project
