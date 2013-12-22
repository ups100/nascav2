/**
 * @file  MessageSigner.cpp
 * @brief  Implementation of the Class INZ_project::TcpStandardModule::MessageSigner
 * @date  23-11-2013 11:09:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */
#include <QtEndian>

#include "MessageSigner.h"
#include "LogEntry.h"

namespace INZ_project {

using namespace Cryptographic;

namespace TcpStandardModule {

MessageSigner::MessageSigner(boost::shared_ptr<MessageSink> messageSink,
        Cryptographic::SignAlgorithm *signer)
        : MessageDecorator(messageSink), m_signer(signer)
{

}

void MessageSigner::transformReadData(const QByteArray& message)
{
    QByteArray rawSize = message.left(4);
    qint32 size = qFromBigEndian<qint32>((const uchar*) rawSize.data());
    if ((size <= 0) || (message.size() - 4 - size < 0)) {
        //message is not well-formed
        LOG_ENTRY(MyLogger::ERROR,
                "Wrong message format. Total size: "<<message.size()<<" Hash Size: "<<size);
        ///////////////////////////////////
        //emit SocketError() TODO
        ///////////////////////////////////
    } else {
        //we have the message now it's time to decrypt the sign
        QByteArray rawMessage = message.mid(4, message.size() - 4 - size);
        QByteArray rawSignature = message.right(size);
        try {
            if (m_signer->verify(rawMessage, rawSignature)) {
                appendToBuffer(rawMessage.left(rawMessage.size() - size));
            } else {
                LOG_ENTRY(MyLogger::ERROR,
                        "Hash verification failed. Message corrupted.");
                ///////////////////////////////////
                //emit SocketError() TODO
                ///////////////////////////////////
            }
        } catch (const SignAlgorithm::SignAlgorithmException &e) {
            LOG_ENTRY(MyLogger::ERROR,
                    "Unable to validate a message: "<<message<<" because: "<<e.what());
            ///////////////////////////////////
            //emit SocketError() TODO
            ///////////////////////////////////
        }
    }
}

QByteArray MessageSigner::transformWriteData(const QByteArray& message)
{
    QByteArray signature;
    qint32 signatureSize;
    try {
        signature = m_signer->sign(message);
        signatureSize = signature.size();
    } catch (const SignAlgorithm::SignAlgorithmException &e) {
        LOG_ENTRY(MyLogger::ERROR,
                "Unable to validate a message: "<<message<<" because: "<<e.what());
        ///////////////////////////////////
        //emit SocketError() TODO
        ///////////////////////////////////
    }
    signatureSize = qToBigEndian(signatureSize);
    return QByteArray((char *)&signatureSize, 4) + message + signature;
}

void MessageSigner::closeThisSink()
{

}

} //namespace TcpStandardModule
} //namespace INZ_project
