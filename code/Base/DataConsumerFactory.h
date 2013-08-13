/**
* @file  DataConsumerFactory.h
* @brief  Definition of the Class DataConsumerFactory
* @date  13-sie-2013 17:38:11
* @author Krysztof Opasiak <ups100@tlen.pl>
*/

#if !defined(EA_9F993516_AE1A_4dc6_AB52_3495ECADF809__INCLUDED_)
#define EA_9F993516_AE1A_4dc6_AB52_3495ECADF809__INCLUDED_

#include "DataConsumer.h"

namespace INZ_project
{
	namespace Base
	{
		class DataConsumerFactory
		{

		public:
			DataConsumerFactory();
			virtual ~DataConsumerFactory();
			INZ_project::Base::DataConsumer *m_DataConsumer;

			static DataConsumer* getDataConsumer();
			static DataConsumerFactory* getInstance();
			static void registerDataConsumer(DataConsumer* consumer, const QString& id);

		};

	}

}
#endif // !defined(EA_9F993516_AE1A_4dc6_AB52_3495ECADF809__INCLUDED_)
