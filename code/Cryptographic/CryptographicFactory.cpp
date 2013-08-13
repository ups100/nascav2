/**
* @file  CryptographicFactory.h
* @brief  Implementation of the Class CryptographicFactory
* @date  Created on:      13-sie-2013 17:38:10
* @author Krysztof Opasiak <ups100@tlen.pl>
*/

#include "CryptographicFactory.h"

using INZ_project::Cryptographic::CryptographicFactory;


CryptographicFactory::CryptographicFactory(){

}



CryptographicFactory::~CryptographicFactory(){

}





const QList<QString>& CryptographicFactory::getASymAlgorithmList(){

	return  NULL;
}


CryptographicFactory* CryptographicFactory::getInstance(){

	return  NULL;
}


const QList<QString>& CryptographicFactory::getSymAlgorithmList(){

	return  NULL;
}


const QList<QString>& CryptographicFactory::getSymetricAlgorithmList(){

	return  NULL;
}