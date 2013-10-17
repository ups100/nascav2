/**
 * @file  DataChannel.cpp
 * @brief Implementation of the Class DataChannel
 * @date  27-09-2013 21:21:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "DataFile.h"
#include "DataConsumer.h"
#include "ReadPortion.h"
#include "MyNscaMain.h"
#include "LogEntry.h"
#include <QObject>
#include <QDataStream>

namespace INZ_project {
namespace Base {

DataFile::DataFile()
        : m_maxSize(0), m_firstLog(-1), m_toRead(-1), m_toWrite(-1),
                m_currentLogEnd(-1), m_nmbDest(-1)
{

}

DataFile::~DataFile()
{
    if (m_portion != 0L)
        delete m_portion;
}

bool DataFile::setFile(const QString& fileName, qint64 maxSize)
{
    if (maxSize <= 2 * sizeof(qint64)) {
        LOG_ENTRY(MyLogger::DEBUG, "Wrong max file size: "<<maxSize);
        return false;
    }
    QMutexLocker locker(&m_mutex);
    if (m_file.isOpen()) {
        LOG_ENTRY(MyLogger::DEBUG,
                "Trying to set buffer file but file is already set");
        return false;
    }

    m_file.setFileName(fileName);

    if (!m_file.exists()) {
        //file don't exist so we have to create it and set our structure
        LOG_ENTRY(MyLogger::INFO,
                "File "<<fileName<<" does not exist, creating new one.");
        if (!m_file.open(QIODevice::ReadWrite)) {
            LOG_ENTRY(MyLogger::ERROR, "Unable to create file "<<fileName);
            return false;
        }

        m_firstLog = 2 * sizeof(qint64);
        m_toRead = -1;
        m_toWrite = m_firstLog;

        QByteArray buf;
        QDataStream stream(&buf, QIODevice::ReadWrite);
        stream << m_toWrite << m_toRead;

        if (m_file.write(buf) < 0) {
            LOG_ENTRY(MyLogger::ERROR,
                    "Unable to write to "<<fileName<<" buffer.");
            m_file.close();
            return false;
        }
        //we have a brand new file opened and ready to work
        return true;
    }

    if (m_file.size() > maxSize) {
        LOG_ENTRY(MyLogger::ERROR,
                "File "<<fileName<<" is bigger than its limit");
        return false;
    }

    if (!m_file.open(QIODevice::ReadWrite)) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to open "<<fileName<<" for RW");
        return false;
    }

    m_maxSize = maxSize;

    QDataStream stream(&m_file);
    stream >> m_toWrite >> m_toRead;
    stream.unsetDevice();

    m_firstLog = m_file.pos();

    if (m_toWrite > maxSize) {

    }

    if (m_toRead < 0) {
        //there is no pending logs in this file
        if (m_toWrite > maxSize) {
            LOG_ENTRY(MyLogger::INFO, "File too big but empty, resizing...");

            m_file.resize(maxSize);
            m_toWrite = m_firstLog;
        }
    } else {
        //there are some pending logs we have to check if the are in correct format
        if (m_toWrite > maxSize) {
            LOG_ENTRY(MyLogger::ERROR, "File is already bigger than limit");
            m_file.close();
            m_firstLog = 0;
            m_toRead = -1;
            m_toWrite = -1;
            m_maxSize = -1;
            return false;
        }

        while (m_file.pos() != m_toRead) {
            if (!checkLogStructure()) {
                LOG_ENTRY(MyLogger::ERROR,
                        "Wrong log format near: "<<m_file.pos());
                m_file.close();
                m_firstLog = 0;
                m_toRead = -1;
                m_toWrite = -1;
                m_maxSize = -1;
                return false;
            }
        }
    }

    return true;
}

bool DataFile::checkLogStructure()
{
    try {
        if (m_toRead == -1)
            return false;

        if (!m_file.seek(m_toRead)) {
            LOG_ENTRY(MyLogger::ERROR, "Unable to seek in buffer.");
            return false;
        }

        QString nmbOfDest, nmbOfLogs;
        //client
        if (readLine() == QString() || readLine() == QString() || (nmbOfDest =
                readLine()) == QString()) {
            return false;
        }

        bool ok;
        int destCount = nmbOfDest.toInt(&ok);
        if (!ok) {
            LOG_ENTRY(MyLogger::ERROR, "Wrong file format");
            return false;
        }

        for (int i = 0; i < destCount; ++i) {
            QString dest = readLine();
            dest = dest.trimmed();

            if (MyNscaMain::getConsumer(dest) == 0L) {
                LOG_ENTRY(MyLogger::ERROR, "No such data consumer: "<<dest);
                return false;
            }
        }

        nmbOfLogs = readLine();
        int logsCount = nmbOfDest.toInt(&ok);
        if (!ok) {
            LOG_ENTRY(MyLogger::ERROR, "Wrong file format");
            return false;
        }

        for (int i = 0; i < logsCount; ++i) {
            QString log = readLine();

            if (log == QString()) {
                LOG_ENTRY(MyLogger::ERROR, "Wrong file format");
                return false;
            }
        }

    } catch (const QString& e) {
        return false;
    }

    return true;
}

qint64 DataFile::getMaxFileSize()
{
    return m_maxSize;
}

qint64 DataFile::getFreeSpaceSize()
{
    QMutexLocker locker(&m_mutex);
    return getFreeSpaceSizeImpl();
}

void DataFile::closeFile()
{
    QMutexLocker locker(&m_mutex);
    if (m_portion != 0L) {
        delete m_portion;
        m_nmbDest = -1;
    }

    if (m_file.isOpen()) {
        m_file.close();
    } else {
        LOG_ENTRY(MyLogger::ERROR,
                "Trying to close a file which is not opened.");
    }
}

bool DataFile::write(const QString& client, const QString& provider,
        const QMap<QString, DataConsumer*> consumers, const QByteArray& header,
        const QList<QString>& logs)
{
    QByteArray logsArray;
    logsArray.append(QString().setNum(logs.size()) + "\n");
    foreach(QString log, logs) {
        logsArray.append(log);
    }

    qint64 size = header.size() + logsArray.size();
    const QByteArray *tab[2];
    QMutexLocker locker(&m_mutex);
    if (!m_file.isOpen()) {
        LOG_ENTRY(MyLogger::ERROR,
                "Trying to write to "<<provider<<" buffer which has not been set.");
        return false;
    }

    if (getFreeSpaceSizeImpl() < size) {
        LOG_ENTRY(MyLogger::INFO,
                "Not enough free space in buffer for "<<provider);
        return false;
    }

    //there is enough space so we know that here we will start writing
    if (!m_file.seek(m_toWrite)) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to seek in "<<provider<<" buffer.");
        return false;
    }

    for (int i = 0; i < 1; ++i) {
        if (m_file.pos() > (m_toRead > 0) ? m_toRead : 0) {
            /*    |
             *    |< m_toRead
             *    |
             *    |< m_toWrite
             *    |
             */
            if (m_file.pos() + tab[i]->size() > m_maxSize) {
                if (m_file.write(tab[i]->left(m_maxSize - m_file.pos())) < 0) {
                    LOG_ENTRY(MyLogger::ERROR,
                            "Unable to write to "<<provider<<" buffer.");
                    return false;
                }
                if (!m_file.seek(m_firstLog)) {
                    LOG_ENTRY(MyLogger::ERROR,
                            "Unable to seek in "<<provider<<" buffer.");
                    return false;
                }
                if (m_file.write(
                        tab[i]->right(
                                tab[i]->size() - (m_maxSize - m_file.pos())))
                        < 0) {
                    LOG_ENTRY(MyLogger::ERROR,
                            "Unable to write to "<<provider<<" buffer.");
                    return false;
                }
            } else {
                if (m_file.write(*(tab[i])) < 0) {
                    LOG_ENTRY(MyLogger::ERROR,
                            "Unable to write to "<<provider<<" buffer.");
                    return false;
                }
            }
        } else {
            /*    |
             *    |< m_toWrite
             *    |
             *    |< m_toRead
             *    |
             */
            if (m_file.write(*(tab[i])) < 0) {
                LOG_ENTRY(MyLogger::ERROR,
                        "Unable to write to "<<provider<<" buffer.");
                return false;
            }
        }
    }

    QByteArray buf;
    QDataStream stream(&buf, QIODevice::ReadWrite);
    stream << (qint64) m_file.pos();
    if (m_toRead < 0) {
        stream << m_toWrite;
    }

    qint64 readTmp = (m_toRead < 0) ? m_toWrite : m_toRead;
    qint64 writeTmp = m_file.pos();

    if (!m_file.seek(0)) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to seek in "<<provider<<" buffer.");
        return false;
    }

    if (m_file.write(buf) < 0) {
        LOG_ENTRY(MyLogger::ERROR, "Unable to write to "<<provider<<" buffer.");
        return false;
    }

    if ((m_toRead < 0) && (m_portion == 0L)) {
        m_currentLogEnd = writeTmp;
        m_portion = new ReadPortion(logs, client, provider);
        m_nmbDest = consumers.size();

        foreach(DataConsumer* consumer, consumers) {
            QMetaObject::invokeMethod(consumer, "consumeDataPortion",
                    Qt::QueuedConnection, Q_ARG(const ReadPortion*, m_portion));
        }
    }

    m_toWrite = writeTmp;
    m_toRead = readTmp;

    return true;
}

QString DataFile::readLine()
{
    QString buf;
    buf = m_file.readLine();
    if (!buf.contains("\n")) {
        if (!m_file.seek(m_firstLog)) {
            LOG_ENTRY(MyLogger::ERROR, "Unable to seek in buffer.");
            throw QString("Unable to seek in buffer.");
        }
        buf += m_file.readLine();
    }
    return buf;
}

void DataFile::readAndDistribute()
{
    QList<QString> *logs;
    try {
        if (m_toRead == -1) {
            emit bufferEmpty();
            return;
        }

        if (!m_file.seek(m_toRead)) {
            LOG_ENTRY(MyLogger::ERROR, "Unable to seek in buffer.");
            return;
        }

        QString client, provider, nmbOfDest, nmbOfLogs;
        QMap<QString, DataConsumer*> destinations;
        logs = new QList<QString>();

        client = readLine();
        provider = readLine();
        nmbOfDest = readLine();

        bool ok;
        int destCount = nmbOfDest.toInt(&ok);
        if (!ok) {
            LOG_ENTRY(MyLogger::ERROR, "Wrong file format");
            return;
        }

        for (int i = 0; i < destCount; ++i) {
            QString dest = readLine();
            dest = dest.trimmed();

            DataConsumer *consumer = MyNscaMain::getConsumer(dest);
            if (consumer == 0L) {
                LOG_ENTRY(MyLogger::ERROR, "No such data consumer: "<<dest);
                return;
            } else {
                destinations[dest] = consumer;
            }
        }

        nmbOfLogs = readLine();
        int logsCount = nmbOfDest.toInt(&ok);
        if (!ok) {
            LOG_ENTRY(MyLogger::ERROR, "Wrong file format");
            return;
        }

        for (int i = 0; i < logsCount; ++i) {
            QString log = readLine();
            log = log.trimmed();

            logs->append(log);
        }

        m_currentLogEnd = m_file.pos();
        m_portion = new ReadPortion(logs, client, provider);
        m_nmbDest = destCount;

        foreach(DataConsumer* consumer, destinations) {
            QMetaObject::invokeMethod(consumer,
                    "consumeDataPortion", Qt::QueuedConnection,
                    Q_ARG(const ReadPortion*, m_portion));
        }
    } catch (const QString& e) {
        delete logs;
        return;
    }
}

void DataFile::confirmPortion(ReadPortion *portion)
{
    if (portion == 0L)
        return;
    QMutexLocker locker(&m_mutex);
    if (portion != m_portion)
        return;

    if (--m_nmbDest == 0) {

        delete m_portion;

        if (m_toWrite == m_currentLogEnd) {
            //there is nothing to read
            QByteArray buf;
            QDataStream stream(&buf, QIODevice::ReadWrite);
            stream << m_firstLog << (qint64) -1;

            if (!m_file.seek(0)) {
                LOG_ENTRY(MyLogger::ERROR, "Unable to seek in buffer.");
                return;
            }

            if (m_file.write(buf) < 0) {
                LOG_ENTRY(MyLogger::ERROR, "Unable to write to buffer.");
                return;
            }

            m_currentLogEnd = -1;
            m_toRead = -1;
            m_toWrite = m_firstLog;
        } else {
            //there is some pending log in a file to be read
            QByteArray buf;
            QDataStream stream(&buf, QIODevice::ReadWrite);
            stream << m_toWrite << m_currentLogEnd;

            if (!m_file.seek(0)) {
                LOG_ENTRY(MyLogger::ERROR, "Unable to seek in buffer.");
                return;
            }

            if (m_file.write(buf) < 0) {
                LOG_ENTRY(MyLogger::ERROR, "Unable to write to buffer.");
                return;
            }

            m_toRead = m_currentLogEnd;
            m_currentLogEnd = -1;

            readAndDistribute();
        }
    }
}

void DataFile::initialRead()
{
    QMutexLocker locker(&m_mutex);
    if(!m_file.isOpen()) {
        LOG_ENTRY(MyLogger::ERROR, "Trying to initial read but file not set");
    }
    readAndDistribute();
}

qint64 DataFile::getFreeSpaceSizeImpl()
{
    if (m_toRead == -1)
        return m_maxSize - m_firstLog;

    return (m_toRead < m_toWrite) ?
            m_maxSize - m_firstLog - (m_toWrite - m_toRead) :
            m_toRead - m_toWrite;
}

} //namespace Base
} //namespace INZ_project
