/**
 * @file  Client.h
 * @brief  Definition of the Class Client
 * @date  13-sie-2013 17:38:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_D6440D50_CCA5_46f1_8A41_6416AF2C70A2__INCLUDED_)
#define EA_D6440D50_CCA5_46f1_8A41_6416AF2C70A2__INCLUDED_

#include <QString>

namespace INZ_project {
namespace Base {

class DataChannel;

class Client
{

public:
    Client();
    virtual ~Client();

    const QString& getClientId();
    const DataChannel& getDataChannel();

private:
    DataChannel *m_DataChannel;
};

} //namespace Base
} //namespace INZ_project
#endif // !defined(EA_D6440D50_CCA5_46f1_8A41_6416AF2C70A2__INCLUDED_)
