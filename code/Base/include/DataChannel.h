/**
 * @file  DataChannel.h
 * @brief  Definition of the Class DataChannel
 * @date  13-09-2013 17:38:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_ECD19921_6648_4524_87B9_6086B11E331C__INCLUDED_)
#define EA_ECD19921_6648_4524_87B9_6086B11E331C__INCLUDED_

#include <QString>
#include <QSet>
#include <QMap>
#include <stdexcept>
#include <QObject>
#include <QQueue>

namespace INZ_project {
namespace Base {

class DataPortion;
class DataFile;
class DataConsumer;

/**
 * @brief Channel for passing logs to all required data consumers
 */
class DataChannel: public QObject
{
Q_OBJECT
public:
    /**
     * @brief Constructor
     * @details Creates a dataChannel with file as a buffer and
     * with specific destinations
     *
     * @param[in] file to be used as a buffer, This object do not take
     * the ownership of this parameter
     *
     * @param[in] destinations places where logs will be provided via
     * this channel. This Set should contain valid DataConsumers ID's
     *
     * @param[in] client which use this channel
     * @param[in] provider which is connected with this client
     *
     * @throws BadDestinationException if one or more destinations does not exist
     */
    DataChannel(DataFile *file, const QSet<QString>& destinations,
            const QString& client, const QString& provider);

    /**
     * @brief Destructor
     */
    virtual ~DataChannel();

    /**
     * @brief Writes a data portion to a buffer.
     * @details When this function returns successfully you may be sure
     * that passed logs are persistently stored and will be delivered to all
     * required places.
     *
     * @param[in] portion of logs to be stored
     * @return true on success, false if unable to submit this log portion
     * for example if there is currently not enough space in buffer.
     */
    bool write(const DataPortion& portion);

    /**
     * @brief Get's the size of free space required to store this data portion
     * @param[in] portion of logs
     * @return Size of required free space to store this logs portion
     */
    qint64 getSize(const DataPortion& portion) const;

    /**
     * @brief Get's the maximum size of buffer
     * @return Maximum buffer size
     */
    qint64 getBufferSize() const;

    /**
     * @brief Gets the current free space size
     * @return current free space size.
     */
    qint64 getFreeSpaceSize() const;

    /**
     * @brief Exception thrown then invalid data has been provided
     */
    class BadIdException : public std::runtime_error
    {
    public:
        /**
         * @brief Constructor
         * @param[in] arg exception string
         */
        explicit BadIdException(const std::string& arg)
                : std::runtime_error(arg)
        {
        }
    };

    /**
     * @brief Schedules portion to be written into channel when it will be possible
     * @param[in] portion to be written. This function does not take the ownership
     * of this object.
     * @return 0 if portion has been submitted, value below 0 if error occurred,
     * if value if greater than 0 it's id of submit
     */
    int scheduleForWrite(const DataPortion& portion);

signals:

    /**
     * @brief Signal emitted when portion has been submitted
     * @param[in] id of submitted logs portion
     */
    void portionWritten(qint64 id);



private:
    /**
     * @brief Buffer where logs are stored
     */
    DataFile *m_file;
    /**
     * @brief Consumers which should receive logs via this data channel
     */
    QMap<QString, DataConsumer*> m_consumers;
    /**
     * @brief Client which sends logs via this data channel
     */
    QString m_client;

    /**
     * @brief Provider which pass this logs to channel
     */
    QString m_provider;

    /**
     * @brief Ready to store logs header
     */
    QByteArray m_header;

    /**
     * @brief Queue of pending write demands
     */
    QQueue< qint64 > m_pendingWrites;
};

} //namespace Base
} //namespace INZ_project
#endif // !defined(EA_ECD19921_6648_4524_87B9_6086B11E331C__INCLUDED_)
