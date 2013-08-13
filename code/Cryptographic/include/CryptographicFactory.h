/**
* @file  CryptographicFactory.h
* @brief  Definition of the Class CryptographicFactory
* @date  13-sie-2013 17:38:10
* @author Krysztof Opasiak <ups100@tlen.pl>
*/

#if !defined(EA_27CC8965_95B9_4981_8DB5_2701B8B1050C__INCLUDED_)
#define EA_27CC8965_95B9_4981_8DB5_2701B8B1050C__INCLUDED_

#include "SymetricAlgorithm.h"
#include "AsymetricAlgorithm.h"

namespace INZ_project
{
	namespace Cryptographic
	{
		class CryptographicFactory
		{

		public:
			CryptographicFactory();
			virtual ~CryptographicFactory();
			INZ_project::Cryptographic::SymetricAlgorithm *m_SymetricAlgorithm;
			INZ_project::Cryptographic::AsymetricAlgorithm *m_AsymetricAlgorithm;

			static const QList<QString>& getASymAlgorithmList();
			static CryptographicFactory* getInstance();
			static const QList<QString>& getSymAlgorithmList();
			static const QList<QString>& getSymetricAlgorithmList();

		};

	}

}
#endif // !defined(EA_27CC8965_95B9_4981_8DB5_2701B8B1050C__INCLUDED_)
