/**
 * @file  LoginPass.cpp
 * @brief  Definition of the Class INZ_project::AAA::LoginPass
 * @date  06-01-2014 08:18:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "LoginPass.h"
#include "LogEntry.h"

namespace INZ_project {
namespace AAA {

char LoginPass::m_codes[INVALID] = { REQUEST_LOGIN, LOGIN, REQUEST_PASSWORD,
        PASSWORD };

LoginPass::LoginPass()
        : m_status(NONE), m_enoughtData(false)
{

}

LoginPass::~LoginPass()
{

}

AAAModule* LoginPass::clone()
{
    return m_conversationInterface ? 0L : new LoginPass();
}

void LoginPass::setAdditionalData(const QString &additionalData)
{
    static const QString passString = "Password:";
    static const QString loginString = "Login:";
    int pos = -1, nlPos = -1;

    //check if there is login to set
    if ((pos = additionalData.indexOf(loginString)) != -1) {
        nlPos = additionalData.indexOf("\n", pos);
        m_login = additionalData.mid(pos + loginString.size(),
                nlPos - pos - loginString.size()).simplified();

    } else {
        LOG_ENTRY(MyLogger::ERROR, "Login has not been provided.");
    }

    //check if there is password to set
    if ((pos = additionalData.indexOf(passString)) != -1) {
        nlPos = additionalData.indexOf("\n", pos);
        m_pass = additionalData.mid(pos + passString.size(),
                nlPos - pos - passString.size()).simplified();

    } else {
        LOG_ENTRY(MyLogger::ERROR, "Password has not been provided.");
    }

    //we need correct login and password to authorize this user.
    //password and login may not be blank
    if (m_login == QString() || m_pass == QString() || m_login.size() == 0
            || m_pass.size() == 0) {
        m_enoughtData = false;
    } else {
        m_enoughtData = true;
    }

}

void LoginPass::run()
{
    if (!m_enoughtData) {
        LOG_ENTRY(MyLogger::ERROR, "Not enough data to start authorization.");
        emit authorizationFinished(false);
    } else if (m_status == NONE) {
        if (m_conversationInterface->sendMessage(getMessageCode(REQUEST_LOGIN))
                < 0) {
            emit authorizationFinished(false);
        }
        m_status = LOGIN_REQUESTED;
    }
}

void LoginPass::newDataArrived()
{
    QByteArray data = m_conversationInterface->getMessage();
    switch (m_status) {
        case LOGIN_REQUESTED:
            if (getMessageType(data) == LOGIN) {
                QString login = QString::fromUtf8(data.mid(1));
                if (login == m_login) {
                    //login is correct, now get password
                    if (m_conversationInterface->sendMessage(
                            getMessageCode(REQUEST_PASSWORD)) < 0) {
                        m_status = NONE;
                        emit authorizationFinished(false);
                    } else {
                        m_status = PASSWORD_REQUESTED;
                    }
                } else {
                    m_status = NONE;
                    emit authorizationFinished(false);
                }
            } else {
                LOG_ENTRY(MyLogger::ERROR,
                        "Unexpected message: "<<getMessageType(data));
                m_status = NONE;
                emit authorizationFinished(false);
            }
            break;
        case PASSWORD_REQUESTED:
            if (getMessageType(data) == PASSWORD) {
                QString pass = QString::fromUtf8(data.mid(1));
                m_status = NONE;
                emit authorizationFinished(pass == m_pass);
            } else {
                LOG_ENTRY(MyLogger::ERROR,
                        "Unexpected message: "<<getMessageType(data));
                m_status = NONE;
                emit authorizationFinished(false);
            }
            break;
        default:
            LOG_ENTRY(MyLogger::ERROR,
                    "No data should be received in this state");
            break;
    }
}

LoginPass::LoginPass(const QString& login, const QString& pass)
        : m_status(NONE), m_login(login), m_pass(pass)
{
    if (m_login == QString() || m_pass == QString() || m_login.size() == 0
            || m_pass.size() == 0) {
        m_enoughtData = false;
    } else {
        m_enoughtData = true;
    }
}

LoginPass::Message LoginPass::getMessageType(const QByteArray &message)
{
    Message ret = INVALID;
    for (int i = 0; i < INVALID; ++i) {
        if (*(message.data()) == m_codes[i]) {
            ret = (Message) i;
            break;
        }
    }

    return ret;
}

QByteArray LoginPass::getMessageCode(LoginPass::Message message)
{
    return message != INVALID ?
            QByteArray(&(m_codes[message]), 1) : QByteArray();
}

} //namespace AAA
} //namespace INZ_project
