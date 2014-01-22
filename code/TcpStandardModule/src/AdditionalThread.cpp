/**
 * @file  AdditionalThread.cpp
 * @brief  Implementation of the Class INZ_project::TcpStandardModule::AdditionalThread
 * @date  17-11-2013 11:09:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "AdditionalThread.h"
#include <QEventLoop>
#include <QTimer>

namespace INZ_project {
namespace TcpStandardModule {

AdditionalThread::AdditionalThread(boost::function<int ()> runFunction)
 : m_runFunction(runFunction)
{

}

void AdditionalThread::run()
{
    m_runFunction();
}

void AdditionalThread::endThread()
{
    if(!this->isRunning()) {
        return;
    }
    QEventLoop loop;
    connect(this, SIGNAL(finished()), &loop, SLOT(quit()));

    QTimer::singleShot(0, this, SLOT(quit()));
    loop.exec();
}

void AdditionalThread::startThread()
{
    QEventLoop loop;
    connect(this, SIGNAL(started()), &loop, SLOT(quit()));
    connect(this, SIGNAL(finished()), &loop, SLOT(quit()));

    QTimer::singleShot(0, this, SLOT(start()));
    loop.exec();
}


} //namespace TcpStandardModule
} //namespace INZ_project
