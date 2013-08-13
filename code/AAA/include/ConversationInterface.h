/**
* @file  ConversationInterface.h
* @brief  Definition of the Class ConversationInterface
* @date  13-sie-2013 17:38:10
* @author Krysztof Opasiak <ups100@tlen.pl>
*/

#if !defined(EA_FF532278_8F94_479e_AC6E_C6195432C273__INCLUDED_)
#define EA_FF532278_8F94_479e_AC6E_C6195432C273__INCLUDED_

namespace INZ_project
{
	namespace AAA
	{
		class ConversationInterface
		{

		public:
			ConversationInterface();
			virtual ~ConversationInterface();

			virtual QByteArray providePassword() =0;
			virtual QByteArray provideUserName() =0;

		};

	}

}
#endif // !defined(EA_FF532278_8F94_479e_AC6E_C6195432C273__INCLUDED_)
