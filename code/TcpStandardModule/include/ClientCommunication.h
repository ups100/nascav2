/**
 * @file  ClientCommunication.h
 * @brief  Definition of the Class ClientCommunication
 * @date  13-sie-2013 17:38:10
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_F3F6B536_D461_46af_B80E_686AFF726757__INCLUDED_)
#define EA_F3F6B536_D461_46af_B80E_686AFF726757__INCLUDED_

#include "ClientConnection.h"

namespace INZ_project {
namespace TcpStandardModule {

class ClientCommunication
{
public:
    ClientCommunication();
    virtual ~ClientCommunication();
    INZ_project::TcpStandardModule::ClientConnection *m_ClientConnection;

    ClientCommunication* clone();

};

} //namespace TcpStandardModule
} //namespace INZ_project
#endif // !defined(EA_F3F6B536_D461_46af_B80E_686AFF726757__INCLUDED_)
