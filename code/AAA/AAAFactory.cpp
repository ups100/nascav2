/**
* @file  AAAFactory.h
* @brief  Implementation of the Class AAAFactory
* @date  Created on:      13-sie-2013 17:38:09
* @author Krysztof Opasiak <ups100@tlen.pl>
*/

#include "AAAFactory.h"

using INZ_project::AAA::AAAFactory;


AAAFactory::AAAFactory(){

}



AAAFactory::~AAAFactory(){

}





AAAModule* AAAFactory::getAAAModule(const QString& id){

	return  NULL;
}


QList<QString> AAAFactory::getAAAModulesList(){

	return  NULL;
}


AAAFactory* AAAFactory::getInstance(){

	return  NULL;
}


void AAAFactory::registerAAAModule(AAAModule* module, QString id){

}