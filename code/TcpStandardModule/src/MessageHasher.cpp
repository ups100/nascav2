/**
 * @file  MessageHasher.cpp
 * @brief  Implementation of the Class INZ_project::TcpStandardModule::MessageHasher
 * @date  23-11-2013 11:09:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */
#include <QtEndian>

#include "MessageHasher.h"
#include "LogEntry.h"

namespace INZ_project {

using namespace Cryptographic;

namespace TcpStandardModule {

MessageHasher::MessageHasher(boost::shared_ptr<MessageSink> messageSink,
        HashAlgorithm *alg)
        : MessageDecorator(messageSink), m_hash(alg)
{

}

void MessageHasher::transformReadData(const QByteArray& message)
{
    QByteArray rawSize = message.left(4);
    qint32 size = qFromBigEndian<qint32>((const uchar*) rawSize.data());
    if ((size <= 0) || (message.size() - 4 - size <= 0)) {
        //message is not well-formed
        LOG_ENTRY(MyLogger::ERROR,
                "Wrong message format. Total size: "<<message.size()<<" Hash Size: "<<size);

        emit SinkError(WrongFormat);
    } else {
        //let's try to verify it
        QByteArray rawMessage = message.right(message.size() - 4);
        try {
            if (m_hash->verifyHash(rawMessage.left(rawMessage.size() - size),
                    rawMessage.right(size))) {
                appendToBuffer(rawMessage.left(rawMessage.size() - size));
            } else {
                LOG_ENTRY(MyLogger::ERROR,
                        "Hash verification failed. Message corrupted.");
                emit SinkError(MessageCorrupted);
            }
        } catch (const HashAlgorithm::HashAlgorithmException &e) {
            LOG_ENTRY(MyLogger::ERROR,
                    "Unable to validate a message: "<<message<<" because: "<<e.what());
            emit SinkError(MessageCorrupted);
        }
    }
}

QByteArray MessageHasher::transformWriteData(const QByteArray& message)
{
    QByteArray hash;
    qint32 hashSize;
    try {
        hash = m_hash->generateHash(message);
        hashSize = hash.size();
    } catch (const HashAlgorithm::HashAlgorithmException &e) {
        LOG_ENTRY(MyLogger::ERROR,
                "Unable to hash a message: "<<message<<" because: "<<e.what());
        emit SinkError(WrongData);
    }
    hashSize = qToBigEndian(hashSize);
    return QByteArray((char *)&hashSize, 4) + message + hash;
}

void MessageHasher::closeThisSink()
{

}

} //namespace TcpStandardModule
} //namespace INZ_project
