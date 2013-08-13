/**
* @file  SymetricAlgorithm.h
* @brief  Definition of the Class SymetricAlgorithm
* @date  13-sie-2013 17:38:12
* @author Krysztof Opasiak <ups100@tlen.pl>
*/

#if !defined(EA_7969CDE2_2366_4739_9A40_DB426E0C42FC__INCLUDED_)
#define EA_7969CDE2_2366_4739_9A40_DB426E0C42FC__INCLUDED_

namespace INZ_project
{
	namespace Cryptographic
	{
		class SymetricAlgorithm
		{

		public:
			SymetricAlgorithm();
			virtual ~SymetricAlgorithm();

			virtual SymetricAlgorithm* clone() =0;
			virtual QByteArray decrypt(const QByteArray& encrypted) =0;
			virtual QByteArray encrypt(const QByteArray& plain) =0;
			virtual const QByteArray& generateKey() =0;
			virtual const QByteArray& getKey() =0;
			virtual void setKey(const QByteArray& key) =0;

		};

	}

}
#endif // !defined(EA_7969CDE2_2366_4739_9A40_DB426E0C42FC__INCLUDED_)
