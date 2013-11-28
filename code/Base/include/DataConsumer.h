/**
 * @file  DataConsumer.h
 * @brief  Definition of the Class INZ_project::Base::DataConsumer
 * @date  13-09-2013 17:38:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_388BCAB5_7025_4448_8149_748AA8F8721B__INCLUDED_)
#define EA_388BCAB5_7025_4448_8149_748AA8F8721B__INCLUDED_

#include <QString>
#include <QObject>
#include "DataPortion.h"

namespace INZ_project {
namespace Base {

class Client;
class DataConsumer;

class ReadPortion;

/**
 * @brief Base class for all data consumers
 */
class DataConsumer : public QObject
{
Q_OBJECT
    ;
public:

    /**
     * @brief Constructor
     * @details Any action which prevents from deleting this object
     * should not be performed in constructor but in init() function
     */
    DataConsumer();

    /**
     * @brief Destructor
     */
    virtual ~DataConsumer();

    /**
     * @brief This function should stop data consumer.
     * @details After return from this function all connections should be closed
     * and object should be in ready to delete state.
     */
    virtual void close() =0;

    /**
     * @brief Initialize the data provider
     * @param[in] additionalData from configuration file
     * @param[in] consumerId consumer name from configuration file
     * @param[in] consumerType from configuration file
     * @return value returned from function initImpl()
     * @note This class does some preparation and then call initImpl() function,
     * which default implementation does nothing. To initialize your class
     * override initImpl() function.
     */
    virtual int init(const QString& additionalData, const QString& consumerId,
            const QString& consumerType);

    /**
     * @brief Gets the consumer type string
     * @return Consumer type string
     */
    const QString& getConsumerType() const;

    /**
     * @brief Gets the consumer ID string
     * @return Consumer ID in form groupID::providerID
     */
    const QString& getConsumerId() const;

public slots:

    virtual void consumeDataPortion(const ReadPortion *portion)=0;

private:
    /**
     * @brief Function used to initialize subclass
     * @param[in] additionalData from configuration file
     * @param[in] consumerId consumer name from configuration file
     * @param[in] consumerType from configuration file
     * @return You should return 0 on success and non zero value if an error
     * occurred.
     * @note Default implementation simply does nothing and return 0.
     * Override this function to initialize your subclass.
     */
    virtual int initImpl(const QString& additionalData, const QString& consumerId,
            const QString& consumerType);

    /**
     * @brief Consumer type string
     */
    QString m_consumerType;
    /**
     * @brief Consumer ID string
     */
    QString m_consumerId;
};

} //namespace Base
} //namespace INZ_project
#endif // !defined(EA_388BCAB5_7025_4448_8149_748AA8F8721B__INCLUDED_)
