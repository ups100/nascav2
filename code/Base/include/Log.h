/**
 * @file  Log.h
 * @brief  Definition of the Class INZ_project::Base::Log
 * @date  09-01-2014 18:55:36
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __BASE_LOG_CLASS_H__
#define __BASE_LOG_CLASS_H__

#include <QString>
#include <stdexcept>

namespace INZ_project {
namespace Base {

/**
 * @brief Class which represents single log
 */
class Log
{
public:
    /**
     * @brief Constructor
     * @param[in] time of this log entry in this time zone
     * @param[in] hostName where check has been performed
     * @param[in] serviceName which has been checked or empty QString if this is a host check
     * @param[in] state of service/host
     * @param[in] output of plugin
     */
    Log(time_t time, const QString& hostName, const QString& serviceName,
            int state, const QString output);

    /**
     * @brief Destructor
     */
    virtual ~Log();

    /**
     * @brief Gets the GMT time when this log has been created on client device
     * @return time in GMT
     */
    time_t getTime() const;

    /**
     * @brief Gets the host name where this log has been created
     * @return host name
     */
    const QString& getHostName() const;

    /**
     * @brief Gets the service name
     * @return Service name or empty QString if this is a host check
     */
    const QString& getServiceName() const;

    /**
     * @brief Gets the state of service/host
     */
    int getState() const;

    /**
     * @brief Gets the output of plugin
     * @return plugin output
     */
    const QString& getPluginOutput() const;

    /**
     * @brief Gets the log formatted according to icinga rules
     */
    const QString& getFormatedOutput() const;

    /**
     * @brief Exception class for errors occurred while parsing
     */
    class LogWrongFormatException : public std::runtime_error
    {
    public:
        /**
         * @brief Constructor
         * @param[in] arg exception string
         */
        explicit LogWrongFormatException(const std::string& arg)
                : std::runtime_error(arg)
        {
        }
    };

protected:

    /**
     * @brief Generates formated output and
     * @param[in] time of this log entry
     * @param[in] hostName where check has been performed
     * @param[in] serviceName which has been checked or empty QString if this is a host check
     * @param[in] state of service/host
     * @param[in] output of plugin
     * @throws LogException if some data is not suitable
     */
    virtual void generateFormatedOutput(time_t time, const QString& hostName,
            const QString& serviceName, int state, const QString output);
    /**
     * @brief Log formatted according to icinga rules
     */
    QString m_formated;

private:
    /**
     * @brief Time when this check has been performed in this system location
     */
    time_t m_time;
    /**
     * @brief Host name
     */
    QString m_hostName;
    /**
     * @brief Service name
     */
    QString m_serviceName;
    /**
     * @brief Service/host state
     */
    int m_state;
    /**
     * @brief Plugin output
     */
    QString m_pluginOutput;
};

} //namespace Base
} //namespace INZ_project

#endif //__BASE_LOG_CLASS_H__
