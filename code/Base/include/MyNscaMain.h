/**
 * @file  MyNscaMain.h
 * @brief  Definition of the Class MyNascaMain
 * @date  13-09-2013 17:38:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_D5A037F0_18C8_4dda_9955_7BDA9D7E33F3__INCLUDED_)
#define EA_D5A037F0_18C8_4dda_9955_7BDA9D7E33F3__INCLUDED_

#include "DataProvider.h"
#include "DataConsumer.h"
#include "ConfigurationParser.h"
#include <QMap>
#include <QString>
#include <boost/shared_ptr.hpp>
#include <QCoreApplication>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

namespace INZ_project {
namespace Base {

/**
 * @brief Main class for whole program
 */
class MyNscaMain : public QObject
{
Q_OBJECT
    ;
public:
    /**
     * @brief Constructor
     * @param[in] argc argument counter
     * @param[in] argv argument value
     */
    MyNscaMain(int argc, char **argv);

    /**
     * @brief Destructor
     * @details This object should not be destroyed while being in exec() function
     */
    virtual ~MyNscaMain();

    /**
     * @brief Makes command line argument parsing, creates all necessary objects
     * and starts the program main loop
     */
    int exec();

    /**
     * @brief Provides data channel for specific client and data provider
     * @details Use this function to obtain a convenient way to provide received logs
     * to correct data consumers
     * @param[in] client which has been connected
     * @param[in] provider to which client has been connected
     * @return object which should be used as communication channel
     */
    static DataChannel* provideDataChannel(const QString& client,
            const QString& provider);

    /**
     * @brief Shuts down the whole program
     */
    static void shutDown();

    /**
     * @brief Gets the consumer with given id
     * @param[in] id of consumer to obtain
     * @return Data consumer associated with this id or NULL if no such consumer
     */
    static DataConsumer* getConsumer(const QString& id);

    /**
     * @brief Exception class for errors occurred while parsing
     */
    class MyNscaMainException : public std::runtime_error
    {
    public:
        /**
         * @brief Constructor
         * @param[in] arg exception string
         */
        explicit MyNscaMainException(const std::string& arg)
                : std::runtime_error(arg)
        {
        }
    };
private slots:

    /**
     * @brief Slot used to initialize data consumer
     */
    void initDataConsumers();

    /**
     * @brief Slot used to initialize data Providers
     */
    void initDataProviders();

    void closeAllSlot();

signals:

    /**
     * @brief Signal emitted when all data consumers have been successfully initialized.
     */
    void dataConsummersReady();

    /**
     * @brief Signal emitted when something goes wrong while initialization and
     * we have to shut down the program
     */
    void emergencyShutdown();

    /**
     * @brief Signal emitted when all data providers have been successfully initialized.
     */
    void dataProvidersReady();

private:

    /**
     * @brief Global instance of this class.
     */
    static MyNscaMain *m_instance;

    /**
     * @brief Get's the instance of this class
     */
    static MyNscaMain* getInstance();

    /**
     * @brief Enumeration with possible actions.
     */
    enum WhatToDo
    {
        START_PROGRAM,
        PRINT_HELP,
        CHECK_CONFIGURATION_FILE,
        START_ONLY_CONSUMERS
    };

    /**
     * @todo Implement me
     * @brief Parses the command line arguments and told what to do
     * @return Action requested by user
     */
    WhatToDo parseCommandLineArgs();

    /**
     * @brief Starts the program
     * @return 0 if program ends without any error, non zero value otherwise
     */
    int startProgram();

    /**
     * @todo Implement me
     * @brief Prints the help text and exits
     * @return 0 if program ends without any error, non zero value otherwise
     */
    int printHelp();

    /**
     * @brief Checks the configuration file and exits
     * @return 0 if program ends without any error, non zero value otherwise
     */
    int checkConfigurationFile();

    /**
     * @brief Starts only the buffer files and data consumers.
     * @return 0 if program ends without any error, non zero value otherwise
     */
    int startOnlyConsumers();

    /**
     * @brief Creates data consumers according to configuration file.
     */
    int createDataConsumers();

    /**
     * @brief Creates data providers according to configuration file.
     */
    int createDataProviders();

    /**
     * @brief Creates data files for data providers according to configuration file.
     */
    int createDataFiles();

    /**
     * @brief Make this process a daemon.
     * @return 0 on success in child process, value above zero in parent
     * and below zero if error occurred
     */
    int demonize();

    /**
     * @brief Active data providers
     */
    QMap<QString, boost::shared_ptr<DataProvider> > m_dataProviders;
    /**
     * @brief Active data consumers
     */
    QMap<QString, boost::shared_ptr<DataConsumer> > m_dataConsumers;
    /**
     * @brief Active data consumers
     */
    QMap<QString, boost::shared_ptr<DataFile> > m_dataFiles;
    /**
     * @brief Main event loop
     */
    QCoreApplication m_app;
    /**
     * @brief Path to configuration file
     */
    QString m_confPath;
    /**
     * @brief Directory where buffer files should be placed
     */
    QString m_bufPath;

    /**
     * @brief Maximum size of each buffer file.
     */
    qint64 m_maxBufSize;

    /**
     * @brief Description of allowed program options
     */
    boost::program_options::options_description m_desc;

    /**
     * @brief Map of options which has been passed from command line.
     */
    boost::program_options::variables_map m_vm;
};

} //namespace Base
} //namespace INZ_project
#endif // !defined(EA_D5A037F0_18C8_4dda_9955_7BDA9D7E33F3__INCLUDED_)
