/**
 * @file  AAAModule.h
 * @brief  Definition of the Class AAAModule
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

class AAAModule
{

public:
    AAAModule();
    virtual ~AAAModule();

    virtual AAAModule* clone() =0;
    virtual Base::Client* getAuthorizedClient() =0;
    virtual bool run() =0;
    void setConversationInterface(ConversationInterface* conv);

protected:
    ConversationInterface *m_conversationInterface;
};

} //namespace AAA
} //namespace INZ_project
#endif // !defined(EA_E50F9419_49C3_4ff4_83E4_7F91A1A4D666__INCLUDED_)
