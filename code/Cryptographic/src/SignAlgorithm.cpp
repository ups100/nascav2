/**
 * @file  SignAlgorithm.cpp
 * @brief  Definition of the Class INZ_project::Cryptographic::SignAlgorithm
 * @date  21-11-2013 17:38:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "SignAlgorithm.h"

namespace INZ_project {
namespace Cryptographic {

SignAlgorithm::SignAlgorithm()
{

}

SignAlgorithm::~SignAlgorithm()
{

}

const QByteArray& SignAlgorithm::getPrivateKey()
{

    return m_privateKey;
}

const QByteArray& SignAlgorithm::getPublicKey()
{

    return m_publicKey;
}

void SignAlgorithm::setPublicKey(const QByteArray& partnerKey)
{
    m_publicKey = partnerKey;
}

void SignAlgorithm::setPrivateKey(const QByteArray& partnerKey)
{
    m_privateKey = partnerKey;
}

} //namespace Cryptographic
} //namespace INZ_project
