/**
 * @file  DataPortion.h
 * @brief  Definition of the Class INZ_project::Base::DataPortion
 * @date  07-10-2013 19:55:36
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_3F47DABF_3EBE_4bb2_82FB_628B9CD3C449__INCLUDED_)
#define EA_3F47DABF_3EBE_4bb2_82FB_628B9CD3C449__INCLUDED_

#include <QString>
#include <QList>
#include <stdexcept>
#include <boost/shared_ptr.hpp>

namespace INZ_project {
namespace Base {

class ClientSession;
class DataProvider;
class Log;

class DataPortion
{

public:
    /**
     * @brief Constructor
     * @param[in] logs to be stored in this data portion.
     * This object takes the ownership of this param
     * @param[in] client who send this logs.
     * This object does NOT take the ownership of this param
     * @param[in] provider which received those logs
     * This object does NOT take the ownership of this param
     */
    DataPortion(const QList<boost::shared_ptr<Log> >& logs, const ClientSession* client,
            const DataProvider* provider);

    /**
     * @brief Destructor
     */
    virtual ~DataPortion();

    /**
     * @brief Get's the logs in icinga suitable format.
     * @return icinga formated logs
     */
    const QList<boost::shared_ptr<Log> >& getLogs() const;

    /**
     * @brief Get's the size of logs portion
     * @return size of logs
     */
    qint64 getSize() const;

    /**
     * @brief Get's the id of client which send this data
     * @return client id
     */
    const QString& getClient() const;

    /**
     * @brief Gets the id of provider which received this logs portion
     * @return provider id
     */
    const QString& getProvider() const;

    /**
     * @brief Exception class for errors occurred creating this portion
     */
    class NoRightsException : public std::runtime_error
    {
    public:
        /**
         * @brief Constructor
         * @param[in] arg exception string
         */
        explicit NoRightsException(const std::string& arg)
                : std::runtime_error(arg)
        {
        }
    };

private:
    /**
     * @brief Client ID
     */
    QString m_client;
    /**
     * @brief Provider ID
     */
    QString m_provider;
    /**
     * @brief Size of this data portion
     */
    qint64 m_size;
    /**
     * @brief Logs stored in this data portion
     */
    QList<boost::shared_ptr<Log> > m_logs;
};

} //namespace Base
} //namespace INZ_project
#endif // !defined(EA_3F47DABF_3EBE_4bb2_82FB_628B9CD3C449__INCLUDED_)
