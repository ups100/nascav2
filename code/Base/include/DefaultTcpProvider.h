/**
 * @file  DefaultTcpProvider.h
 * @brief  Definition of the Class INZ_project::Base::DefaultTcpProvider
 * @date  26-11-2013 22:38:35
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include <QThread>
#include "AdditionalThread.h"
#include "DataProviderRegister.h"

namespace INZ_project {
namespace Base {

DATA_PROVIDER(DefaultTcpProvider, DefaultTcpProvider)
{
public:
    DefaultTcpProvider();

    virtual ~DefaultTcpProvider();

    virtual void close();

protected:
    virtual int initImpl(const QString& additionalData, const QString& providerId,
                const QString& providerType);

private:

    int run();

    TcpStandardModule::AdditionalThread m_thread;
};

} //namespace Base
} //namespace INZ_project
