/**
 * @file  ReadPortion.h
 * @brief  Definition of the Class INZ_project::Base::ReadPortion
 * @date  20-09-2013 11:14:11
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __BASE_READ_PORTION_H__
#define __BASE_READ_PORTION_H__

#include <QList>
#include <QString>
#include <boost/shared_ptr.hpp>

namespace INZ_project {
namespace Base {

/**
 * @brief Portion of logs delivered to data consumer.
 */
class ReadPortion
{
public:
    /**
     * @brief Constructor
     * @param[in] logs which belongs to this portion
     * @param[in] client who send this portion
     * @param[in] provider who received this portion
     */
    ReadPortion(const QList<QString>& logs, const QString& client,
            const QString& provider);

    /**
     * @brief Constructor
     * @param[in] logs which belongs to this portion.
     * This object takes the ownership of this parameter
     * @param[in] client who send this portion
     * @param[in] provider who received this portion
     */
    ReadPortion(QList<QString>* logs, const QString& client,
            const QString& provider);

    /**
     * @brief Gets the list of logs related to this portion
     * @return List of logs
     */
    const QList<QString>& getLogs() const;

    /**
     * @brief Gets the client id in form groupId::clientId
     * @return client id
     */
    const QString& getClient() const;

    /**
     * @brief Gets the provider id in form groupId::clientId
     * @return provider id
     */
    const QString& getProvider() const;

private:
    /**
     * @brief Client ID
     */
    QString m_client;
    /**
     * @brief Provider ID
     */
    QString m_provider;
    /**
     * @brief Logs to be submited
     */
    boost::shared_ptr<QList<QString> > m_logs;

};

} //namespace Base
} //namespace INZ_project
#endif //__BASE_READ_PORTION_H__
