/**
 * @file  Logger.cpp
 * @brief  Definition of MyLogger::Logger methods
 * @date  15-08-2013
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "Logger.h"
#include <QMutexLocker>
#include <QString>

namespace MyLogger {

QMutex Logger::m_mutex;
Logger* Logger::m_instance=0L;

QTextStream* Logger::getNewStream()
{
    return new QTextStream(new QString(), QIODevice::ReadOnly);
}

Logger* Logger::getInstance()
{
    if(m_instance == 0L) {
        QMutexLocker locker(&m_mutex);
        if(m_instance == 0L) {
            m_instance = Logger::getInstanceHelper();
        }
    }
    return m_instance;
}

Logger::Logger()
{
    time_t current = time(0);
    QString filename = QString("/tmp/nscav2.") + QString().setNum((long)current) + QString(".log");
    QFile *fileErr = new QFile(filename);
    fileErr->open(QIODevice::WriteOnly);
    m_outputDevice = fileErr;
}

const char* Logger::trimFilePath(const char *filePath)
{
    // /home/ble/blebleble/ble/ble
    //  ^p1  ^p2 ^tmp  ^p3->
    const char *p1,*p2,*p3, *tmp;

    char separator;
#ifdef WIN32
    separator = '\\';
#else
     separator = '/';
#endif
     if(*filePath == '\0') return filePath;

    p1 = (filePath[0] == separator) ? filePath +1: filePath;

    for(p2 = p1; *p2 != '\0'; ++p2) {
        if(*p2 == separator) {
            ++p2; break;
        }
    }
    if(*p2 == '\0') return p1;

    p3 =p2;
    // /home/ble/ble/ble/ble
    //  ^p1  ^p2 ^p3
    while(true) {
        for(tmp = p3; *p3 != '\0'; ++p3) {
            if(*p3 == separator) {
                p1 = p2;
                p2 = tmp;
                ++p3;
                break;
            }
        }
        if(*p3 == '\0') return p1;
    }

    //Suppress compiler warning;
    return p1;
}

Logger* Logger::getInstanceHelper()
{
    static Logger instance;
    return &instance;
}

} //namespace MyLogger
