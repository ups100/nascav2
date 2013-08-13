/**
 * @file  DataConsumer.h
 * @brief  Definition of the Class DataConsumer
 * @date  13-sie-2013 17:38:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_388BCAB5_7025_4448_8149_748AA8F8721B__INCLUDED_)
#define EA_388BCAB5_7025_4448_8149_748AA8F8721B__INCLUDED_

namespace INZ_project {
namespace Base {
class DataConsumer
{

public:
    DataConsumer();
    virtual ~DataConsumer();

    virtual DataConsumer* clone() =0;
    virtual void close() =0;
    virtual void consumeData(Client* client, DataPortion data) =0;
    virtual void init(int argc, char ** argv) =0;

};

}

}
#endif // !defined(EA_388BCAB5_7025_4448_8149_748AA8F8721B__INCLUDED_)
