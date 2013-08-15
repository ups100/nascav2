/**
 * @file  DataConsumerFactory.h
 * @brief  Definition of the Class DataConsumerFactory
 * @date  13-sie-2013 17:38:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_9F993516_AE1A_4dc6_AB52_3495ECADF809__INCLUDED_)
#define EA_9F993516_AE1A_4dc6_AB52_3495ECADF809__INCLUDED_

#include <QHash>
#include <QString>

namespace INZ_project {
namespace Base {

class DataConsumer;

class DataConsumerFactory
{

public:
    DataConsumerFactory();
    virtual ~DataConsumerFactory();

    static DataConsumer* getDataConsumer();
    static DataConsumerFactory* getInstance();
    static void registerDataConsumer(DataConsumer* consumer, const QString& id);

private:
    QHash<QString, DataConsumer*> m_dataConsumers;
};

} //namespace Base
} //namespace INZ_project
#endif // !defined(EA_9F993516_AE1A_4dc6_AB52_3495ECADF809__INCLUDED_)
