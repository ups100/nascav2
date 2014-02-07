/**
 * @file  MyNscaMain.cpp
 * @brief  Implementation of the Class INZ_project::Base::MyNascaMain
 * @date  20-09-2013 17:38:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "MyNscaMain.h"
#include "LogEntry.h"
#include "DataConsumerFactory.h"
#include "DataProviderFactory.h"
#include "DataFile.h"
#include <string>
#include <iostream>
#include <QFile>
#include <QDir>
#include <QTimer>

#include <sys/types.h>
#include <sys/stat.h>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/parsers.hpp>

using namespace boost::program_options;

namespace INZ_project {
namespace Base {

MyNscaMain* MyNscaMain::m_instance = 0L;

MyNscaMain::MyNscaMain(int argc, char **argv)
        : m_app(argc, argv), m_confPath("/usr/local/nscav2/etc/config.xml"),
                m_bufPath("/usr/local/nscav2/var/buf"),
                m_maxBufSize(1024 * 100),
                m_desc(
                        "Author: Krzysztof Opasiak <ups100@tlen.pl>\n Allowed options")
{
    if (m_instance == 0L) {
        m_instance = this;
    } else {
        LOG_ENTRY(MyLogger::FATAL,
                "Another instance of MyNscaMain has been created!");
        throw MyNscaMainException("Another instance is alive");
    }

    m_desc.add_options()("help", "Prints this help message")("check",
            "Only check the configuration file instead of running whole program")(
            "consume", "Run consumers only")("conf_file", value<std::string>(),
            "Use this configuration file")("buffers", value<std::string>(),
            "Use this directory for buffers")("maxBufferSize", value<qint64>(),
            "Set max size of single buffer. Has to be larger than 1024");

    store(parse_command_line(argc, argv, m_desc), m_vm);
    notify(m_vm);
}

MyNscaMain::~MyNscaMain()
{
    if (m_instance == this) {
        m_instance = 0L;
    } else {
        LOG_ENTRY(MyLogger::FATAL, "Instance overwritten!!!");
    }
}

MyNscaMain* MyNscaMain::getInstance()
{
    if (m_instance == 0L) {
        throw MyNscaMainException("No instance created!");
    }

    return m_instance;
}

int MyNscaMain::exec()
{
    int ret = 0;
    switch (parseCommandLineArgs()) {
        case START_PROGRAM:
            ret = startProgram();
            break;
        case PRINT_HELP:
            ret = printHelp();
            break;
        case CHECK_CONFIGURATION_FILE:
            ret = checkConfigurationFile();
            if (ret < 0) {
                std::cerr << "Wrong config given. Check log for details.\n";
            } else {
                std::cerr << "OK\n";
            }
            break;
        case START_ONLY_CONSUMERS:
            ret = startOnlyConsumers();
            break;
    }

    return ret;
}

int MyNscaMain::startOnlyConsumers()
{
    int ret = 0;
    if ((ret = checkConfigurationFile()) != 0) {
        std::cerr << "Wrong config given. Check log for details.\n";
        return ret;
    }

    ret = demonize();
    if (ret > 0) {
        //parent
        std::cerr << "Fork finished. New PID: " << ret << std::endl;
        return 0;
    } else if (ret < 0) {
        std::cerr << "Unable to fork. Check log for details." << std::endl;
        return -1;
    }

    //here goes only child
    //create data consumers
    if ((ret = createDataConsumers()) != 0) {
        return ret;
    }

    //set the buffer files
    if ((ret = createDataFiles()) != 0) {
        return ret;
    }

    //schedule initialization slot to be run
    QTimer::singleShot(0, this, SLOT(initDataConsumers()));

    //run the main loop.
    return m_app.exec();
}

MyNscaMain::WhatToDo MyNscaMain::parseCommandLineArgs()
{
    WhatToDo ret = START_PROGRAM;

    if (m_vm.count("help")) {
        ret = PRINT_HELP;
    } else if (m_vm.count("consume")) {
        ret = START_ONLY_CONSUMERS;
    } else if (m_vm.count("check")) {
        ret = CHECK_CONFIGURATION_FILE;
    }

    if (m_vm.count("conf_file") && m_vm.count("conf_file") == 1) {
        m_confPath = QString::fromStdString(
                m_vm["conf_file"].as<std::string>());
    }

    if (m_vm.count("buffers") && m_vm.count("buffers") == 1) {
        m_bufPath = QString::fromStdString(m_vm["conf_file"].as<std::string>());
    }

    if (m_vm.count("maxBufferSize") && m_vm.count("maxBufferSize") == 1) {
        qint64 val = m_vm["maxBufferSize"].as<qint64>();

        if (val <= 1024) {
            ret = PRINT_HELP;
        } else {
            m_maxBufSize = val;
        }
    }

    return ret;
}

int MyNscaMain::startProgram()
{
    int ret = 0;
    if ((ret = checkConfigurationFile()) != 0) {
        std::cerr << "Wrong config given. Check log for details.\n";
        return ret;
    }

    ret = demonize();
    if (ret > 0) {
        //parent
        std::cerr << "Fork finished. New PID: " << ret << std::endl;
        return 0;
    } else if (ret < 0) {
        std::cerr << "Unable to fork. Check log for details." << std::endl;
        return -1;
    }

    //create data consumers
    if ((ret = createDataConsumers()) != 0) {
        return ret;
    }

    //create data providers
    if ((ret = createDataProviders()) != 0) {
        return ret;
    }

    //set the buffer files
    if ((ret = createDataFiles()) != 0) {
        return ret;
    }

    //This is a full start so connect all signals
    connect(this, SIGNAL(dataConsummersReady()), this,
            SLOT(initDataProviders()));

    //schedule initialization slot to be run
    QTimer::singleShot(0, this, SLOT(initDataConsumers()));

    //run the main loop.
    return m_app.exec();
}

int MyNscaMain::printHelp()
{
    std::cerr << m_desc;
    return -1;
}

int MyNscaMain::demonize()
{
    pid_t pid, sid;

    pid = fork();

    //error
    if (pid < 0) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to fork.");
        return -1;
    }

    //parent
    if (pid > 0) {
        LOG_ENTRY(MyLogger::INFO, "Fork finished. New PID: "<<pid);
        return pid;
    }

    umask(0);

    sid = setsid();
    if (sid < 0) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to create new sid.");
        return -1;
    }

    if (chdir("/") < 0) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to change dir.");
        return -1;
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    return 0;
}

int MyNscaMain::checkConfigurationFile()
{
    QFile config(m_confPath);
    if (!config.exists()) {
        LOG_ENTRY(MyLogger::FATAL, "No such file: "<<m_confPath);
        return -1;
    } else {
        LOG_ENTRY(MyLogger::DEBUG, "File "<<m_confPath<<" exist, opening..");
    }

    if (!config.open(QIODevice::ReadOnly)) {
        LOG_ENTRY(MyLogger::FATAL,
                "Unable to open configuration file: "<<m_confPath);
        return -1;
    } else {
        LOG_ENTRY(MyLogger::DEBUG,
                "File "<<m_confPath<<" successfully opened for reading.");
    }

    bool result = ConfigurationParser::parseConfiguration(&config);
    config.close();

    if (result) {
        LOG_ENTRY(MyLogger::DEBUG, "Configuration file OK.");
        return 0;
    } else {
        return -1;
    }
}

DataChannel* MyNscaMain::provideDataChannel(const QString& client,
        const QString& provider)
{
    DataChannel* channel = 0L;
    try {
        MyNscaMain *instance = MyNscaMain::getInstance();
        if (client != 0L && provider != 0L
                && instance->m_dataFiles.contains(provider)) {
            try {
                channel = new DataChannel(instance->m_dataFiles[provider].get(),
                        ConfigurationParser::getRoutes(client, provider),
                        client, provider);
            } catch (const DataChannel::BadIdException &e) {
                LOG_ENTRY(MyLogger::DEBUG,
                        "Cannot create data channel for: " <<client<<" connected to "<<provider);
                //only to clarify
                channel = 0L;
            }
        }

    } catch (const MyNscaMainException& e) {
        LOG_ENTRY(MyLogger::FATAL, e.what());
    }

    return channel;
}

DataConsumer* MyNscaMain::getConsumer(const QString& id)
{

    DataConsumer *consumer = 0L;
    try {
        MyNscaMain *instance = MyNscaMain::getInstance();

        if (instance->m_dataConsumers.contains(id)) {
            consumer = instance->m_dataConsumers[id].get();
        }
    } catch (const MyNscaMainException& e) {
        LOG_ENTRY(MyLogger::FATAL, e.what());
    }

    return consumer;
}

void MyNscaMain::initDataConsumers()
{
    QList<QString> consumers = m_dataConsumers.keys();
    QString errorWhileConsumer;

    foreach(QString consumer, consumers) {
        if (m_dataConsumers[consumer]->init(
                ConfigurationParser::getDataConsumerAdditionalData(consumer),
                consumer, ConfigurationParser::getDataConsumerType(consumer))
                != 0) {
            //we have some error while initialization of data consumer
            LOG_ENTRY(MyLogger::FATAL,
                    "Error while initialization of "<<consumer);
            errorWhileConsumer = consumer;
            break;
        }
    }

    if (errorWhileConsumer != QString()) {
        //there was some error while initialization of consumers,
        //we have to stop those who are started
        foreach(QString consumer, consumers) {
            if (consumer != errorWhileConsumer) {
                m_dataConsumers[consumer]->close();
            } else {
                m_dataConsumers.clear();
                emit emergencyShutdown();
                break;
            }
        }
    } else {
        emit dataConsummersReady();
    }
}

void MyNscaMain::initDataProviders()
{
    QList<QString> providers = m_dataProviders.keys();
    QString errorWhileProvider;

    foreach(QString provider, providers) {
        if (m_dataProviders[provider]->init(
                ConfigurationParser::getDataProviderAdditionalData(provider),
                provider, ConfigurationParser::getDataProviderType(provider))
                != 0) {
            //we have some error while initialization of data consumer
            LOG_ENTRY(MyLogger::FATAL,
                    "Error while initialization of "<<provider);
            errorWhileProvider = provider;
            break;
        }
    }

    if (errorWhileProvider != QString()) {
        //there was some error while initialization of providers,
        //we have to stop those who are started
        foreach(QString provider, providers) {
            if (provider != errorWhileProvider) {
                m_dataProviders[provider]->close();
            } else {
                m_dataProviders.clear();
                emit emergencyShutdown();
                break;
            }
        }
    } else {
        emit dataProvidersReady();
    }
}

int MyNscaMain::createDataConsumers()
{
    QSet<QString> consumers = ConfigurationParser::getDataConsumersNames();
    foreach(QString consumer, consumers) {
        QString type;
        try {
            type = ConfigurationParser::getDataConsumerType(consumer);
        } catch (std::runtime_error &e) {
            LOG_ENTRY(MyLogger::FATAL, e.what()<<" in "<<consumer);
            return -1;
        }
        boost::shared_ptr<DataConsumer> cons(
                DataConsumerFactory::getDataConsumer(type));
        if (cons.get() == 0L) {
            LOG_ENTRY(MyLogger::FATAL,
                    "No such type: "<<type<<" in consumer factory");
            return -1;
        }
        m_dataConsumers[consumer] = cons;
    }

    return 0;
}

int MyNscaMain::createDataProviders()
{
    QSet<QString> providers = ConfigurationParser::getDataProvidersNames();
    foreach(QString provider, providers) {
        QString type;
        try {
            type = ConfigurationParser::getDataProviderType(provider);
        } catch (std::runtime_error &e) {
            LOG_ENTRY(MyLogger::FATAL, e.what()<<" in "<<provider);
            return -1;
        }
        boost::shared_ptr<DataProvider> prov(
                DataProviderFactory::getDataProvider(type));
        if (prov.get() == 0L) {
            LOG_ENTRY(MyLogger::FATAL,
                    "No such type: "<<type<<" in consumer factory");
            return -1;
        }
        m_dataProviders[provider] = prov;
    }

    return 0;
}

int MyNscaMain::createDataFiles()
{
    //check if suitable directory exist, if not create it
    QDir rootPath(m_bufPath);
    if (!rootPath.exists()) {
        LOG_ENTRY(MyLogger::INFO,
                "Directory: "<<m_bufPath<<" doesn't exist. Creating...");
        if (!rootPath.mkpath(".")) {
            LOG_ENTRY(MyLogger::FATAL,
                    "Unable to create a directory: "<<m_bufPath);
            return -1;
        }
    }
    QSet<QString> providers = ConfigurationParser::getDataProvidersNames();
    foreach(QString provider, providers) {
        m_dataFiles[provider] = boost::shared_ptr<DataFile>(new DataFile());
        if (!m_dataFiles[provider]->setFile(m_bufPath + "/" + provider,
                m_maxBufSize)) {
            LOG_ENTRY(MyLogger::FATAL,
                    "Unable to use "<<m_bufPath + "/" + provider <<" as a buffer file.");
            return -1;
        }
    }

    return 0;
}

void MyNscaMain::shutDown()
{
    QTimer::singleShot(0, getInstance(), SLOT(closeAllSlot()));
}

void MyNscaMain::closeAllSlot()
{
    LOG_ENTRY(MyLogger::INFO, "Asking all provider to shut down");

    foreach(QString provider, m_dataProviders.keys()) {
        m_dataProviders[provider]->close();
    }

    LOG_ENTRY(MyLogger::INFO, "Asking all consumers to shut down");

    foreach(QString consumer, m_dataConsumers.keys()) {
        m_dataConsumers[consumer]->close();
    }

    LOG_ENTRY(MyLogger::INFO, "Closing all buffers");

    foreach(QString buffer, m_dataFiles.keys()) {
        m_dataFiles[buffer]->closeFile();
    }

    LOG_ENTRY(MyLogger::INFO, "Exiting main event loop");

    m_app.exit(0);
}

} //namespace Base
} //namespace INZ_project
