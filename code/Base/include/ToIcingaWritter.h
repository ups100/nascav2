/**
 * @file  ToIcingaWritter.h
 * @brief  Definition of the Class INZ_project::Base::ToIcingaWritter
 * @date  20-12-2013 22:38:35
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include <QThread>
#include "DataConsumerRegister.h"
#include "AdditionalThread.h"

namespace INZ_project {
namespace Base {

DATA_CONSUMER(ToIcingaWritter, ToIcingaWritter)
{
public:
    ToIcingaWritter();

    virtual ~ToIcingaWritter();

    virtual void close();

    void consumeDataPortion(const ReadPortion *portion, QObject* toConfirm,
            QString confirmMethod);

protected:
    virtual int initImpl(const QString& additionalData,
            const QString& providerId, const QString& providerType);

private:

    int parseOptions(const QString& options);
    int run();

    TcpStandardModule::AdditionalThread m_thread;

    QString m_pipeFile;
    int m_pipeDesc;
};

} //namespace Base
} //namespace INZ_project
