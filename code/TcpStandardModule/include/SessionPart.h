/**
 * @file  SessionPart.h
 * @brief  Definition of the Class INZ_project::TcpStandardModule::SessionPart
 * @date  17-11-2013 10:56:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __SESSION_PART_H__
#define __SESSION_PART_H__

#include <QObject>

namespace INZ_project {
namespace TcpStandardModule {

class Session;

/**
 * @brief Represents a part of session.
 * @details You should use this class to add a new element to session with client.
 */
class SessionPart: public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param[in] session to which this part belongs to
     * @param[in] milis number of milliseconds to timeout
     */
    SessionPart(Session *session = 0L, int milis = 10000);

    /**
     * @brief Destructor
     */
    virtual ~SessionPart();

    /**
     * @brief Assigns this SessionPart to a session.
     * @param[in] session to assign this part
     */
    virtual void setSession(Session *session);

    /**
     * @brief Sets the timeout to given value.
     */
    virtual void setTimeout(int milis);

    /**
     * @brief Creates a deep copy of this object
     */
    virtual SessionPart * clone() = 0;

public slots:
    /**
     * @brief Slot used to execute this session part
     */
    virtual void execute() = 0;

    /**
     * @brief Terminates this part of session
     */
    virtual void terminate() = 0;

signals:
    /**
     * @brief Signal emitted when session part has been finished
     */
    void finished(bool successfully);

protected:
    /**
     * @brief Session to which this session part belongs to
     */
    Session *m_session;

    /**
     * @brief Timeout for communication
     */
    int m_timeout;
};

} //namespace TcpStandardModule
} //namespace INZ_project

#endif //__SESSION_PART_H__
