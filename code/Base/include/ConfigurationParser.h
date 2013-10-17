/**
 * @file  ConfigurationParser.h
 * @brief  Definition of the Class ConfigurationParser
 * @date  13-sie-2013 17:38:10
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_E9E17D99_BB22_452f_BD9C_2B30B7D935A1__INCLUDED_)
#define EA_E9E17D99_BB22_452f_BD9C_2B30B7D935A1__INCLUDED_

#include <QList>
#include <QString>
#include <stdexcept>
#include <QXmlStreamReader>
#include <QHash>
#include <QMap>
#include <QSet>
#include <QMutex>
#include <QMutexLocker>
#include <boost/shared_ptr.hpp>
#include <string>

namespace INZ_project {
namespace Base {
class ConfigurationParser
{

public:
    /**
     * @brief Destructor
     */
    virtual ~ConfigurationParser();

    /**
     * @brief Reads the configuration details from input
     * @param[in] input with configuration
     * @return true if file parsed successfully,
     * false and FATAL log otherwise
     */
    static bool parseConfiguration(QIODevice *input);

    /**
     * @brief Gets the data consumer names parsed from configuration input
     * @details This function should be called only after parseConfguration()
     * Successful call.
     * @return List of unique names for data consumers
     * @see parseConfguration()
     */
    static QSet<QString> getDataConsumersNames();

    /**
     * @brief Gets the register type of data consumer
     * @details This function should be called only after parseConfguration()
     * Successful call.
     * @param[in] consumerName identity obtained from getDataConsumersNames()
     * @return Consumer type identification
     * @see parseConfguration()
     */
    static const QString& getDataConsumerType(const QString& consumerName);

    /**
     * @brief Gets the additional data of data consumer
     * @details This function should be called only after parseConfguration()
     * Successful call.
     * @param[in] consumerName identity obtained from getDataConsumersNames()
     * @return Additional data for this data consumer from configuration file
     * @throw ParserError if invalid parameter strings has been provided
     * @see parseConfguration()
     */
    static const QString& getDataConsumerAdditionalData(
            const QString& consumerName);

    /**
     * @brief Gets the data providers names parsed from configuration input
     * @details This function should be called only after parseConfguration()
     * Successful call.
     * @return List of unique names for data providers
     * @see parseConfguration()
     */
    static QSet<QString> getDataProvidersNames();

    /**
     * @brief Gets the register type of data consumer
     * @details This function should be called only after parseConfguration()
     * Successful call.
     * @param[in] providerName identity obtained from getDataProvidersNames()
     * @return Provider type identification
     * @throw ParserError if invalid parameter strings has been provided
     * @see parseConfguration()
     */
    static const QString& getDataProviderType(const QString& providerName);

    /**
     * @brief Gets the additional data of data provider
     * @details This function should be called only after parseConfguration()
     * Successful call.
     * @param[in] providerName identity obtained from getDataProvidersNames()
     * @return Additional data for this data consumer from configuration file
     * @throw ParserError if invalid parameter strings has been provided
     * @see parseConfguration()
     */
    static const QString& getDataProviderAdditionalData(
            const QString& providerName);

    /**
     * @brief Gets the destinations for logs form specified client via specified data
     * provider
     * @details This function should be called only after parseConfguration()
     * Successful call.
     * @throw ParserError if invalid parameter strings has been provided
     */
    static QSet<QString> getRoutes(const QString& client,
            const QString& providerName);

    /**
     * @brief Gets the list of AAAModules accessible for this client
     * @param[in] client id of client
     * @return list of allowed AAAModules
     * @throws ParserError if invalid parameter strings has been provided
     */
    static QList<QString> getAAAModulesForClient(const QString& client);

    /**
     * @brief Gets the additional data for given client and AAAModule
     * @param[in] client id of client
     * @param[in] aaaModule id of AAA modules
     * @return list of allowed AAAModules
     * @throws ParserError if invalid parameter strings has been provided
     */
    static const QString& getAAAModuleData(const QString& client,
            const QString& aaaModule);

    /**
     * @brief Gets the instance of this object.
     * @return Instance of this object
     */
    static ConfigurationParser* getInstance();

    /**
     * @brief Exception class for errors occurred while parsing
     */
    class ParserException : public std::runtime_error
    {
    public:
        /**
         * @brief Constructor
         * @param[in] arg exception string
         */
        explicit ParserException(const std::string& arg)
                : std::runtime_error(arg)
        {
        }
    };

private:

    /**
     * @brief Enumeration for acceptable token types in config file
     */
    enum Token
    {
        NSCA_CONFIG = 0,
        DATA_PROVIDERS,
        DATA_PROVIDER,
        ADDITIONAL_DATA,
        DATA_CONSUMERS,
        DATA_CONSUMER,
        CLIENTS,
        CLIENT,
        AAA_DATA,
        AAA_MODULE,
        ROUTES,
        ROUTE,
        OTHER
    };

    /**
     * @brief Internal structure which represents data provider
     */
    struct DataProvider
    {
        /**
         * @brief Type of this data provider
         */
        QString m_type;
        /**
         * @brief Additional data from config file
         */
        QString m_additionalData;
    };

    /**
     * @brief Internal structure which represents data consumer
     */
    struct DataConsumer
    {
        /**
         * @brief Type of this data consumer
         QString m_type;
         */
        QString m_type;
        /**
         * @brief Additional data from config file
         */
        QString m_additionalData;
    };

    /**
     * @brief Internal structure which represents client
     */
    struct Client
    {
        /**
         * @brief Allowed AAAModules and their additional data
         */
        QMap<QString, QString> m_auth;
        /**
         * @brief Data providers to which client is allowed to connect
         */
        QMap<QString, QSet<QString> > m_routes;
    };

    /**
     * @brief Constructor
     */
    ConfigurationParser();

    /**
     * @brief Helper function to provide class instance
     */
    static ConfigurationParser *getInstanceHelper();

    /**
     * @brief Mutex for instance getter synchronization
     */
    static QMutex m_mutex;

    /**
     * @brief Mutex for data consumers collection
     */
    QMutex m_documentMutex;

    /**
     * @brief Instance of this class.
     */
    static ConfigurationParser* m_instance;

    /**
     * @brief Parses the XML configuration
     * @details Configuration is saved in this class data structures
     * @param[in] stream with xml configuration
     * @return true if provided configuration has been successfully parsed,
     * false otherwise
     */
    bool parseXMLConf(QXmlStreamReader &stream);

    /**
     * @brief Parses the nsca config section
     * @details Configuration is saved in this class data structures
     * @param[in] stream with xml configuration
     * @throw ParserException if document format is wrong
     */
    void parseNscaConfig(QXmlStreamReader &stream);

    /**
     * @brief Parses the data providers section
     * @details Configuration is saved in this class data structures
     * @param[in] stream with xml configuration
     * @throw ParserException if document format is wrong
     */
    void parseDataProviders(QXmlStreamReader &stream);

    /**
     * @brief Parses the data provider section
     * @details Configuration is saved in this class data structures
     * @param[in] stream with xml configuration
     * @param[in] groupName group to which this data provider is assigned
     * @throw ParserException if document format is wrong
     */
    void parseDataProvider(QXmlStreamReader &stream, const QString &groupName);

    /**
     * @brief Parses the data consumers section
     * @details Configuration is saved in this class data structures
     * @param[in] stream with xml configuration
     * @throw ParserException if document format is wrong
     */
    void parseDataConsumers(QXmlStreamReader &stream);

    /**
     * @brief Parses the data consumer section
     * @details Configuration is saved in this class data structures
     * @param[in] stream with xml configuration
     * @param[in] groupName group to which this data consumer is assigned
     * @throw ParserException if document format is wrong
     */
    void parseDataConsumer(QXmlStreamReader &stream, const QString &groupName);

    /**
     * @brief Parses the data clients section
     * @details Configuration is saved in this class data structures
     * @param[in] stream with xml configuration
     * @throw ParserException if document format is wrong
     */
    void parseClients(QXmlStreamReader &stream);

    /**
     * @brief Parses the client section
     * @details Configuration is saved in this class data structures
     * @param[in] stream with xml configuration
     * @param[in] groupName group to clientName is assigned
     * @param[in] clientName name of client whose this data is
     * @throw ParserException if document format is wrong
     */
    void parseAAAData(QXmlStreamReader &stream, const QString &groupName,
            const QString &clientName);

    /**
     * @brief Parses the AAA data section
     * @details Configuration is saved in this class data structures
     * @param[in] stream with xml configuration
     * @param[in] groupName group to which this client is assigned
     * @throw ParserException if document format is wrong
     */
    void parseClient(QXmlStreamReader &stream, const QString &groupName);

    /**
     * @brief Parses the data providers section
     * @details Configuration is saved in this class data structures
     * @param[in] stream with xml configuration
     * @throw ParserException if document format is wrong
     */
    void parseRoutes(QXmlStreamReader &stream);

    /**
     * @brief Parses the data providers section
     * @details Configuration is saved in this class data structures
     * @param[in] stream with xml configuration
     * @throw ParserException if document format is wrong
     */
    void parseRoute(QXmlStreamReader &stream);

    /**
     * @brief Submits a route.
     * @details If some error occurred this function creates a WARN level log
     *  and does nothing
     * @param[in] from client or client group who may use this road
     * @param[in] via data provider or data providers group which this road
     * consist of
     * @param[in] to data consumer or data consumer group which this road
     * consist of
     */
    void submitRoute(const QString& from, const QString& via,
            const QString& to);

    /**
     * @brief Checks whether configuration read from config file is correct
     * @return true if OK, false otherwise
     */
    bool checkProgramConfiguration();

    /**
     * @brief Solves the token type bese on string
     * @param[in] tokenString String value of token to be analyzed
     * @return type of this token
     */
    Token getTokenType(const QStringRef& tokenString);

    /**
     * @brief Separates group name from string
     * @param[in] fullName String should be formated [group name][separator][member name]
     * or [group name]
     * @return #m_starString if fullName is equal to it, group name otherwise
     * @throw ParserError if fullName is empty, group name is empty or string contains
     * more than one separator
     * @see m_separator
     * @see m_starString
     */
    QString getGroup(const QString& fullName);

    /**
     * @brief Separates group member name from string
     * @param[in] fullName String should be formated [group name][separator][member name]
     * or [group name]
     * @return m_starString if fullName is equal to it, empty string if mamber name was not specified
     *  group member name otherwise
     * @throw ParserError if fullName is empty, group member name is empty or string contains
     * more than one separator
     * @see m_separator
     * @see m_starString
     */
    QString getGroupMember(const QString& fullName);

    /**
     * @brief Field which contains a string which means that all values are acceptable
     */
    QString m_starString;

    /**
     * @brief Special string which separates group and member name
     */
    QString m_separator;

    /**
     * @brief String values of tokens
     */
    QHash<QString, Token> m_tokenStrings;

    /**
     * @brief Data providers information read from config
     */
    QHash<QString, QHash<QString, boost::shared_ptr<DataProvider> > > m_providerGroups;

    /**
     * @brief Data consumers information read from config
     */
    QHash<QString, QHash<QString, boost::shared_ptr<DataConsumer> > > m_consumerGroups;

    /**
     * @brief Clients and routes information read from config
     */
    QHash<QString, QHash<QString, boost::shared_ptr<Client> > > m_clientGroups;

};

} //namespace Base
} //namespace INZ_project
#endif // !defined(EA_E9E17D99_BB22_452f_BD9C_2B30B7D935A1__INCLUDED_)
