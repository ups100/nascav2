/**
 * @file  SHA256Algorithm.h
 * @brief  Definition of the Class INZ_project::Cryptographic::SHA256Algorithm
 * @date  21-11-2013 21:43:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "SHA256Algorithm.h"
#include <crypto++/filters.h>
#include <string>

using namespace CryptoPP;

namespace INZ_project {
namespace Cryptographic {

SHA256Algorithm::SHA256Algorithm()
{
}

SHA256Algorithm::~SHA256Algorithm()
{

}

SignAlgorithm* SHA256Algorithm::clone()
{
    return new SHA256Algorithm();
}

QByteArray SHA256Algorithm::generateSign(const QByteArray& message)
{
    std::string hash;
    try {

        StringSource(reinterpret_cast<const byte*>(message.data()),
                message.length(), true,
                new HashFilter(m_hash, new StringSink(hash)));

    } catch (const CryptoPP::Exception& e) {
        throw SignAlgorithmException(e.what());
    }

    return QByteArray(hash.c_str(), hash.length());
}

bool SHA256Algorithm::verifySign(const QByteArray& message,
        const QByteArray& sign)
{
    return generateSign(message) == sign;
}

} //namespace Cryptographic
} //namespace INZ_project
