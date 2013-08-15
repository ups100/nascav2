/**
 * @file  ClientCommunicationFactory.h
 * @brief  Definition of the Class ClientCommunicationFactory
 * @date  13-sie-2013 17:38:10
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_F72028D3_8BE8_4f0f_823B_D1BBE5AA96A8__INCLUDED_)
#define EA_F72028D3_8BE8_4f0f_823B_D1BBE5AA96A8__INCLUDED_

#include <QList>
#include <QString>

namespace INZ_project {
namespace TcpStandardModule {

class ClientCommunication;

class ClientCommunicationFactory
{

public:
    ClientCommunicationFactory();
    virtual ~ClientCommunicationFactory();
    INZ_project::TcpStandardModule::ClientCommunication *m_ClientCommunication;

    static ClientCommunication* getClientCommunication(const QString& version);
    static ClientCommunicationFactory* getInstance();
    static QList<QString> getProtocolVersions();

};

} //namespace TcpStandardModule
} //namespace INZ_project
#endif // !defined(EA_F72028D3_8BE8_4f0f_823B_D1BBE5AA96A8__INCLUDED_)
