/**
 * @file  AsymetricAlgorithm.h
 * @brief  Definition of the Class AsymetricAlgorithm
 * @date  13-sie-2013 17:38:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_EB7D702C_2C96_4fe9_AC1C_5DC5DDE2C809__INCLUDED_)
#define EA_EB7D702C_2C96_4fe9_AC1C_5DC5DDE2C809__INCLUDED_

#include <QByteArray>

namespace INZ_project {
namespace Cryptographic {
class AsymetricAlgorithm
{

public:
    AsymetricAlgorithm();
    virtual ~AsymetricAlgorithm();

    virtual AsymetricAlgorithm* clone() =0;
    virtual QByteArray decrypt(const QByteArray& encrypted) =0;
    QByteArray encrypt(const QByteArray& plain);
    virtual void generateKeys() =0;
    const QByteArray& getPrivateKey();
    virtual const QByteArray& getPublicKey() =0;
    void setPublicKey(const QByteArray& partnerKey);

};

}

}
#endif // !defined(EA_EB7D702C_2C96_4fe9_AC1C_5DC5DDE2C809__INCLUDED_)
