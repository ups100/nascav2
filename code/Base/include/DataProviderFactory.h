/**
 * @file  DataProviderFactory.h
 * @brief  Definition of the Class DataProviderFactory
 * @date  13-sie-2013 17:38:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_23160C1D_5A21_447d_841D_C07CB33C6AC4__INCLUDED_)
#define EA_23160C1D_5A21_447d_841D_C07CB33C6AC4__INCLUDED_

#include "DataProvider.h"

namespace INZ_project {
namespace Base {
class DataProviderFactory
{

public:
    DataProviderFactory();
    virtual ~DataProviderFactory();
    INZ_project::Base::DataProvider *m_DataProvider;

    static DataProvider * getDataProvider(const QString& id);
    static DataProviderFactory* getInstance();
    static void registerDataProvider(DataProvider* dataProvider, QString id);

};

}

}
#endif // !defined(EA_23160C1D_5A21_447d_841D_C07CB33C6AC4__INCLUDED_)
