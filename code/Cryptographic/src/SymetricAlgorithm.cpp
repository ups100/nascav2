/**
 * @file  SymetricAlgorithm.cpp
 * @brief  Implementation of the Class INZ_project::Cryptographic::SymetricAlgorithm
 * @date  Created on:      13-sie-2013 17:38:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "SymetricAlgorithm.h"

namespace INZ_project {
namespace Cryptographic {

SymetricAlgorithm::SymetricAlgorithm()
{

}

SymetricAlgorithm::~SymetricAlgorithm()
{

}

const QByteArray& SymetricAlgorithm::getKey()
{

    return m_key;
}

void SymetricAlgorithm::setKey(const QByteArray& key)
{
    m_key = key;
}

} //namespace Cryptographic
} //namespace INZ_project
