/**
 * @file  AESAlgorithmCBCMode.h
 * @brief  Definition of the Class INZ_project::Cryptographic::AESAlgorithmCBCMode
 * @date  19-10-2013 21:43:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "AESAlgorithmCBCMode.h"
#include <crypto++/filters.h>
#include <crypto++/osrng.h>
#include <crypto++/cryptlib.h>
#include <QList>

using namespace CryptoPP;

namespace INZ_project {
namespace Cryptographic {

AESAlgorithmCBCMode::AESAlgorithmCBCMode()
{
    //http://www.cryptopp.com/wiki/Aes
    m_allowedKeySizes.append(16);
    m_allowedKeySizes.append(24);
    m_allowedKeySizes.append(32);
    m_allowedBlockSizes.append(16);
}

AESAlgorithmCBCMode::~AESAlgorithmCBCMode()
{

}

AESAlgorithmCBCMode::AESAlgorithmCBCMode(const QByteArray& key,
        const QByteArray& iv)
        : m_iv(iv)
{
    m_key = key;
    //http://www.cryptopp.com/wiki/Aes
    m_allowedKeySizes.append(16);
    m_allowedKeySizes.append(24);
    m_allowedKeySizes.append(32);
    m_allowedBlockSizes.append(16);

    try {
        m_aesEncryption.SetKeyWithIV(
                reinterpret_cast<const byte *>(m_key.data()),
                static_cast<unsigned>(m_key.length()),
                reinterpret_cast<const byte *>(m_iv.data()));

        m_aesDecryption.SetKeyWithIV(
                reinterpret_cast<const byte *>(m_key.data()),
                static_cast<unsigned>(m_key.length()),
                reinterpret_cast<const byte *>(m_iv.data()));

    } catch (const CryptoPP::Exception& e) {
        throw SymmetricAlgorithmException(e.what());
    }
}

SymetricAlgorithm* AESAlgorithmCBCMode::clone()
{
    return (m_key.isEmpty() || m_iv.isEmpty()) ?
            new AESAlgorithmCBCMode() : new AESAlgorithmCBCMode(m_key, m_iv);
}

QByteArray AESAlgorithmCBCMode::decrypt(const QByteArray& encrypted)
{
    if (m_key.isEmpty() || m_iv.isEmpty()) {
        throw SymmetricAlgorithmException("Key has not been generated or set");
    }
    std::string plain;
    try {
        StringSource(reinterpret_cast<const byte*>(encrypted.data()), encrypted.length(), true,
                new StreamTransformationFilter(m_aesDecryption,
                        new StringSink(plain)));
    } catch (const CryptoPP::Exception& e) {
        throw SymmetricAlgorithmException(e.what());
    }

    return QByteArray(plain.c_str(), plain.length());

}

QByteArray AESAlgorithmCBCMode::encrypt(const QByteArray& plain)
{
    if (m_key.isEmpty() || m_iv.isEmpty()) {
        throw SymmetricAlgorithmException("Key has not been generated or set");
    }
    std::string encrypted;

    try {
        StringSource(reinterpret_cast<const byte*>(plain.data()), plain.length(), true,
                new StreamTransformationFilter(m_aesEncryption,
                        new StringSink(encrypted)));

    } catch (const CryptoPP::Exception& e) {
        throw SymmetricAlgorithmException(e.what());
    }

    return QByteArray(encrypted.c_str(), encrypted.length());
}

const QByteArray& AESAlgorithmCBCMode::generateKey(int keySize, int blockSize)
{
    byte *key;
    byte *iv;

    if (!m_allowedKeySizes.contains(keySize)
            || !m_allowedBlockSizes.contains(blockSize)) {
        throw SymmetricAlgorithmException("Unsupported IV or Key length");
    }
    m_key.clear();
    m_iv.clear();
    AutoSeededRandomPool prng;

    key = new byte[keySize];
    iv = new byte[blockSize];

    try {
        //generate random key and iv
        prng.GenerateBlock(key, keySize);
        prng.GenerateBlock(iv, blockSize);

        //try to set them
        m_aesEncryption.SetKeyWithIV(key, keySize, iv);
        m_aesDecryption.SetKeyWithIV(key, keySize, iv);

        m_key = QByteArray(reinterpret_cast<const char*>(key), keySize);
        m_iv = QByteArray(reinterpret_cast<const char*>(iv), blockSize);
    } catch (const CryptoPP::Exception& e) {
        delete[] key;
        delete[] iv;
        throw SymmetricAlgorithmException(e.what());
    }

    delete[] key;
    delete[] iv;

    return m_key;
}

void AESAlgorithmCBCMode::setKey(const QByteArray& key)
{
    if (!m_allowedKeySizes.contains(key.length())) {
        throw SymmetricAlgorithmException("Unsupported key length");
    }

    m_key = key;

    if (!m_iv.isEmpty()) {
        try {
            m_aesEncryption.SetKeyWithIV(
                    reinterpret_cast<const byte *>(m_key.data()),
                    static_cast<unsigned>(m_key.length()),
                    reinterpret_cast<const byte *>(m_iv.data()));

            m_aesDecryption.SetKeyWithIV(
                    reinterpret_cast<const byte *>(m_key.data()),
                    static_cast<unsigned>(m_key.length()),
                    reinterpret_cast<const byte *>(m_iv.data()));
        } catch (const CryptoPP::Exception& e) {
            m_key = QByteArray();
            throw SymmetricAlgorithmException(e.what());
        }
    }
}

QList<int> AESAlgorithmCBCMode::getSuitableKeySizes()
{

    return m_allowedKeySizes;
}

QList<int> AESAlgorithmCBCMode::getSuitableBlockSizes()
{
    return m_allowedBlockSizes;
}

const QByteArray& AESAlgorithmCBCMode::getIv()
{
    return m_iv;
}

void AESAlgorithmCBCMode::setIv(const QByteArray& iv)
{
    if (!m_allowedBlockSizes.contains(iv.length())) {
        throw SymmetricAlgorithmException("Unsupported IV length");
    }

    m_iv = iv;

    if (!m_key.isEmpty()) {
        try {
            m_aesEncryption.SetKeyWithIV(
                    reinterpret_cast<const byte *>(m_key.data()),
                    static_cast<unsigned>(m_key.length()),
                    reinterpret_cast<const byte *>(m_iv.data()));

            m_aesDecryption.SetKeyWithIV(
                    reinterpret_cast<const byte *>(m_key.data()),
                    static_cast<unsigned>(m_key.length()),
                    reinterpret_cast<const byte *>(m_iv.data()));

        } catch (const CryptoPP::Exception& e) {
            m_iv = QByteArray();
            throw SymmetricAlgorithmException(e.what());
        }
    }
}

} //namespace Cryptographic
} //namespace INZ_project
