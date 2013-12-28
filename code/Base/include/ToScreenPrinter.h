/**
 * @file  ToScreenPrinter.h
 * @brief  Definition of the Class INZ_project::Base::ToScreenPrinter
 * @date  26-11-2013 22:38:35
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include <QThread>
#include "DataConsumerRegister.h"
#include "AdditionalThread.h"

namespace INZ_project {
namespace Base {

DATA_CONSUMER(ToScreenPrinter, ToScreenPrinter)
{
public:
    ToScreenPrinter();

    virtual ~ToScreenPrinter();

    virtual void close();

    void consumeDataPortion(const ReadPortion *portion, QObject* toConfirm,
            QString confirmMethod);

protected:
    virtual int initImpl(const QString& additionalData,
            const QString& providerId, const QString& providerType);

private:

    int run();

    TcpStandardModule::AdditionalThread m_thread;
};

} //namespace Base
} //namespace INZ_project
