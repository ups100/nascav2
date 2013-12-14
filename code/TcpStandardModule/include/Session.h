/**
 * @file  Session.h
 * @brief  Definition of the Class INZ_project::TcpStandardModule::Session
 * @date  17-11-2013 10:56:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include <QObject>
#include <boost/shared_ptr.hpp>
#include "HashAlgorithm.h"

namespace INZ_project {
namespace Base {
class ClientSession;
}
namespace TcpStandardModule {

class MessageSink;
class SessionPart;

class Session : public QObject
{
Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param[in] destination endpoint of communication
     * @param[in] parts fixed parts of session which should be executed
     */
    Session(MessageSink *destination,
            const QList<boost::shared_ptr<SessionPart> >& parts);

    /**
     * @brief Destructor
     */
    virtual ~Session();

    /**
     * @brief Gets the current end point for communication
     */
    boost::shared_ptr<MessageSink> getDestination();

    /**
     * @brief Sets new destination for communication
     */
    void setDestination(boost::shared_ptr<MessageSink> destination);

    /**
     * @brief Schedules additional part of session to be executed
     * @param[in] sessionPart to be executed
     * @note This object takes the ownership of passed object
     */
    void appendSessionPart(SessionPart* sessionPart);

    /**
     * @brief Identifies a client to which this session belongs
     */
    void setClient(Base::ClientSession *client);

    /**
     * @brief Saves the hash algorithm
     */
    void setHashAlgorithm(Cryptographic::HashAlgorithm *hash);

    /**
     * @brief Gets the client to which this session belongs to
     * @return Client to which this session belongs or NULL if owner has not been set
     */
    boost::shared_ptr<Base::ClientSession> getClient();

    /**
     * @brief Gets the hash algorithm used in this session
     * @return Hash algorithm used in this session or NULL if hash algorithm has not been set
     */
    boost::shared_ptr<Cryptographic::HashAlgorithm> getHashAlgorithm();

signals:
    /**
     * @brief Signal emitted when session has been finished
     * @param[in] ok determines if all session parts has been finished successfully
     * @param[in] which session part has been finished
     * @note remember to not delete the which parameter, use deleteLater instead
     */
    void sessionFinished(bool ok, Session *which);

public slots:
    /**
     * @brief Tells session to stop.
     * @details when session stops, sessionFinished signal with false will be emitted
     */
    void stopSession();

    /**
     * @brief Starts the session
     */
    void startSession();

private slots:
    /**
     * @brief Slot executed when session part has been finished
     */
    void sessionPartFinished(bool result);

    /**
     * @brief Slot executed when all session parts has been finished and we are
     *  closing the sink
     */
    void destinationClosed();

private:
    void clearPartsLeft();

    bool m_result;
    SessionPart* m_currentSessionPart;
    QList<SessionPart*> m_partsLeft;
    boost::shared_ptr<MessageSink> m_destination;
    boost::shared_ptr<Base::ClientSession> m_client;
    boost::shared_ptr<Cryptographic::HashAlgorithm> m_hash;
};

} //namespace TcpStandardModule
} //namespace INZ_project
