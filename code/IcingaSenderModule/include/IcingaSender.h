/**
 * @file  IcingaSender.h
 * @brief  Definition of the Class IcingaSender
 * @date  13-sie-2013 17:38:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_C8DC8C46_D858_4433_A65D_04FBAEDD061D__INCLUDED_)
#define EA_C8DC8C46_D858_4433_A65D_04FBAEDD061D__INCLUDED_

#include "DataConsumer.h"

namespace INZ_project {
namespace IcingaSenderModule {
class IcingaSender : public INZ_project::Base::DataConsumer
{

public:
    IcingaSender();
    virtual ~IcingaSender();

    virtual DataConsumer* clone();
    virtual void close();
    virtual void consumeData(Client* client, DataPortion data);
    virtual void init(int argc, char ** argv);

};

}

}
#endif // !defined(EA_C8DC8C46_D858_4433_A65D_04FBAEDD061D__INCLUDED_)
