/**
* @file  IcingaSender.h
* @brief  Implementation of the Class IcingaSender
* @date  Created on:      13-sie-2013 17:38:12
* @author Krysztof Opasiak <ups100@tlen.pl>
*/

#include "IcingaSender.h"

using INZ_project::IcingaSenderModule::IcingaSender;


IcingaSender::IcingaSender(){

}



IcingaSender::~IcingaSender(){

}





DataConsumer* IcingaSender::clone(){

	return  NULL;
}


void IcingaSender::close(){

}


void IcingaSender::consumeData(Client* client, DataPortion data){

}


void IcingaSender::init(int argc, char ** argv){

}