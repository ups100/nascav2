/**
 * @file  ToScreenPrinter.cpp
 * @brief Implementation of the Class ToScreenPrinter
 * @date  28-11-2013 21:21:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "ToScreenPrinter.h"
#include "LogEntry.h"
#include <boost/bind.hpp>
#include <QObject>
#include <QMetaObject>
#include "ReadPortion.h"

namespace INZ_project {
namespace Base {

ToScreenPrinter::ToScreenPrinter()
        : m_thread(boost::bind(&ToScreenPrinter::run, this))
{

}

ToScreenPrinter::~ToScreenPrinter()
{

}

void ToScreenPrinter::close()
{
    m_thread.endThread();
}

void ToScreenPrinter::consumeDataPortion(const ReadPortion *portion)
{
    foreach(QString log, portion->getLogs()) {
        LOG_ENTRY(MyLogger::INFO, log);
    }

    QMetaObject::invokeMethod(sender(),
                        "confirmPortion", Qt::QueuedConnection,
                        Q_ARG(const ReadPortion*, portion));
}

int ToScreenPrinter::initImpl(const QString& additionalData,
        const QString& providerId, const QString& providerType)
{
    m_thread.startThread();
    this->moveToThread(&m_thread);
    return 0;
}

int ToScreenPrinter::run()
{
    LOG_ENTRY(MyLogger::INFO, "Zajebiaszczo");
    return m_thread.exec();
}

} //namespace Base
} //namespace INZ_project
