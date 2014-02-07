/**
 * @file  RSAAlgorithmOAEPUsingSHA.cpp
 * @brief  Definition of the Class INZ_project::Cryptographic::RSAAlgorithmOAEPUsingSHA
 * @date  19-10-2013 21:43:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "RSAAlgorithmOAEPUsingSHA.h"
#include <crypto++/filters.h>
#include <crypto++/osrng.h>
#include <crypto++/cryptlib.h>
#include <QList>

using namespace CryptoPP;

namespace INZ_project {
namespace Cryptographic {
RSAAlgorithmOAEPUsingSHA::RSAAlgorithmOAEPUsingSHA()
{
}

RSAAlgorithmOAEPUsingSHA::~RSAAlgorithmOAEPUsingSHA()
{

}

RSAAlgorithmOAEPUsingSHA::RSAAlgorithmOAEPUsingSHA(const QByteArray& publicKey,
        const QByteArray& privateKey)
{
    m_privateKey = privateKey;
    m_publicKey = publicKey;

    try {
        StringSource pub(reinterpret_cast<const byte*>(m_publicKey.data()),
                static_cast<unsigned>(m_publicKey.length()), true);

        m_rawPublicKey.Load(pub);

        StringSource priv(reinterpret_cast<const byte*>(m_privateKey.data()),
                static_cast<unsigned>(m_privateKey.length()), true);

        m_rawPrivateKey.Load(priv);

        m_encryptor = RSAES_OAEP_SHA_Encryptor(m_rawPublicKey);
        m_decryptor = RSAES_OAEP_SHA_Decryptor(m_rawPrivateKey);
    } catch (const CryptoPP::Exception& e) {
        m_privateKey.clear();
        m_publicKey.clear();
        throw AsymmetricAlgorithmException(e.what());
    }
}

RSAAlgorithmOAEPUsingSHA::RSAAlgorithmOAEPUsingSHA(const QByteArray& key,
        bool isPublic)
{

    try {
        if (isPublic) {
            m_publicKey = key;
            StringSource pub(reinterpret_cast<const byte*>(m_publicKey.data()),
                    static_cast<unsigned>(m_publicKey.length()), true);

            m_rawPublicKey.Load(pub);
            m_encryptor = RSAES_OAEP_SHA_Encryptor(m_rawPublicKey);
        } else {
            m_privateKey = key;
            StringSource priv(
                    reinterpret_cast<const byte*>(m_privateKey.data()),
                    static_cast<unsigned>(m_privateKey.length()), true);

            m_rawPrivateKey.Load(priv);
            m_decryptor = RSAES_OAEP_SHA_Decryptor(m_rawPrivateKey);
        }
    } catch (const CryptoPP::Exception& e) {
        m_publicKey.clear();
        m_privateKey.clear();
        throw AsymmetricAlgorithmException(e.what());
    }
}

AsymetricAlgorithm* RSAAlgorithmOAEPUsingSHA::clone()
{
    AsymetricAlgorithm *other = 0L;
    if (m_publicKey.isEmpty() && m_privateKey.isEmpty()) {
        other = new RSAAlgorithmOAEPUsingSHA();
    } else if (!m_privateKey.isEmpty() && !m_publicKey.isEmpty()) {
        other = new RSAAlgorithmOAEPUsingSHA(m_publicKey, m_privateKey);
    } else {
        other = m_privateKey.isEmpty() ?
                new RSAAlgorithmOAEPUsingSHA(m_publicKey, true) :
                new RSAAlgorithmOAEPUsingSHA(m_privateKey, false);
    }

    return other;
}

QByteArray RSAAlgorithmOAEPUsingSHA::decrypt(const QByteArray& encrypted)
{
    if (m_privateKey.isEmpty()) {
        throw AsymmetricAlgorithmException(
                "Private key has not been generated or set");
    }
    std::string plain;
    try {
        AutoSeededRandomPool rng;

        StringSource(reinterpret_cast<const byte*>(encrypted.data()),
                encrypted.length(), true,
                new PK_DecryptorFilter(rng, m_decryptor,
                        new StringSink(plain)));

    } catch (const CryptoPP::Exception& e) {
        throw AsymmetricAlgorithmException(e.what());
    }

    return QByteArray(plain.c_str(), plain.length());

}

QByteArray RSAAlgorithmOAEPUsingSHA::encrypt(const QByteArray& plain)
{
    if (m_publicKey.isEmpty()) {
        throw AsymmetricAlgorithmException(
                "Public key has not been generated or set");
    }
    std::string encrypted;

    try {
        AutoSeededRandomPool rng;

        StringSource(reinterpret_cast<const byte*>(plain.data()),
                plain.length(), true,
                new PK_EncryptorFilter(rng, m_encryptor,
                        new StringSink(encrypted)));

    } catch (const CryptoPP::Exception& e) {
        throw AsymmetricAlgorithmException(e.what());
    }

    return QByteArray(encrypted.c_str(), encrypted.length());
}

const QByteArray& RSAAlgorithmOAEPUsingSHA::generateKeys(int keySize)
{
    if (keySize < 0) {
        //default key length
        keySize = 1024;
    } else if (keySize == 0) {
        throw AsymmetricAlgorithmException("Key too short");
    } else {
        keySize *= 8;
    }

    try {
        AutoSeededRandomPool rng;

        m_rawPrivateKey.GenerateRandomWithKeySize(rng, keySize);

        m_rawPublicKey = RSA::PublicKey(m_rawPrivateKey);

        m_encryptor = RSAES_OAEP_SHA_Encryptor(m_rawPublicKey);
        m_decryptor = RSAES_OAEP_SHA_Decryptor(m_rawPrivateKey);

        std::string publicKey, privateKey;
        StringSink priv(privateKey), pub(publicKey);

        m_rawPrivateKey.Save(priv);
        m_rawPublicKey.Save(pub);

        m_publicKey = QByteArray(publicKey.c_str(), publicKey.length());
        m_privateKey = QByteArray(privateKey.c_str(), privateKey.length());

    } catch (const CryptoPP::Exception& e) {
        m_publicKey.clear();
        m_privateKey.clear();
        throw AsymmetricAlgorithmException(e.what());
    }

    return m_publicKey;
}

void RSAAlgorithmOAEPUsingSHA::setPublicKey(const QByteArray& partnerKey)
{
    if (partnerKey == QByteArray()) {
        throw AsymmetricAlgorithmException("Key cannot be empty");
    }

    m_publicKey = partnerKey;

    try {
        StringSource pub(reinterpret_cast<const byte*>(m_publicKey.data()),
                static_cast<unsigned>(m_publicKey.length()), true);

        m_rawPublicKey.Load(pub);
        m_encryptor = RSAES_OAEP_SHA_Encryptor(m_rawPublicKey);
    } catch (const CryptoPP::Exception& e) {
        m_publicKey.clear();
        throw AsymmetricAlgorithmException(e.what());
    }

    return;
}

void RSAAlgorithmOAEPUsingSHA::setPrivateKey(const QByteArray& partnerKey)
{
    if (partnerKey == QByteArray()) {
        throw AsymmetricAlgorithmException("Key cannot be empty");
    }

    m_privateKey = partnerKey;

    try {
        StringSource priv(reinterpret_cast<const byte*>(m_privateKey.data()),
                static_cast<unsigned>(m_privateKey.length()), true);

        m_rawPrivateKey.Load(priv);
        m_decryptor = RSAES_OAEP_SHA_Decryptor(m_rawPrivateKey);
    } catch (const CryptoPP::Exception& e) {
        m_privateKey.clear();
        throw AsymmetricAlgorithmException(e.what());
    }

    return;
}

} //namespace Cryptographic
} //namespace INZ_project
