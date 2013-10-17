/**
 * @file  ClientSession.h
 * @brief  Definition of the Class ClientSession
 * @date  13-sie-2013 17:38:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_D6440D50_CCA5_46f1_8A41_6416AF2C70A2__INCLUDED_)
#define EA_D6440D50_CCA5_46f1_8A41_6416AF2C70A2__INCLUDED_

#include <QString>
#include "AAAModule.h"
#include "AAAFactory.h"
#include "ConversationInterface.h"
#include <boost/shared_ptr.hpp>

namespace INZ_project {
namespace Base {

class DataChannel;
class DataProvider;

/**
 * @brief Class which represents a single client connection.
 */
class ClientSession
{

public:
    /**
     * @brief Constructor
     * @param[in] clientId of client
     * @param[in] provider to which client has connected
     * @throws ClientException if no such client
     */
    ClientSession(const QString& clientId, const DataProvider* provider);

    /**
     * @brief Destructor
     */
    virtual ~ClientSession();

    /**
     * @brief Gets the client id
     * @return Client ID
     */
    const QString& getClientId() const;

    /**
     * @brief Gets the list of available AAA modules for this client
     * @return List of available modules
     * @throws ClientException if no modules available for client
     */
    const QList<QString>& getAAAModulesList();

    /**
     * @brief Authorize a client using a given AAAModule and conversation interface
     * @param[in] aaaModuleId id of module to use for authorization
     * @param[in] interface for conversation with client. This function does not
     * take the ownership of this object. You should delete it after return from this
     * function.
     * @return true if client has been successfully authorized with this module,
     * false otherwise
     * @throws ClientException if given module is unavailable for this client
     */
    bool authorize(const QString& aaaModuleId,
            AAA::ConversationInterface* interface);

    /**
     * @brief Check whether this client has been successfully authorized
     * @return true if client has been authorized, false otherwise
     */
    bool isAuthorized();

    /**
     * @brief Gets the data channel for this client
     * @return Data channel associated with this client
     * @note This object has an ownership of returned object so you should
     * not delete it.
     * @throws ClientException if client has not been authorized
     */
    DataChannel* getDataChannel();

    /**
     * @brief Exception class for client class
     */
    class ClientException : public std::runtime_error
    {
    public:
        /**
         * @brief Constructor
         * @param[in] arg exception string
         */
        explicit ClientException(const std::string& arg)
                : std::runtime_error(arg)
        {
        }
    };
private:
    /**
     * @brief Id of client
     */
    QString m_clientId;
    /**
     * @brief Provider id
     */
    QString m_providerId;
    /**
     * @brief List of AAAModules accessible for this client
     */
    QList<QString> m_aaaModules;
    /**
     * @brief Information if client has been already authorized
     */
    bool m_isAuthorized;
    /**
     * @brief Data channel to save received data.
     */
    boost::shared_ptr<DataChannel> m_dataChannel;
};

} //namespace Base
} //namespace INZ_project
#endif // !defined(EA_D6440D50_CCA5_46f1_8A41_6416AF2C70A2__INCLUDED_)
