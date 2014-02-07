/**
 * @file  DataFile.h
 * @brief  Definition of the Class INZ_project::Base::DataFile
 * @date  20-09-2013 11:14:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __BASE_DATA_FILE_H__
#define __BASE_DATA_FILE_H__

#include <QObject>
#include <QFile>
#include <QMutex>
#include <QMap>
#include <QList>
#include <boost/shared_ptr.hpp>
#include <QQueue>

namespace INZ_project {
namespace Base {

class ReadPortion;
class DataConsumer;

class MyNscaMain;

/**
 * @brief Persisted buffer for logs storing
 * @details File should have such structure: (qint64)readOffset (qint64)writeOffset logEntry ... logEntry
 * each log should have such structure: client\n provider\n (as string) nmbOfDest\n
 * dest\n ... dest\n (as string) nmbOfLogs\n log\n log\n log\n
 */
class DataFile : public QObject
{
Q_OBJECT
public:
    friend class MyNscaMain;

    /**
     * @brief Constructor
     */
    DataFile();

    /**
     * @brief Destructor
     */
    virtual ~DataFile();

    /**
     * @brief Sets the file to be used as a buffer.
     * @details File should not exist, or have suitable structure
     * @param[in] fileName
     * @param[in] maxSize of file should be greater than 4096
     * @return true if file is ready for work, false and log if file is not suitable
     * @note This function is thread safe.
     */
    virtual bool setFile(const QString& fileName, qint64 maxSize = 4096 * 10);

    /**
     * @brief Get's the maximum size of a file
     * @return Maximum size of a file or -1 if file has not been set
     * @note This function is thread safe.
     */
    virtual qint64 getMaxFileSize();

    /**
     * @brief Returns the amount of free space in a file
     * @return Amount of free space or -1 if file has not been set
     * @note This function is thread safe.
     */
    virtual qint64 getFreeSpaceSize();

    /**
     * @brief Stores a log and notify receivers if no log is currently processed
     * @param[in] client who send this logs
     * @param[in] provider who received this logs
     * @param[in] consumers who should receive it
     * @param[in] header to be stored in file
     * @param[in] logs which should be stored
     * @return false if there was no file set or data portion is to big to place it
     * now in a file, true if portion was successfully written.
     * @note This function is thread safe.
     */
    bool write(const QString& client, const QString& provider,
            const QMap<QString, DataConsumer*> consumers,
            const QByteArray& header, const QList<QString>& logs);

    /**
     * @brief Schedules logs to be written
     * @return value below 0 if error occurred in logs,
     * 0 if logs has been written immediately, value above 0 as id of task
     */
    qint64 scheduleForWrite(const QString& client, const QString& provider,
            const QMap<QString, DataConsumer*> consumers,
            const QByteArray& header, const QList<QString>& logs);

protected slots:

    /**
     * @brief Closes the file.
     * @details If some pending logs are while submitting this will not be noticed
     * bufferClosed() signal is emitted.
     */
    virtual void closeFile();

    /**
     * @brief Reads first record from a file.
     * @details Does nothing if there is no record in file.
     */
    virtual void initialRead();

    /**
     * @brief Slot used to confirm that data portion has been successfully submitted
     *  to all data providers
     * @param[in] portion which has been submitted
     */
    void confirmPortion(const ReadPortion *portion);

signals:

    /**
     * @brief Signal emitted when there is no data to be distributed.
     */
    void bufferEmpty();

    /**
     * @brief Signal emitted when request with given id has been written
     * @param[in] id of written request
     */
    void requestWritten(qint64 id);

protected:
    /**
     * @brief Stores a log and notify receivers if no log is currently processed
     * @param[in] client who send this logs
     * @param[in] provider who received this logs
     * @param[in] consumers who should receive it
     * @param[in] header to be stored in file
     * @param[in] logs which should be stored
     * @param[in] logsArray array of logs to be stored
     * @return false if there was no file set or data portion is to big to place it
     * now in a file, true if portion was successfully written.
     * @note This function is thread safe.
     */
    bool writeImpl(const QString& client, const QString& provider,
            const QMap<QString, DataConsumer*> consumers,
            const QByteArray& header, const QList<QString>& logs,
            const QByteArray& logsArray);

private:

    /**
     * @brief Tries to write next message from queue if any
     */
    void popNextFromQueue();

    /**
     * @brief Check the structure of single logEntry
     * @return true if structure is suitable, false otherwise
     * @warning This function is not thread safe!
     */
    bool checkLogStructure();

    /**
     * @brief Reads file until /n sign
     * @details This function automatically rewind to m_firstLog and continue read if
     * EOF occurred before \n character.
     * @return Read content or QString() if an error occurred
     * @throw QString with error description
     * @warning This function is not thread safe!
     */
    QString readLine();

    /**
     * @brief Count the free space in file.
     * @return Amount of free space
     */
    qint64 getFreeSpaceSizeImpl();

    /**
     * @brief Reads next portion of logs
     * @returns Fresh portion of logs or NULL if portion has been already read.
     * @warning This function has the ownership of returned object you should
     * not delete it. If file is being closed but ReadPortion has not been confirmed
     * bufferClosed() signal is emitted and you should not use this object any more
     */
    virtual void readAndDistribute();

    /**
     * @brief File used as a circular buffer
     */
    QFile m_file;
    /**
     * @brief Max file size
     */
    qint64 m_maxSize;
    /**
     * @brief Position where first log is placed
     */
    qint64 m_firstLog;
    /**
     * @brief Position where current log to be read is placed
     */
    qint64 m_toRead;
    /**
     * @brief Position of first free space
     */
    qint64 m_toWrite;
    /**
     * @brief Offset where log end
     */
    qint64 m_currentLogEnd;
    /**
     * @brief Mutex for synchronization
     */
    QMutex m_mutex;
    /**
     * @brief Portion which is now distributed to data consumers
     */
    ReadPortion *m_portion;
    /**
     * @brief Number of data consumers which received notification about current readportion.
     */
    int m_nmbDest;

    struct Request
    {
        Request(const QString& cli, const QString &prov,
                const QMap<QString, DataConsumer*>& cons,
                const QByteArray& head, const QList<QString>& l, qint64 s);
        QString client;
        QString provider;
        QMap<QString, DataConsumer*> consumers;
        QByteArray header;
        QList<QString> logs;
        qint64 size;
        qint64 id;
    };

    /**
     * @brief Queue of requests to be saved
     */
    QQueue<boost::shared_ptr<Request> > m_requestQueue;

};

} //namespace Base
} //namespace INZ_project
#endif //__BASE_DATA_FILE_H__
