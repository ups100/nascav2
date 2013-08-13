/**
 * @file  TcpReceiver.h
 * @brief  Definition of the Class TcpReceiver
 * @date  13-sie-2013 17:38:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_78E1E3AD_7C3A_4ea9_86E8_1701FFCB501F__INCLUDED_)
#define EA_78E1E3AD_7C3A_4ea9_86E8_1701FFCB501F__INCLUDED_

#include "DataProvider.h"
#include "TcpServer.h"
#include "ClientCommunication.h"

namespace INZ_project {
namespace TcpStandardModule {
class TcpReceiver : public INZ_project::Base::DataProvider
{

public:
    TcpReceiver();
    virtual ~TcpReceiver();
    INZ_project::TcpStandardModule::TcpServer *m_TcpServer;
    INZ_project::TcpStandardModule::ClientCommunication *m_ClientCommunication;

    void clone();
    void close();
    void init(int argc, char ** argv);

};

}

}
#endif // !defined(EA_78E1E3AD_7C3A_4ea9_86E8_1701FFCB501F__INCLUDED_)
