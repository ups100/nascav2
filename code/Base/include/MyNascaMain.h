/**
 * @file  MyNascaMain.h
 * @brief  Definition of the Class MyNascaMain
 * @date  13-sie-2013 17:38:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_D5A037F0_18C8_4dda_9955_7BDA9D7E33F3__INCLUDED_)
#define EA_D5A037F0_18C8_4dda_9955_7BDA9D7E33F3__INCLUDED_

#include "DataProvider.h"
#include "DataConsumer.h"
#include "ConfigurationParser.h"

namespace INZ_project {
namespace Base {
class MyNascaMain
{

public:
    MyNascaMain();
    virtual ~MyNascaMain();
    INZ_project::Base::DataProvider *m_DataProvider;
    INZ_project::Base::DataConsumer *m_DataConsumer;
    INZ_project::Base::ConfigurationParser *m_ConfigurationParser;

    static void provideDataChannel(Client* client, DataProvider* provider);

};

}

}
#endif // !defined(EA_D5A037F0_18C8_4dda_9955_7BDA9D7E33F3__INCLUDED_)
