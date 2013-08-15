/**
 * @file  IcingaSender.h
 * @brief  Implementation of the Class IcingaSender
 * @date  Created on:      13-sie-2013 17:38:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "IcingaSender.h"

namespace INZ_project {
namespace IcingaSenderModule {

IcingaSender::IcingaSender()
{

}

IcingaSender::~IcingaSender()
{

}

INZ_project::Base::DataConsumer* IcingaSender::clone()
{

    return NULL;
}

void IcingaSender::close()
{

}

void consumeData(INZ_project::Base::Client* client,
        INZ_project::Base::DataPortion data)
{

}

void IcingaSender::init(int argc, char ** argv)
{

}

} //namespace Base
} //namespace INZ_project
