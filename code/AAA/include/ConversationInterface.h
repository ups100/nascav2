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
     * @brief This function should collect a password from user and return it.
     * @note This function should not cache a password but each time it's called
     * it should ask client once again
     */
    virtual QByteArray providePassword() = 0;

    /**
     * @brief This function should collect a user name from client and return it.
     * @note This function should not cache a user name but each time it's called
     * it should ask client once again
     */
    virtual QByteArray provideUserName() = 0;

};

} //namespace AAA
} //namespace INZ_project
#endif // !defined(EA_FF532278_8F94_479e_AC6E_C6195432C273__INCLUDED_)
