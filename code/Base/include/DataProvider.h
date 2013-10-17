/**
 * @file  DataProvider.h
 * @brief  Definition of the Class INZ_project::Base::DataProvider
 * @date  13-08-2013 17:38:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#if !defined(EA_C29B231D_317B_4162_81AF_9381B6F6380C__INCLUDED_)
#define EA_C29B231D_317B_4162_81AF_9381B6F6380C__INCLUDED_

#include <QString>
#include "DataChannel.h"

namespace INZ_project {
namespace Base {

/**
 * @brief Base class for data providers
 */
class DataProvider
{

public:
    /**
     * @brief Constructor
     * @details Any action which prevents from deleting this object
     * should not be performed in constructor but in initImpl() function
     */
    DataProvider();

    /**
     * @brief Destructor
     */
    virtual ~DataProvider();

    /**
     * @brief This function should stop data provider.
     * @details After return from this function all connections should be closed
     * and object should be in ready to delete state
     */
    virtual void close() = 0;

    /**
     * @brief Initialize the data provider
     * @param[in] additionalData from configuration file
     * @param[in] providerId provider name from configuration file
     * @param[in] providerType from configuration file
     * @return value returned from function initImpl()
     * @note This class does some preparation and then call initImpl() function,
     * which default implementation does nothing. To initialize your class
     * override initImpl() function.
     */
    int init(const QString& additionalData, const QString& providerId,
            const QString& providerType);

    /**
     * @brief Gets the provider type string
     * @return Provider type string
     */
    const QString& getProviderType() const;

    /**
     * @brief Gets the provider ID string
     * @return Provider ID in form groupID::providerID
     */
    const QString& getProviderId() const;

private:
    /**
     * @brief Function used to initialize subclass
     * @param[in] additionalData from configuration file
     * @param[in] providerId provider name from configuration file
     * @param[in] providerType from configuration file
     * @return You should return 0 on success and non zero value if an error
     * occurred.
     * @note Default implementation simply does nothing and return 0.
     * Override this function to initialize your subclass.
     */
    virtual int initImpl(const QString& additionalData, const QString& providerId,
            const QString& providerType);

    /**
     * @brief Provider type string
     */
    QString m_providerType;
    /**
     * @brief Provider ID string
     */
    QString m_providerId;

};

} //namespace Base
} //namespace INZ_project
#endif // !defined(EA_C29B231D_317B_4162_81AF_9381B6F6380C__INCLUDED_)
