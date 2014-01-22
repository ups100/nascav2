/**
 * @file  Registrator.h
 * @brief  Definition of MyLogger::Logger template class
 * @date  15-08-2013
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */
#ifndef __MY_LOGGER_H__
#define __MY_LOGGER_H__

#include <QTextStream>
#include <QMutex>
#include <QFile>

namespace MyLogger {

/**
 * @brief Levels of logs
 */
enum LogLevel
{
    INFO = 0, DEBUG = 1, WARN = 2, ERROR = 3, FATAL = 4
};

/**
 * @brief Main class for logs aggregation
 */
class Logger
{
public:
    /**
     * @brief Creates new stream for log message
     * @return New instance of stream for log message
     * @note Object returned from this function should be deleted.
     */
    static QTextStream* getNewStream();

    /**
     * @brief Gets the instance of Logger object
     * @return Instance of logger object.
     */
    static Logger *getInstance();

    /**
     * @brief Main function for log collection
     * @details When b parameter of template is true then log is submitted otherwise
     * log message is ignored.
     *
     * @param[in] logLevel level of this log importance
     * @param[in] line where log appeared
     * @param[in] file where log appeared
     * @param[in] fun function where log appeared
     * @param[in,out] message log message
     *
     * @note message parameter is deleted after function execution
     */
    template<int i>
    static inline void appendLog(LogLevel logLevel, int line, const char *file,
            const char *fun, QTextStream &message);
private:
    /**
     * @brief Constructor
     */
    Logger();

    /**
     * @brief Trims file path up to 3 slashes.
     * @details If file path does not consist of 3 slashes returns the whole path;
     * @param[in] filePath to be trimmed
     * @return pointer to place with 3 folder levels down. eg /home/aaa/bb/cc/main.cpp
     * is trimed to bb/cc/main.cpp
     */
    static const char* trimFilePath(const char *filePath);

    /**
     * @brief Helper function to provide class instance
     */
    static Logger *getInstanceHelper();

    /**
     * @brief Mutex for instance getter synchronization
     */
    static QMutex m_mutex;

    /**
     * @brief Mutex for log output synchronization
     */
    QMutex m_outputMutex;

    /**
     * @brief Instance of this class.
     */
    static Logger* m_instance;

    /**
     * @brief Place where logs are stored
     */
    QIODevice *m_outputDevice;
};

template<>
inline void Logger::appendLog<0>(LogLevel logLevel, int line, const char *file,
        const char *fun, QTextStream &message)
{
    delete &message;
}

template<int i>
inline void Logger::appendLog(LogLevel logLevel, int line, const char *file,
        const char *fun, QTextStream &message)
{
    QString *messageString = message.string();
    QString log;
    message.setString(&log, QIODevice::WriteOnly);
    file = trimFilePath(file);

    message<<"[ "<<time(0L)<<" ]";
    switch(logLevel) {
        case INFO:
            message<<"[ INFO ]";
            break;
        case DEBUG:
            message<<"[ DEBUG ]";
            break;
        case WARN:
            message<<"[ WARN ]";
            break;
        case ERROR:
            message<<"[ ERROR ]";
            break;
        case FATAL:
            message<<"[ FATAL ]";
            break;
        default:
            message<<"[ UNKNOWN ]";
            break;
    };

    message<<"[ "<<file<<" ]"<<"[ "<<fun<<" ]"<<"[ "<<line<<" ]: "<<*messageString;
    if(log.right(1) != "\n") log.append("\n");

    Logger *logger = Logger::getInstance();
    {
        QMutexLocker locker(&(logger->m_outputMutex));
        QTextStream(logger->m_outputDevice)<<log;
    }

    delete messageString;
    delete &message;
}

} //namespace MyLogger

#endif // ifndef __INZ_PROJECT_LOGGER_H__
