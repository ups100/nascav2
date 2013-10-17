/**
 * @file  Client.h
 * @brief  Implementation of the Class ClientSession
 * @date  17-10-2013 17:38:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "Client.h"
#include "DataChannel.h"
#include "LogEntry.h"
#include "DataProvider.h"
#include "ConfigurationParser.h"
#include "MyNscaMain.h"

namespace INZ_project {
namespace Base {

ClientSession::ClientSession(const QString& clientId,
        const DataProvider* provider)
        : m_clientId(clientId), m_providerId(provider->getProviderId()),
                m_isAuthorized(false), m_dataChannel(static_cast<DataChannel*>(0L))
{
    try {
        m_aaaModules = ConfigurationParser::getAAAModulesForClient(clientId);
    } catch (const ConfigurationParser::ParserException& e) {
        LOG_ENTRY(MyLogger::DEBUG, e.what()<<" While id = "<<clientId);
        throw ClientException(e.what());
    }
}

ClientSession::~ClientSession()
{

}

const QList<QString>& ClientSession::getAAAModulesList()
{
    if (m_aaaModules.isEmpty()) {
        throw ClientException("No AAAModules allowed for this client");
    }

    return m_aaaModules;
}

bool ClientSession::authorize(const QString& aaaModuleId,
        AAA::ConversationInterface* interface)
{
    if (!m_aaaModules.contains(aaaModuleId)) {
        LOG_ENTRY(MyLogger::DEBUG,
                "Trying to authorize client "<<m_clientId <<" using unsupported AAAModule: "<<aaaModuleId);
        throw ClientException("AAAModule not allowed for this client");
    }

    boost::shared_ptr<AAA::AAAModule> module = boost::shared_ptr<AAA::AAAModule>(AAA::AAAFactory::getAAAModule(
            aaaModuleId));

    if (module == 0L) {
        LOG_ENTRY(MyLogger::DEBUG,
                "Trying to authorize client "<<m_clientId <<" using unavailable AAAModule: "<<aaaModuleId);
        throw ClientException("AAAModule unavailable");
    }

    //Let's set the method for conversation
    module->setConversationInterface(interface);

    try {
        //set the additional data about this client
        module->setAdditionalData(
                ConfigurationParser::getAAAModuleData(m_clientId, aaaModuleId));
        //and let's try to authorize
        m_isAuthorized = module->run();

        //if authorization is successful client will send some data so let's
        //provide data channel for this client
        if (m_isAuthorized) {
            m_dataChannel = boost::shared_ptr<DataChannel>(MyNscaMain::provideDataChannel(m_clientId,
                    m_providerId));
        }
    } catch (const ConfigurationParser::ParserException& e) {
        LOG_ENTRY(MyLogger::FATAL,
                "Unable to get additional data of "<<aaaModuleId<<"for "<<m_clientId);
        throw ClientException("Unable to get additional data.");
    } catch (const AAA::AAAModule::AdditionalDataException& e) {
        LOG_ENTRY(MyLogger::FATAL,
                "Unable to parse data for "<<aaaModuleId<<" client: "<<m_clientId);
        throw ClientException("Unable to parse additional data.");
    } catch (const std::runtime_error& e) {
        //some error while trying to authorize client
        LOG_ENTRY(MyLogger::DEBUG,
                e.what()<< "While trying to authorize using "<<aaaModuleId<<" client: "<<m_clientId);
        m_isAuthorized = false;
    } catch (...) {
        //unknown error while trying to authorize client
        LOG_ENTRY(MyLogger::DEBUG,
                "Unknown error while trying to authorize using "<<aaaModuleId<<" client: "<<m_clientId);
        m_isAuthorized = false;
    }

    return m_isAuthorized;
}

bool ClientSession::isAuthorized()
{
    return m_isAuthorized;
}

const QString& ClientSession::getClientId() const
{

    return m_clientId;
}

DataChannel* ClientSession::getDataChannel()
{
    if(!m_isAuthorized) {
        throw ClientException("Client not authorized");
    }

    return m_dataChannel.get();
}

} //namespace Base
} //namespace INZ_project
