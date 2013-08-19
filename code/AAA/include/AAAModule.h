/**
 * @file  AAAModule.h
 * @brief  Definition of the Class INZ_project::AAA::AAAModule
 * @date  13-sie-2013 17:38:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_E50F9419_49C3_4ff4_83E4_7F91A1A4D666__INCLUDED_)
#define EA_E50F9419_49C3_4ff4_83E4_7F91A1A4D666__INCLUDED_

#include "ConversationInterface.h"
#include "Client.h"

namespace INZ_project {
namespace Base {
class Client;
}

namespace AAA {

/**
 * @brief Base class for AAA modules
 */
class AAAModule
{

public:
    /**
     * @brief Constructor
     */
    AAAModule();

    /**
     * @brief Destructor
     */
    virtual ~AAAModule();

    /**
     * @brief Return the deep copy of this object
     */
    virtual AAAModule* clone() = 0;

    /**
     * @brief Gets the authorized client
     * @return New instance of Client class or NULL if
     * client has not been authorized
     */
    virtual Base::Client* getAuthorizedClient() = 0;

    /**
     * @brief Runs the identification and authorization process
     */
    virtual bool run() = 0;

    /**
     * @brief Sets the conversation interface for this object
     * @param[in] conv conversation interface to be set
     * @note This object does not take the ownership of conv param
     */
    void setConversationInterface(ConversationInterface* conv);

protected:

    /**
     * @brief Currently set conversation interface.
     * @note If interface has not been set this field is NULL.
     */
    ConversationInterface *m_conversationInterface;
};

} //namespace AAA
} //namespace INZ_project
#endif // !defined(EA_E50F9419_49C3_4ff4_83E4_7F91A1A4D666__INCLUDED_)
