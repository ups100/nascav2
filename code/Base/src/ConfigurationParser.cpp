/**
 * @file  ConfigurationParser.h
 * @brief  Implementation of the Class ConfigurationParser
 * @date  Created on:      13-sie-2013 17:38:10
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "ConfigurationParser.h"
#include "LogEntry.h"
#include "AAAFactory.h"
#include "DataProviderFactory.h"
#include "DataConsumerFactory.h"
#include <QHash>
#include <QtGlobal>

namespace INZ_project {
namespace Base {

QMutex ConfigurationParser::m_mutex;
ConfigurationParser* ConfigurationParser::m_instance = 0L;

ConfigurationParser::ConfigurationParser()
        : m_starString("*"), m_separator("::")
{
    m_tokenStrings["nsca_config"] = NSCA_CONFIG;
    m_tokenStrings["data_providers"] = DATA_PROVIDERS;
    m_tokenStrings["data_provider"] = DATA_PROVIDER;
    m_tokenStrings["additional_data"] = ADDITIONAL_DATA;
    m_tokenStrings["data_consumers"] = DATA_CONSUMERS;
    m_tokenStrings["data_consumer"] = DATA_CONSUMER;
    m_tokenStrings["clients"] = CLIENTS;
    m_tokenStrings["client"] = CLIENT;
    m_tokenStrings["aaa_data"] = AAA_DATA;
    m_tokenStrings["aaa_module"] = AAA_MODULE;
    m_tokenStrings["routes"] = ROUTES;
    m_tokenStrings["route"] = ROUTE;
}

ConfigurationParser::~ConfigurationParser()
{
    m_tokenStrings.clear();
    m_providerGroups.clear();
    m_consumerGroups.clear();
    m_clientGroups.clear();
}

ConfigurationParser* ConfigurationParser::getInstance()
{
    if (m_instance == 0L) {
        QMutexLocker locker(&m_mutex);
        if (m_instance == 0L) {
            m_instance = ConfigurationParser::getInstanceHelper();
        }
    }
    return m_instance;
}

ConfigurationParser *ConfigurationParser::getInstanceHelper()
{
    static ConfigurationParser instance;
    return &instance;
}

QSet<QString> ConfigurationParser::getDataConsumersNames()
{
    QSet<QString> ret;
    ConfigurationParser *parser = ConfigurationParser::getInstance();

    foreach(QString consumerGroup, parser->m_consumerGroups.keys()) {
        foreach(QString consumerMember, parser->m_consumerGroups[consumerGroup].keys()) {
            ret.insert(consumerGroup + parser->m_separator + consumerMember);
        }
    }

    return ret;
}

const QString& ConfigurationParser::getDataConsumerType(
        const QString& consumerName)
{
    ConfigurationParser *parser = ConfigurationParser::getInstance();

    QString consumerGroup, consumerMember;
    consumerGroup = parser->getGroup(consumerName);
    consumerMember = parser->getGroupMember(consumerName);

    if (!parser->m_consumerGroups.contains(consumerGroup)
            || !parser->m_consumerGroups[consumerGroup].contains(
                    consumerMember)) {
        throw ParserException("No such consumer.");
    }

    return parser->m_consumerGroups[consumerGroup][consumerMember]->m_type;
}

const QString& ConfigurationParser::getDataConsumerAdditionalData(
        const QString& consumerName)
{
    ConfigurationParser *parser = ConfigurationParser::getInstance();

    QString consumerGroup, consumerMember;
    consumerGroup = parser->getGroup(consumerName);
    consumerMember = parser->getGroupMember(consumerName);

    if (!parser->m_consumerGroups.contains(consumerGroup)
            || !parser->m_consumerGroups[consumerGroup].contains(
                    consumerMember)) {
        throw ParserException("No such consumer.");
    }

    return parser->m_consumerGroups[consumerGroup][consumerMember]
            ->m_additionalData;
}

QSet<QString> ConfigurationParser::getDataProvidersNames()
{
    QSet<QString> ret;

    ConfigurationParser *parser = ConfigurationParser::getInstance();

    foreach(QString providerGroup, parser->m_providerGroups.keys()) {
        foreach(QString providerMember, parser->m_providerGroups[providerGroup].keys()) {
            ret.insert(providerGroup + parser->m_separator + providerMember);
        }
    }

    return ret;
}

const QString& ConfigurationParser::getDataProviderType(
        const QString& providerName)
{
    ConfigurationParser *parser = ConfigurationParser::getInstance();

    QString providerGroup, providerMember;
    providerGroup = parser->getGroup(providerName);
    providerMember = parser->getGroupMember(providerName);

    if (!parser->m_providerGroups.contains(providerGroup)
            || !parser->m_providerGroups[providerGroup].contains(
                    providerMember)) {
        throw ParserException("No such provider.");
    }

    return parser->m_providerGroups[providerGroup][providerMember]->m_type;
}

const QString& ConfigurationParser::getDataProviderAdditionalData(
        const QString& providerName)
{
    ConfigurationParser *parser = ConfigurationParser::getInstance();

    QString providerGroup, providerMember;
    providerGroup = parser->getGroup(providerName);
    providerMember = parser->getGroupMember(providerName);

    if (!parser->m_providerGroups.contains(providerGroup)
            || !parser->m_providerGroups[providerGroup].contains(
                    providerMember)) {
        throw ParserException("No such provider.");
    }

    return parser->m_providerGroups[providerGroup][providerMember]
            ->m_additionalData;
}

QSet<QString> ConfigurationParser::getRoutes(const QString& client,
        const QString& providerName)
{
    ConfigurationParser *parser = ConfigurationParser::getInstance();
    QString fromGroup, fromMember, viaGroup, viaMember;

    fromGroup = parser->getGroup(client);
    fromMember = parser->getGroupMember(client);

    viaGroup = parser->getGroup(providerName);
    viaMember = parser->getGroupMember(providerName);

    if (!parser->m_clientGroups.contains(fromGroup)
            || !parser->m_clientGroups[fromGroup].contains(fromMember)) {
        throw ParserException("No such client.");
    }

    QSet<QString> consumersRaw;
    //let's check if there is a rule for this provider
    if (parser->m_clientGroups[fromGroup][fromMember]->m_routes.contains(
            providerName)) {
        consumersRaw.unite(
                parser->m_clientGroups[fromGroup][fromMember]->m_routes[providerName]);
    }

    //let's chceck if there is a rule for this provider group
    if (parser->m_clientGroups[fromGroup][fromMember]->m_routes.contains(
            viaGroup)) {
        consumersRaw.unite(
                parser->m_clientGroups[fromGroup][fromMember]->m_routes[viaGroup]);
    }

    //let's check if there is an all match rule for this client
    if (parser->m_clientGroups[fromGroup][fromMember]->m_routes.contains(
            parser->m_starString)) {
        consumersRaw.unite(
                parser->m_clientGroups[fromGroup][fromMember]->m_routes[parser
                        ->m_starString]);
    }

    QList<QString> rulesList = consumersRaw.toList();
    for (QList<QString>::iterator it = rulesList.begin(); it != rulesList.end();
            ) {
        if (*it == parser->m_starString) {
            //We need to add every one so let's do this
            //When this is clear we know that there will be no groups but only valid members
            consumersRaw.clear();
            foreach(QString consumerGroup, parser->m_consumerGroups.keys()) {
                foreach(QString consumer, parser->m_consumerGroups[consumerGroup].keys()) {
                    consumersRaw.insert(
                            consumerGroup + parser->m_separator + consumer);
                }
            }
            //There will be nothing more to add for sure
            break;
        } else {
            QString toGroup, toMember;
            //This functions should not throw an exception here because those strings
            //has been checked in checkConfiguration function
            toGroup = parser->getGroup(*it);
            toMember = parser->getGroupMember(*it);

            if (toMember == QString()) {
                //This is a group, not a single member so remove it from our set
                consumersRaw.remove(*it);
                //We have the whole group so we have to add her members
                foreach(QString consumer, parser->m_consumerGroups[toGroup].keys()) {
                    consumersRaw.insert(
                            toGroup + parser->m_separator + consumer);
                }
            }
        }
    } //for

    return consumersRaw;
}

QList<QString> ConfigurationParser::getAAAModulesForClient(
        const QString& client)
{
    ConfigurationParser *parser = ConfigurationParser::getInstance();
    QString clientGroup, clientMember;

    clientGroup = parser->getGroup(client);
    clientMember = parser->getGroupMember(client);

    if (!parser->m_clientGroups.contains(clientGroup)
            || !parser->m_clientGroups[clientGroup].contains(clientMember)) {
        throw ParserException("No such client.");
    }

    return parser->m_clientGroups[clientGroup][clientMember]->m_auth.keys();
}

const QString& ConfigurationParser::getAAAModuleData(const QString& client,
        const QString& aaaModule)
{
    ConfigurationParser *parser = ConfigurationParser::getInstance();
    QString clientGroup, clientMember;

    clientGroup = parser->getGroup(client);
    clientMember = parser->getGroupMember(client);

    if (!parser->m_clientGroups.contains(clientGroup)
            || !parser->m_clientGroups[clientGroup].contains(clientMember)) {
        throw ParserException("No such client.");
    }

    if (!parser->m_clientGroups[clientGroup][clientMember]->m_auth.contains(
            aaaModule)) {
        throw ParserException("AAAModule not allowed for this client");
    }

    return parser->m_clientGroups[clientGroup][clientMember]->m_auth[aaaModule];
}

bool ConfigurationParser::parseConfiguration(QIODevice *input)
{
    ConfigurationParser *parser = ConfigurationParser::getInstance();
    QXmlStreamReader stream(input);
    bool result = true;
    if (parser->parseXMLConf(stream)) {
        LOG_ENTRY(MyLogger::DEBUG, "Configuration file successfully parsed.");
        if (parser->checkProgramConfiguration()) {
            LOG_ENTRY(MyLogger::DEBUG, "Configuration check OK.");
        } else {
            LOG_ENTRY(MyLogger::ERROR, "Incorrect configuration parsed.");
            parser->m_providerGroups.clear();
            parser->m_consumerGroups.clear();
            parser->m_clientGroups.clear();
            result = false;
        }
    } else {
        LOG_ENTRY(MyLogger::ERROR, "Unable to parse given configuration file");
        parser->m_providerGroups.clear();
        parser->m_consumerGroups.clear();
        parser->m_clientGroups.clear();
        result = false;
    }
    return result;
}

ConfigurationParser::Token ConfigurationParser::getTokenType(
        const QStringRef& tokenString)
{
    QHash<QString, Token>::iterator it = m_tokenStrings.find(
            tokenString.toString());
    return it == m_tokenStrings.end() ? OTHER : *it;
}

bool ConfigurationParser::parseXMLConf(QXmlStreamReader &stream)
{
    try {
        while (!stream.atEnd() && !stream.hasError()) {
            QXmlStreamReader::TokenType token = stream.readNext();
            switch (token) {
                case QXmlStreamReader::StartDocument:
                    continue;

                case QXmlStreamReader::StartElement:
                    switch (getTokenType(stream.name())) {
                        case NSCA_CONFIG:
                            parseNscaConfig(stream);
                            break;
                        default:
                            LOG_ENTRY(MyLogger::ERROR,
                                    "Invalid token "<<stream.name().toString() <<" at: "<<stream.lineNumber());
                            throw ParserException("Invalid token ");
                    }
                    break;
            }

        } //while
    } catch (ParserException &e) {
        return false;
    }

    if (stream.hasError()) {
        LOG_ENTRY(MyLogger::ERROR,
                stream.errorString()<<" At: "<<stream.lineNumber());
        return false;
    } else {
        return true;
    }
}

void ConfigurationParser::parseNscaConfig(QXmlStreamReader &stream)
{
    while (!stream.atEnd()) {
        QXmlStreamReader::TokenType token = stream.readNext();
        switch (token) {
            case QXmlStreamReader::StartElement:
                switch (getTokenType(stream.name())) {
                    case DATA_PROVIDERS:
                        parseDataProviders(stream);
                        break;

                    case DATA_CONSUMERS:
                        parseDataConsumers(stream);
                        break;

                    case CLIENTS:
                        parseClients(stream);
                        break;

                    case ROUTES:
                        parseRoutes(stream);
                        break;
                    default:
                        LOG_ENTRY(MyLogger::ERROR,
                                "Invalid token "<<stream.name().toString() <<" at: "<<stream.lineNumber());
                        throw ParserException("Invalid token");
                }
                break;

            case QXmlStreamReader::EndElement:
                if (getTokenType(stream.name()) == NSCA_CONFIG) {
                    return;
                }
                break;
        }

    } //while
    if (stream.hasError()) {
        LOG_ENTRY(MyLogger::ERROR,
                stream.errorString()<<" At: "<<stream.lineNumber());
        throw ParserException(stream.errorString().toStdString());
    }
}

void ConfigurationParser::parseDataProviders(QXmlStreamReader &stream)
{
    QXmlStreamAttributes attributes = stream.attributes();
    QString groupName;
    if (attributes.hasAttribute("id")) {
        groupName = attributes.value("id").toString();
        if (!m_providerGroups.contains(groupName)) {
            m_providerGroups.insert(groupName,
                    QHash<QString, boost::shared_ptr<DataProvider> >());
        }
    }

    while (!stream.atEnd()) {
        QXmlStreamReader::TokenType token = stream.readNext();
        switch (token) {
            case QXmlStreamReader::StartElement:
                switch (getTokenType(stream.name())) {
                    case DATA_PROVIDER:
                        parseDataProvider(stream, groupName);
                        break;

                    default:
                        LOG_ENTRY(MyLogger::ERROR,
                                "Invalid token "<<stream.name().toString() <<" at: "<<stream.lineNumber());
                        throw ParserException("Invalid token");
                }
                break;

            case QXmlStreamReader::EndElement:
                if (getTokenType(stream.name()) == DATA_PROVIDERS) {
                    return;
                }
                break;
        }

    } //while
    if (stream.hasError()) {
        LOG_ENTRY(MyLogger::ERROR,
                stream.errorString()<<" At: "<<stream.lineNumber());
        throw ParserException(stream.errorString().toStdString());
    }
}

void ConfigurationParser::parseDataProvider(QXmlStreamReader &stream,
        const QString &groupName)
{
    QXmlStreamAttributes attributes = stream.attributes();

    QString id;
    boost::shared_ptr<DataProvider> provider = boost::shared_ptr<DataProvider>(
            new DataProvider());
    if (attributes.hasAttribute("id") && attributes.hasAttribute("type")) {
        id = attributes.value("id").toString();
        provider->m_type = attributes.value("type").toString();
    } else if (!attributes.hasAttribute("id")) {
        LOG_ENTRY(MyLogger::ERROR,
                "No id attribute in data provider. Line: "<<stream.lineNumber());
        throw ParserException("No id");
    } else {
        LOG_ENTRY(MyLogger::ERROR,
                "No type attribute in data provider. Line: "<<stream.lineNumber());
        throw ParserException("No type");
    }

    while (!stream.atEnd()) {
        QXmlStreamReader::TokenType token = stream.readNext();
        switch (token) {
            case QXmlStreamReader::StartElement:
                switch (getTokenType(stream.name())) {
                    case ADDITIONAL_DATA:
                        provider->m_additionalData = stream.readElementText(
                                QXmlStreamReader::IncludeChildElements);
                        break;

                    default:
                        LOG_ENTRY(MyLogger::ERROR,
                                "Invalid token "<<stream.name().toString() <<" at: "<<stream.lineNumber());
                        throw ParserException("Invalid token");
                }
                break;

            case QXmlStreamReader::EndElement:
                if (getTokenType(stream.name()) == DATA_PROVIDER) {
                    if (m_providerGroups[groupName].contains(id)) {
                        LOG_ENTRY(MyLogger::ERROR,
                                "Id: "<<id<<" already exist in group "<<groupName);
                        throw ParserException("Id already taken");
                    } else {
                        m_providerGroups[groupName][id] = provider;
                    }
                    return;
                }
                break;
        }

    } //while
    if (stream.hasError()) {
        LOG_ENTRY(MyLogger::ERROR,
                stream.errorString()<<" At: "<<stream.lineNumber());
        throw ParserException(stream.errorString().toStdString());
    }
}

void ConfigurationParser::parseDataConsumers(QXmlStreamReader &stream)
{
    QXmlStreamAttributes attributes = stream.attributes();
    QString groupName;
    if (attributes.hasAttribute("id")) {
        groupName = attributes.value("id").toString();
        if (!m_consumerGroups.contains(groupName)) {
            m_consumerGroups.insert(groupName,
                    QHash<QString, boost::shared_ptr<DataConsumer> >());
        }
    }

    while (!stream.atEnd()) {
        QXmlStreamReader::TokenType token = stream.readNext();
        switch (token) {
            case QXmlStreamReader::StartElement:
                switch (getTokenType(stream.name())) {
                    case DATA_CONSUMER:
                        parseDataConsumer(stream, groupName);
                        break;

                    default:
                        LOG_ENTRY(MyLogger::ERROR,
                                "Invalid token "<<stream.name().toString() <<" at: "<<stream.lineNumber());
                        throw ParserException("Invalid token");
                }
                break;

            case QXmlStreamReader::EndElement:
                if (getTokenType(stream.name()) == DATA_CONSUMERS) {
                    return;
                }
                break;
        }

    } //while
    if (stream.hasError()) {
        LOG_ENTRY(MyLogger::ERROR,
                stream.errorString()<<" At: "<<stream.lineNumber());
        throw ParserException(stream.errorString().toStdString());
    }
}

void ConfigurationParser::parseDataConsumer(QXmlStreamReader &stream,
        const QString &groupName)
{
    QXmlStreamAttributes attributes = stream.attributes();

    QString id;
    boost::shared_ptr<DataConsumer> consumer = boost::shared_ptr<DataConsumer>(
            new DataConsumer());
    if (attributes.hasAttribute("id") && attributes.hasAttribute("type")) {
        id = attributes.value("id").toString();
        consumer->m_type = attributes.value("type").toString();
    } else if (!attributes.hasAttribute("id")) {
        LOG_ENTRY(MyLogger::ERROR,
                "No id attribute in data consumer. Line: "<<stream.lineNumber());
        throw ParserException("No id");
    } else {
        LOG_ENTRY(MyLogger::ERROR,
                "No type attribute in data consumer. Line: "<<stream.lineNumber());
        throw ParserException("No type");
    }

    while (!stream.atEnd()) {
        QXmlStreamReader::TokenType token = stream.readNext();
        switch (token) {
            case QXmlStreamReader::StartElement:
                switch (getTokenType(stream.name())) {
                    case ADDITIONAL_DATA:
                        consumer->m_additionalData = stream.readElementText(
                                QXmlStreamReader::IncludeChildElements);
                        break;

                    default:
                        LOG_ENTRY(MyLogger::ERROR,
                                "Invalid token "<<stream.name().toString() <<" at: "<<stream.lineNumber());
                        throw ParserException("Invalid token");
                }
                break;

            case QXmlStreamReader::EndElement:
                if (getTokenType(stream.name()) == DATA_CONSUMER) {

                    if (m_consumerGroups[groupName].contains(id)) {
                        LOG_ENTRY(MyLogger::ERROR,
                                "Id: "<<id<<" already exist in group "<<groupName);
                        throw ParserException("Id already taken");
                    } else {
                        m_consumerGroups[groupName][id] = consumer;
                    }
                    return;
                }
                break;
        }

    } //while
    if (stream.hasError()) {
        LOG_ENTRY(MyLogger::ERROR,
                stream.errorString()<<" At: "<<stream.lineNumber());
        throw ParserException(stream.errorString().toStdString());
    }
}

void ConfigurationParser::parseClients(QXmlStreamReader &stream)
{
    QXmlStreamAttributes attributes = stream.attributes();
    QString groupName;
    if (attributes.hasAttribute("id")) {
        groupName = attributes.value("id").toString();
        if (!m_clientGroups.contains(groupName)) {
            m_clientGroups.insert(groupName,
                    QHash<QString, boost::shared_ptr<Client> >());
        }
    }

    while (!stream.atEnd()) {
        QXmlStreamReader::TokenType token = stream.readNext();
        switch (token) {
            case QXmlStreamReader::StartElement:
                switch (getTokenType(stream.name())) {
                    case CLIENT:
                        parseClient(stream, groupName);
                        break;

                    default:
                        LOG_ENTRY(MyLogger::ERROR,
                                "Invalid token "<<stream.name().toString() <<" at: "<<stream.lineNumber());
                        throw ParserException("Invalid token");
                }
                break;

            case QXmlStreamReader::EndElement:
                if (getTokenType(stream.name()) == CLIENTS) {
                    return;
                }
                break;
        }

    } //while
    if (stream.hasError()) {
        LOG_ENTRY(MyLogger::ERROR,
                stream.errorString()<<" At: "<<stream.lineNumber());
        throw ParserException(stream.errorString().toStdString());
    }
}

void ConfigurationParser::parseClient(QXmlStreamReader &stream,
        const QString &groupName)
{
    QXmlStreamAttributes attributes = stream.attributes();

    QString id;
    if (attributes.hasAttribute("id")) {
        id = attributes.value("id").toString();
        if (!m_clientGroups[groupName].contains(id)) {
            m_clientGroups[groupName][id] = boost::shared_ptr<Client>(
                    new Client());
        } else {
            LOG_ENTRY(MyLogger::ERROR,
                    "Id: "<<id<<" already exist in group "<<groupName);
            throw ParserException("Id already taken");
        }
    } else {
        LOG_ENTRY(MyLogger::ERROR,
                "No id attribute in client. Line: "<<stream.lineNumber());
        throw ParserException("No id");
    }

    while (!stream.atEnd()) {
        QXmlStreamReader::TokenType token = stream.readNext();
        switch (token) {
            case QXmlStreamReader::StartElement:
                switch (getTokenType(stream.name())) {
                    case AAA_DATA:
                        parseAAAData(stream, groupName, id);
                        break;

                    default:
                        LOG_ENTRY(MyLogger::ERROR,
                                "Invalid token "<<stream.name().toString() <<" at: "<<stream.lineNumber());
                        throw ParserException("Invalid token");
                }
                break;

            case QXmlStreamReader::EndElement:
                if (getTokenType(stream.name()) == CLIENT) {
                    return;
                }
                break;
        }

    } //while
    if (stream.hasError()) {
        LOG_ENTRY(MyLogger::ERROR,
                stream.errorString()<<" At: "<<stream.lineNumber());
        throw ParserException(stream.errorString().toStdString());
    }
}

void ConfigurationParser::parseAAAData(QXmlStreamReader &stream,
        const QString &groupName, const QString &clientName)
{
    while (!stream.atEnd()) {
        QXmlStreamReader::TokenType token = stream.readNext();
        QXmlStreamAttributes attributes;
        switch (token) {
            case QXmlStreamReader::StartElement:
                switch (getTokenType(stream.name())) {
                    case AAA_MODULE:
                        attributes = stream.attributes();
                        if (attributes.hasAttribute("type")) {
                            QString type = attributes.value("type").toString();
                            if (!m_clientGroups[groupName][clientName]->m_auth
                                    .contains(type)) {
                                m_clientGroups[groupName][clientName]->m_auth[type] =
                                        stream.readElementText(
                                                QXmlStreamReader::IncludeChildElements);
                            } else {
                                LOG_ENTRY(MyLogger::ERROR,
                                        "Type: "<<type<<" already exist in group "<<groupName <<" client: "<<clientName);
                                throw ParserException(
                                        "Type for this client already taken");
                            }
                        } else {
                            LOG_ENTRY(MyLogger::ERROR,
                                    "No type attribute in aaa module. Line: "<<stream.lineNumber());
                            throw ParserException("No type");
                        }
                        break;

                    default:
                        LOG_ENTRY(MyLogger::ERROR,
                                "Invalid token "<<stream.name().toString() <<" at: "<<stream.lineNumber());
                        throw ParserException("Invalid token");
                }
                break;

            case QXmlStreamReader::EndElement:
                if (getTokenType(stream.name()) == AAA_DATA) {
                    return;
                }
                break;
        }

    } //while
    if (stream.hasError()) {
        LOG_ENTRY(MyLogger::ERROR,
                stream.errorString()<<" At: "<<stream.lineNumber());
        throw ParserException(stream.errorString().toStdString());
    }
}

void ConfigurationParser::parseRoutes(QXmlStreamReader &stream)
{
    while (!stream.atEnd()) {
        QXmlStreamReader::TokenType token = stream.readNext();
        switch (token) {
            case QXmlStreamReader::StartElement:
                switch (getTokenType(stream.name())) {
                    case ROUTE:
                        parseRoute(stream);
                        break;

                    default:
                        LOG_ENTRY(MyLogger::ERROR,
                                "Invalid token "<<stream.name().toString() <<" at: "<<stream.lineNumber());
                        throw ParserException("Invalid token");
                }
                break;

            case QXmlStreamReader::EndElement:
                if (getTokenType(stream.name()) == ROUTES) {
                    return;
                }
                break;
        }

    } //while
    if (stream.hasError()) {
        LOG_ENTRY(MyLogger::ERROR,
                stream.errorString()<<" At: "<<stream.lineNumber());
        throw ParserException(stream.errorString().toStdString());
    }
}

void ConfigurationParser::parseRoute(QXmlStreamReader &stream)
{
    QXmlStreamAttributes attributes = stream.attributes();

    QString client;
    if (attributes.hasAttribute("client")) {
        client = attributes.value("client").toString();
    } else {
        LOG_ENTRY(MyLogger::ERROR,
                "No client attribute in route. Line: "<<stream.lineNumber());
        throw ParserException("No client in route");
    }

    QString via;
    if (attributes.hasAttribute("via")) {
        via = attributes.value("via").toString();
    } else {
        LOG_ENTRY(MyLogger::ERROR,
                "No via attribute in route. Line: "<<stream.lineNumber());
        throw ParserException("No via in route");
    }

    QString to;
    if (attributes.hasAttribute("to")) {
        to = attributes.value("to").toString();
    } else {
        LOG_ENTRY(MyLogger::ERROR,
                "No to attribute in route. Line: "<<stream.lineNumber());
        throw ParserException("No to in route");
    }

    submitRoute(client, via, to);

    while (!stream.atEnd()) {
        QXmlStreamReader::TokenType token = stream.readNext();
        switch (token) {
            case QXmlStreamReader::StartElement:
                switch (getTokenType(stream.name())) {
                    default:
                        LOG_ENTRY(MyLogger::ERROR,
                                "Invalid token "<<stream.name().toString() <<" at: "<<stream.lineNumber());
                        throw ParserException("Invalid token");
                }
                break;

            case QXmlStreamReader::EndElement:
                if (getTokenType(stream.name()) == ROUTES) {
                    return;
                }
                break;
        }

    } //while
    if (stream.hasError()) {
        LOG_ENTRY(MyLogger::ERROR,
                stream.errorString()<<" At: "<<stream.lineNumber());
        throw ParserException(stream.errorString().toStdString());
    }
}

void ConfigurationParser::submitRoute(const QString& from, const QString& via,
        const QString& to)
{
    QString fromGroup, fromMember;

    try {
        fromGroup = this->getGroup(from);
        fromMember = this->getGroupMember(from);
    } catch (ParserException& e) {
        LOG_ENTRY(MyLogger::WARN,
                e.what()<<" In client string. Skipping route " <<from<<" "<<via<<" "<<to<<".");
        return;
    }

    if (fromGroup == m_starString) {
        //this is route for all clients
        QList<QString> keys = m_clientGroups.keys();
        foreach(QString key, keys) {
            foreach(boost::shared_ptr<Client> client, m_clientGroups[key]) {
                client->m_routes[via].insert(to);
            };
        }
    } else if (!m_clientGroups.contains(fromGroup)) {
        LOG_ENTRY(MyLogger::WARN,
                "No such client group "<<fromGroup<< " skipping route:"<<from<<" "<<via<<" "<<to<<".");
        return;
    } else if (fromMember == QString()) {
        //we have a whole clients group

        foreach(boost::shared_ptr<Client> client, m_clientGroups[fromGroup]) {
            client->m_routes[via].insert(to);
            ;
        }

    } else {
        //we have a single client
        if (!m_clientGroups[fromGroup].contains(fromMember)) {
            LOG_ENTRY(MyLogger::WARN,
                    "No such client "<<fromMember<< " skipping route:"<<from<<" "<<via<<" "<<to<<".");
            return;
        }

        m_clientGroups[fromGroup][fromMember]->m_routes[via].insert(to);
    }
}

bool ConfigurationParser::checkProgramConfiguration()
{
    //Checks if all data providers has accessible types
    QSet<QString> providerTypes;
    QList<QString> providerGroups = m_providerGroups.keys();
    foreach(QString group, providerGroups) {
        QList<QString> providers = m_providerGroups.keys();
        foreach(QString provider, providers) {
            providerTypes.insert(m_providerGroups[group][provider]->m_type);
        }

    }

    QSet<QString> availableProviders = QSet<QString>::fromList(
            DataProviderFactory::getDataProviderList());
    providerTypes.subtract(availableProviders);

    //The set should be empty
    if (!providerTypes.empty()) {
        foreach(QString type, providerTypes.toList()) {
            LOG_ENTRY(MyLogger::WARN, "No such Data Provider type: "<<type);
        }
        return false;
    }

    //Checks if all data consumers has accessible types
    QSet<QString> consumerTypes;
    QList<QString> consumerGroups = m_consumerGroups.keys();
    foreach(QString group, consumerGroups) {
        QList<QString> consumers = m_consumerGroups.keys();
        foreach(QString consumer, consumers) {
            consumerTypes.insert(m_consumerGroups[group][consumer]->m_type);
        }

    }

    QSet<QString> availableConsumers = QSet<QString>::fromList(
            DataConsumerFactory::getDataConsumerList());
    consumerTypes.subtract(availableConsumers);

    //The set should be empty
    if (!consumerTypes.empty()) {
        foreach(QString type, consumerTypes.toList()) {
            LOG_ENTRY(MyLogger::WARN, "No such Data Consumer type: "<<type);
        }
        return false;
    }

    //Checks if all roads has valid start and end. And if all AAAModules names are valid
    QList<QString> aaaModules = AAA::AAAFactory::getAAAModulesList();
    QList<QString> groups = m_clientGroups.keys();
    foreach(QString group, groups) {
        QList<QString> clients = m_clientGroups[group].keys();

        foreach(QString clientStr, clients) {
            boost::shared_ptr<Client> client = m_clientGroups[group][clientStr];

            foreach(QString aaaModule, client->m_auth.keys()) {
                if (!aaaModules.contains(aaaModule)) {
                    LOG_ENTRY(MyLogger::WARN,
                            "No such AAAModule accessible: "<<aaaModule);
                    return false;
                }
            }

            QList<QString> providers = client->m_routes.keys();

            //when there is no route for this client it's acceptable but it will be
            //better to warn a user
            if (providers.empty()) {
                LOG_ENTRY(MyLogger::WARN,
                        "Client: "<<group<<m_separator<<clientStr <<" has no route.");
            }

            foreach(QString provider, providers) {
                QString fromGroup, fromMember;

                try {
                    fromGroup = this->getGroup(provider);
                    fromMember = this->getGroupMember(provider);
                } catch (ParserException& e) {
                    LOG_ENTRY(MyLogger::WARN,
                            e.what()<<" In provider string: "<<provider);
                    return false;
                }

                if (fromGroup != m_starString) {
                    //we know that this provider should be on the list
                    //check if this is a group or single client
                    if (!m_providerGroups.contains(fromGroup)) {
                        LOG_ENTRY(MyLogger::WARN,
                                "No such provider group: "<<fromGroup);
                        return false;
                    } else if (fromMember != QString()) {
                        //we have a single provider
                        if (!m_providerGroups[fromGroup].contains(fromMember)) {
                            LOG_ENTRY(MyLogger::WARN,
                                    "No such provider: "<<fromMember<<" in group: "<<fromGroup);
                            return false;
                        }
                    }
                }

                //when there is no end for this route it's an error it shouldn't happen
                if (client->m_routes[provider].empty()) {
                    LOG_ENTRY(MyLogger::ERROR,
                            "Route via "<<provider<<" from client "<<clientStr<<" has no end.");
                    return false;
                }

                foreach(QString consumer,client->m_routes[provider]) {
                    QString toGroup, toMember;

                    try {
                        toGroup = this->getGroup(consumer);
                        toMember = this->getGroupMember(consumer);
                    } catch (ParserException& e) {
                        LOG_ENTRY(MyLogger::WARN,
                                e.what()<<" In consumer string:"<<consumer);
                        return false;
                    }

                    if (fromGroup != m_starString) {
                        //we know that this provider should be on the list
                        //check if this is a group or single client
                        if (!m_consumerGroups.contains(toGroup)) {
                            LOG_ENTRY(MyLogger::WARN,
                                    "No such consumer group: "<<toGroup);
                            return false;
                        } else if (toMember != QString()) {
                            //we have a single provider
                            if (!m_consumerGroups[toGroup].contains(toMember)) {
                                LOG_ENTRY(MyLogger::WARN,
                                        "No such consumer: "<<toGroup<<" in group: "<<toMember);
                                return false;
                            }
                        }
                    }
                } //foreach consumer
            } //foreach provider
        } // foreach client
    } // foreach client group

    return true;
}

QString ConfigurationParser::getGroup(const QString& fullName)
{
    QString ret;

    if (fullName == QString()) {
        throw ParserException("Name should not be empty");
    }

    if (fullName == m_starString) {
        ret == fullName;
    } else if (!fullName.contains(m_separator)) {
        ret == fullName;
    } else {
        int index = fullName.indexOf(m_separator, 0);
        if (index + m_separator.length() == fullName.length()) {
            throw ParserException("Group member can not be empty");
        }

        if (fullName.indexOf(m_separator, index + m_separator.length()) != -1) {
            throw ParserException("Additional separate character in string.");
        }

        ret = fullName.left(index);
    }

    return ret;
}

QString ConfigurationParser::getGroupMember(const QString& fullName)
{
    QString ret;

    if (fullName == QString()) {
        throw ParserException("Name should not be empty");
    }

    if (fullName == m_starString) {
        ret == fullName;
    } else if (!fullName.contains(m_separator)) {
        ret == QString();
    } else {
        int index = fullName.indexOf(m_separator, 0);
        if (index + m_separator.length() == fullName.length()) {
            throw ParserException("Group member can not be empty");
        }

        if (fullName.indexOf(m_separator, index + m_separator.length()) != -1) {
            throw ParserException("Additional separate character in string.");
        }

        ret = fullName.right(fullName.length() - index - m_separator.length());
    }

    return ret;
}

} //namespace Base
} //namespace INZ_project
