/**
 * @file  RSASSA_PKCSv15_SHA256.h
 * @brief  Definition of the Class INZ_project::Cryptographic::RSASSA_PKCSv15_SHA256
 * @date  19-10-2013 21:43:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "RSASSA_PKCSv15_SHA256.h"
#include <crypto++/filters.h>
#include <crypto++/osrng.h>
#include <crypto++/cryptlib.h>
#include <QList>

using namespace CryptoPP;

namespace INZ_project {
namespace Cryptographic {
RSASSA_PKCSv15_SHA256::RSASSA_PKCSv15_SHA256()
{
}

RSASSA_PKCSv15_SHA256::~RSASSA_PKCSv15_SHA256()
{

}

RSASSA_PKCSv15_SHA256::RSASSA_PKCSv15_SHA256(const QByteArray& publicKey,
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

        m_verifier = RSASS<PKCS1v15, SHA256>::Verifier(m_rawPublicKey);
        m_signer = RSASS<PKCS1v15, SHA256>::Signer(m_rawPrivateKey);
    } catch (const CryptoPP::Exception& e) {
        m_privateKey.clear();
        m_publicKey.clear();
        throw SignAlgorithmException(e.what());
    }
}

RSASSA_PKCSv15_SHA256::RSASSA_PKCSv15_SHA256(const QByteArray& key, bool isPublic)
{

    try {
        if (isPublic) {
            m_publicKey = key;
            StringSource pub(reinterpret_cast<const byte*>(m_publicKey.data()),
                    static_cast<unsigned>(m_publicKey.length()), true);

            m_rawPublicKey.Load(pub);
            m_verifier = RSASS<PKCS1v15, SHA256>::Verifier(m_rawPublicKey);
        } else {
            m_privateKey = key;
            StringSource priv(
                    reinterpret_cast<const byte*>(m_privateKey.data()),
                    static_cast<unsigned>(m_privateKey.length()), true);

            m_rawPrivateKey.Load(priv);
            m_signer = RSASS<PKCS1v15, SHA256>::Signer(m_rawPrivateKey);
            ;
        }
    } catch (const CryptoPP::Exception& e) {
        m_publicKey.clear();
        m_privateKey.clear();
        throw SignAlgorithmException(e.what());
    }
}

SignAlgorithm* RSASSA_PKCSv15_SHA256::clone()
{
    SignAlgorithm *other = 0L;
    if (m_publicKey.isEmpty() && m_privateKey.isEmpty()) {
        other = new RSASSA_PKCSv15_SHA256();
    } else if (!m_privateKey.isEmpty() && !m_publicKey.isEmpty()) {
        other = new RSASSA_PKCSv15_SHA256(m_publicKey, m_privateKey);
    } else {
        other = m_privateKey.isEmpty() ?
                new RSASSA_PKCSv15_SHA256(m_publicKey, true) :
                new RSASSA_PKCSv15_SHA256(m_privateKey, false);
    }

    return other;
}

QByteArray RSASSA_PKCSv15_SHA256::sign(const QByteArray& message)
{
    if (m_privateKey.isEmpty()) {
        throw SignAlgorithmException("Private key has not been set");
    }
    std::string signature;

    try {
        AutoSeededRandomPool rng;

        StringSource(reinterpret_cast<const byte*>(message.data()),
                message.length(), true,
                new SignerFilter(rng, m_signer, new StringSink(signature)));

    } catch (const CryptoPP::Exception& e) {
        throw SignAlgorithmException(e.what());
    }

    return QByteArray(signature.c_str(), signature.length());
}

bool RSASSA_PKCSv15_SHA256::verify(const QByteArray& message,
        const QByteArray& sign)
{
    if (m_publicKey.isEmpty()) {
        throw SignAlgorithmException("Public key has not been set");
    }
    bool result;
    QByteArray content = message + sign;
    try {
        AutoSeededRandomPool rng;

        StringSource(reinterpret_cast<const byte*>(content.data()),
                content.length(), true,
                new SignatureVerificationFilter(m_verifier,
                        new ArraySink((byte*) &result, sizeof(result)),
                SignatureVerificationFilter::PUT_RESULT
                        | SignatureVerificationFilter::SIGNATURE_AT_END));

    } catch (const CryptoPP::Exception& e) {
        throw SignAlgorithmException(e.what());
    }

    return result;
}

void RSASSA_PKCSv15_SHA256::setPublicKey(const QByteArray& partnerKey)
{
    if (partnerKey == QByteArray()) {
        throw SignAlgorithmException("Key cannot be empty");
    }

    m_publicKey = partnerKey;

    try {
        StringSource pub(reinterpret_cast<const byte*>(m_publicKey.data()),
                static_cast<unsigned>(m_publicKey.length()), true);

        m_rawPublicKey.Load(pub);
        m_verifier = RSASS<PKCS1v15, SHA256>::Verifier(m_rawPublicKey);
    } catch (const CryptoPP::Exception& e) {
        m_publicKey.clear();
        throw SignAlgorithmException(e.what());
    }

    return;
}

void RSASSA_PKCSv15_SHA256::setPrivateKey(const QByteArray& partnerKey)
{
    if (partnerKey == QByteArray()) {
        throw SignAlgorithmException("Key cannot be empty");
    }

    m_privateKey = partnerKey;

    try {
        StringSource priv(reinterpret_cast<const byte*>(m_privateKey.data()),
                static_cast<unsigned>(m_privateKey.length()), true);

        m_rawPrivateKey.Load(priv);
        m_signer = RSASS<PKCS1v15, SHA256>::Signer(m_rawPrivateKey);
    } catch (const CryptoPP::Exception& e) {
        m_privateKey.clear();
        throw SignAlgorithmException(e.what());
    }

    return;
}

} //namespace Cryptographic
} //namespace INZ_project
