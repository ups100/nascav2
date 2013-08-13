/**
* @file  AAAModule.h
* @brief  Definition of the Class AAAModule
* @date  13-sie-2013 17:38:09
* @author Krysztof Opasiak <ups100@tlen.pl>
*/

#if !defined(EA_E50F9419_49C3_4ff4_83E4_7F91A1A4D666__INCLUDED_)
#define EA_E50F9419_49C3_4ff4_83E4_7F91A1A4D666__INCLUDED_

#include "ConversationInterface.h"

namespace INZ_project
{
	namespace AAA
	{
		class AAAModule
		{

		public:
			AAAModule();
			virtual ~AAAModule();
			INZ_project::AAA::ConversationInterface *m_ConversationInterface;

			virtual AAAModule* clone() =0;
			virtual Client* getAuthorizedClient() =0;
			virtual bool run() =0;
			void setConversationInterface(ConversationInterface* conv);

		};

	}

}
#endif // !defined(EA_E50F9419_49C3_4ff4_83E4_7F91A1A4D666__INCLUDED_)