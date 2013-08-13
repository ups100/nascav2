/**
* @file  DataProviderFactory.h
* @brief  Implementation of the Class DataProviderFactory
* @date  Created on:      13-sie-2013 17:38:11
* @author Krysztof Opasiak <ups100@tlen.pl>
*/

#include "DataProviderFactory.h"

using INZ_project::Base::DataProviderFactory;


DataProviderFactory::DataProviderFactory(){

}



DataProviderFactory::~DataProviderFactory(){

}





DataProvider * DataProviderFactory::getDataProvider(const QString& id){

	return  NULL;
}


DataProviderFactory* DataProviderFactory::getInstance(){

	return  NULL;
}


void DataProviderFactory::registerDataProvider(DataProvider* dataProvider, QString id){

}