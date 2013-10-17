/**
 * @file  ConversationInterface.h
 * @brief  Definition of the Class INZ_project::AAA::ConversationInterface
 * @date  13-sie-2013 17:38:10
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_FF532278_8F94_479e_AC6E_C6195432C273__INCLUDED_)
#define EA_FF532278_8F94_479e_AC6E_C6195432C273__INCLUDED_

#include <QByteArray>

namespace INZ_project {
namespace AAA {

/**
 * @brief Base class for conversation between AAAModule and client
 */
class ConversationInterface
{

public:
    /**
     * @brief Constructor
     */
    ConversationInterface();

    /**
     * @brief Destructor
     */
    virtual ~ConversationInterface();

    /**
     * @brief Sends the message to the client which is authorized
     * @param[in] message to be sent
     * @return Number of bytes sent or -1 if an error occurred.
     */
    virtual int sendMessage(const QByteArray& message) = 0;

    /**
     * @brief Receives the message from client
     * @param[in] timeout number of mili seconds to wait for message.
     * if -1 passed waits forever.
     * @param[out] ok value to be set. True if message has been received,
     * false if an error occurred or timeout reached
     */
    virtual QByteArray receiveMessage(qint64 timeout = -1,bool *ok = 0L) = 0;

};

} //namespace AAA
} //namespace INZ_project
#endif // !defined(EA_FF532278_8F94_479e_AC6E_C6195432C273__INCLUDED_)
