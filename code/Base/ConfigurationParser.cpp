/**
* @file  ConfigurationParser.h
* @brief  Implementation of the Class ConfigurationParser
* @date  Created on:      13-sie-2013 17:38:10
* @author Krysztof Opasiak <ups100@tlen.pl>
*/

#include "ConfigurationParser.h"

using INZ_project::Base::ConfigurationParser;


ConfigurationParser::ConfigurationParser(){

}



ConfigurationParser::~ConfigurationParser(){

}





bool ConfigurationParser::checkConfFile(){

	return false;
}


QList<QString>* ConfigurationParser::getDataConsumers(){

	return  NULL;
}


QList<QString>* ConfigurationParser::getDataProviders(){

	return  NULL;
}


void ConfigurationParser::getRoutes(const QString& client, const QString& DataProvider){

}