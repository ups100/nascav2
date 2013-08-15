/**
 * @file  ConfigurationParser.h
 * @brief  Definition of the Class ConfigurationParser
 * @date  13-sie-2013 17:38:10
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_E9E17D99_BB22_452f_BD9C_2B30B7D935A1__INCLUDED_)
#define EA_E9E17D99_BB22_452f_BD9C_2B30B7D935A1__INCLUDED_

#include <QList>
#include <QString>

namespace INZ_project {
namespace Base {
class ConfigurationParser
{

public:
    ConfigurationParser();
    virtual ~ConfigurationParser();

    bool checkConfFile();
    QList<QString>* getDataConsumers();
    QList<QString>* getDataProviders();
    void getRoutes(const QString& client, const QString& DataProvider);

};

} //namespace Base
} //namespace INZ_project
#endif // !defined(EA_E9E17D99_BB22_452f_BD9C_2B30B7D935A1__INCLUDED_)
