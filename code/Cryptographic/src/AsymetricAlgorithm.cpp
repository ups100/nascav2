/**
 * @file  AsymetricAlgorithm.cpp
 * @brief  Implementation of the Class INZ_project::Cryptographic::AsymetricAlgorithm
 * @date  Created on:      13-sie-2013 17:38:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "AsymetricAlgorithm.h"

namespace INZ_project {
namespace Cryptographic {

AsymetricAlgorithm::AsymetricAlgorithm()
{

}

AsymetricAlgorithm::~AsymetricAlgorithm()
{

}

const QByteArray& AsymetricAlgorithm::getPrivateKey()
{

    return m_privateKey;
}

const QByteArray& AsymetricAlgorithm::getPublicKey()
{

    return m_publicKey;
}

void AsymetricAlgorithm::setPublicKey(const QByteArray& partnerKey)
{
    m_publicKey = partnerKey;
}

void AsymetricAlgorithm::setPrivateKey(const QByteArray& partnerKey)
{
    m_privateKey = partnerKey;
}

} //namespace Cryptographic
} //namespace INZ_project
