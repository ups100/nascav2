/**
 * @file  MessageCipher.cpp
 * @brief  Implementation of the Class INZ_project::TcpStandardModule::MessageCipher
 * @date  17-11-2013 11:09:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "MessageCipher.h"
#include "LogEntry.h"

namespace INZ_project {

using namespace Cryptographic;

namespace TcpStandardModule {

MessageCipher::MessageCipher(boost::shared_ptr<MessageSink> messageSink,
        AsymetricAlgorithm *alg)
        : MessageDecorator(messageSink), m_asym(alg)
{

}

MessageCipher::MessageCipher(boost::shared_ptr<MessageSink> messageSink,
        SymetricAlgorithm *alg)
        : MessageDecorator(messageSink), m_sym(alg)
{

}

void MessageCipher::transformReadData(const QByteArray& message)
{
    if (m_asym) {
        try {
            appendToBuffer(m_asym->decrypt(message));
        } catch (const AsymetricAlgorithm::AsymmetricAlgorithmException &e) {
            LOG_ENTRY(MyLogger::ERROR,
                    "Unable to decrypt a message: "<<message<<" because: "<<e.what());
            emit SinkError(MessageCorrupted);
        }
    } else {
        try {
            appendToBuffer(m_sym->decrypt(message));
        } catch (const SymetricAlgorithm::SymmetricAlgorithmException &e) {
            LOG_ENTRY(MyLogger::ERROR,
                    "Unable to decrypt a message: "<<message<<" because: "<<e.what());
            emit SinkError(MessageCorrupted);
        }
    }
}

QByteArray MessageCipher::transformWriteData(const QByteArray& message)
{
    QByteArray encrypted;
    if (m_asym) {
        try {
            encrypted = m_asym->encrypt(message);
        } catch (const AsymetricAlgorithm::AsymmetricAlgorithmException &e) {
            LOG_ENTRY(MyLogger::ERROR,
                    "Unable to encrypt a message: "<<message<<" because: "<<e.what());
            emit SinkError(WrongData);
        }
    } else {
        try {
            encrypted = m_sym->encrypt(message);
        } catch (const SymetricAlgorithm::SymmetricAlgorithmException &e) {
            LOG_ENTRY(MyLogger::ERROR,
                    "Unable to encrypt a message: "<<message<<" because: "<<e.what());
            emit SinkError(WrongData);
        }
    }

    return encrypted;
}

void MessageCipher::closeThisSink()
{

}

} //namespace TcpStandardModule
} //namespace INZ_project
