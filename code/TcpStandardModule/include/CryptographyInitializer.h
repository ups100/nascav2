/**
 * @file  CryptographyInitializer.h
 * @brief  Definition of the Class INZ_project::TcpStandardModule::CryptographyInitializer
 * @date  08-12-2013 13:56:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __CRYPTOGRAPHY_INITIALIZER_H__
#define __CRYPTOGRAPHY_INITIALIZER_H__

#include "SessionPart.h"
#include "MessageSink.h"
#include "HashAlgorithm.h"
#include "AsymetricAlgorithm.h"
#include "SymetricAlgorithm.h"

#include <boost/shared_ptr.hpp>
#include <QTimer>
#include <QMap>
#include <QList>
#include <QSet>
#include <QString>
#include <QtEndian>

namespace INZ_project {
namespace Base {
class ClientSession;
class DataProvider;
}
namespace TcpStandardModule {

/**
 * @brief Session Part responsible for establishing cryptography
 */
class CryptographyInitializer : public SessionPart
{
Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param[in] allowedAlgorithms of symmetric cryptography
     * @param[in] asym Asymmetric algorithm used for establishing
     * connection
     * @param[in] provider to which this session belongs to
     * @param[in] hash algorithm for integrity insurance
     * @param[in] session to which this part belongs to
     * @param[in] milis timeout for communication with client
     */
    CryptographyInitializer(const QSet<QString>& allowedAlgorithms,
            INZ_project::Cryptographic::AsymetricAlgorithm *asym,
            INZ_project::Base::DataProvider *provider,
            INZ_project::Cryptographic::HashAlgorithm *hash, Session *session =
                    0L, int milis = 10000);

    /**
     * @brief Destructor
     */
    virtual ~CryptographyInitializer();

    virtual SessionPart * clone();

    virtual void execute();

    virtual void terminate();

    virtual void setTimeout(int milis);

private slots:

    /**
     * @brief Slot executed when error occurred in destination
     */
    void sinkError(MessageSink::SinkError error);

    /**
     * @brief Slot executed when new message has arrived
     */
    void readNextMessage();

    /**
     * @brief Slot executed when destination has been closed unexpectedly
     */
    void sinkClosed();

    /**
     * @brief Slot executed when client didn't response in given time
     */
    void requestTimedOut();

private:
    enum
    {
        WAITING_FOR_CLIENT_ID, WAITING_FOR_ALGORITHM, WAITING_FOR_KEY, NONE
    } m_state;

    /**
     * @brief Stops the timer and disconnects all signals from destination
     */
    void disconnectAll();

    /**
     * @brief Function executed when message has been received in WAITING_FOR_CLIENT_ID state.
     * @param[in] message which has been received
     */
    void clientIdReceived(const QByteArray& message);

    /**
     * @brief Function executed when message has been received in WAITING_FOR_ALGORITHM state.
     * @param[in] message which has been received
     */
    void algorithmReceived(const QByteArray& message);

    /**
     * @brief Function executed when message has been received in WAITING_FOR_KEY state.
     * @param[in] message which has been received
     */
    void keyReceived(const QByteArray& message);

    /**
     * @brief Informs if session part has been started
     */
    bool m_isStarted;

    /**
     * @brief Timer for waiting for response
     */
    QTimer m_timer;

    /**
     * @brief Allowed cryptography algorithms
     */
    QSet<QString> m_allowedAlgorithms;

    /**
     * @brief Asymmetric algorithm used for encryption
     */
    INZ_project::Cryptographic::AsymetricAlgorithm *m_asym;

    /**
     * @brief Hash algorithm
     */
    INZ_project::Cryptographic::HashAlgorithm *m_hash;

    /**
     * @brief Identifies client to which this session belongs
     */
    Base::ClientSession* m_clientSession;

    /**
     * @brief DataProvider to which this session belongs
     */
    Base::DataProvider* m_provider;

    /**
     * @brief Symmetric algorithm which will be used for encryption
     */
    INZ_project::Cryptographic::SymetricAlgorithm *m_sym;
};

} //namespace TcpStandardModule
} //namespace INZ_project

#endif //__CRYPTOGRAPHY_INITIALIZER_H__
