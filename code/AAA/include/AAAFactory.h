/**
 * @file  AAAFactory.h
 * @brief  Definition of the Class AAAFactory
 * @date  13-sie-2013 17:38:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_DDBCF13E_F8A4_4333_BE7E_7888BEB5EDEA__INCLUDED_)
#define EA_DDBCF13E_F8A4_4333_BE7E_7888BEB5EDEA__INCLUDED_

#include "AAAModule.h"

namespace INZ_project {
namespace AAA {
class AAAFactory
{

public:
    AAAFactory();
    virtual ~AAAFactory();
    INZ_project::AAA::AAAModule *m_AAAModule;

    static AAAModule* getAAAModule(const QString& id);
    static QList<QString> getAAAModulesList();
    static AAAFactory* getInstance();
    static void registerAAAModule(AAAModule* module, QString id);

};

}

}
#endif // !defined(EA_DDBCF13E_F8A4_4333_BE7E_7888BEB5EDEA__INCLUDED_)
