/**
 * @file  LoginPass.h
 * @brief  Definition of the Class INZ_project::AAA::LoginPass
 * @date  06-01-2014 08:18:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __LOGIN_PASS_H__
#define __LOGIN_PASS_H__

#include "AAAModuleRegister.h"

namespace INZ_project {
namespace AAA {

/**
 * @brief Simply login, pass authorization
 * @details
 * Put in configuration file:
 * <login>Login: ZXY</login>
 * <password>Password: ABCDE</password>
 *
 * @note Password and string are being simplified so whitespace
 *  at beginning and at end are removed
 */
AAAMODULE(LoginPass, LoginPass)
{
public:
    LoginPass();
    virtual ~LoginPass();
    virtual AAAModule* clone();
    virtual void setAdditionalData(const QString &additionalData);
    virtual void run();
    virtual void newDataArrived();
private:

    /**
     * @brief Possible status of this class
     */
    enum
    {
        LOGIN_REQUESTED, PASSWORD_REQUESTED, NONE
    } m_status;

    /**
     * @brief Allowed messages
     */
    enum Message
    {
        REQUEST_LOGIN = 0,
        LOGIN = 1,
        REQUEST_PASSWORD = 2,
        PASSWORD = 3,
        INVALID = 4
    };
    static char m_codes[INVALID];

    /**
     * @brief Constructor for clone method
     * @details It is not allowed to copy this object in state other than none
     * @param[in] login of client
     * @param[in] pass of client
     */
    LoginPass(const QString& login, const QString& pass);

    /**
     * @brief Gets the message type according to given code
     * @param[in] message code
     * @return Message type or INVALID if no such message
     */
    Message getMessageType(const QByteArray &message);

    /**
     * @brief Gets the code for given message
     * @param[in] message type
     * @return message code or empty QByteArray if message type is invalid
     */
    QByteArray getMessageCode(Message message);

    /**
     * @brief Client's login
     */
    QString m_login;

    /**
     * @brief Client's password
     */
    QString m_pass;

    /**
     * @brief Informs if there is enough data to start authorization.
     */
    bool m_enoughtData;
};

} //namespace AAA
} //namespace INZ_project
#endif // __LOGIN_PASS_H__
