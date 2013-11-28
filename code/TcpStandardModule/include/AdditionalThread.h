/**
 * @file  AdditionalThread.h
 * @brief  Definition of the Class INZ_project::TcpStandardModule::AdditionalThread
 * @date  17-11-2013 11:09:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __ADDITIONAL_THREAD_H__
#define __ADDITIONAL_THREAD_H__

#include <QObject>
#include <QThread>
#include <boost/function.hpp>

namespace INZ_project {
namespace TcpStandardModule {

class AdditionalThread : public QThread
{
public:
    /**
     * @brief Constructor
     * @param[in] runFunction function to be executed in run method
     */
    AdditionalThread(boost::function<int()> runFunction);

    using QThread::exec;

    /**
     * @brief Ends the thread synchronously
     * @note This function should not be called in context of this thread
     */
    void endThread();

    /**
     * @brief Starts the thread synchronously
     * @note This function should not be called in context of this thread
     */
    void startThread();

protected:
    virtual void run();

private:
    /**
     * @brief Function to execute in run Function
     */
    boost::function<int()> m_runFunction;
};

} //namespace TcpStandardModule
} //namespace INZ_project

#endif //__ADDITIONAL_THREAD_H__
