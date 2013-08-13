/**
* @file  DataProvider.h
* @brief  Definition of the Class DataProvider
* @date  13-sie-2013 17:38:11
* @author Krysztof Opasiak <ups100@tlen.pl>
*/

#if !defined(EA_C29B231D_317B_4162_81AF_9381B6F6380C__INCLUDED_)
#define EA_C29B231D_317B_4162_81AF_9381B6F6380C__INCLUDED_

#include "Client.h"
#include "DataChannel.h"

namespace INZ_project
{
	namespace Base
	{
		class DataProvider
		{

		public:
			DataProvider();
			virtual ~DataProvider();
			INZ_project::Base::Client *m_Client;
			INZ_project::Base::DataChannel *m_DataChannel;

			virtual void clone() =0;
			virtual void close() =0;
			virtual void init(int argc, char ** argv) =0;

		};

	}

}
#endif // !defined(EA_C29B231D_317B_4162_81AF_9381B6F6380C__INCLUDED_)
