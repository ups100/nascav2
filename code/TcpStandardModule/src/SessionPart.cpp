/**
 * @file  SessionInitializer.cpp
 * @brief  Definition of the Class INZ_project::TcpStandardModule::SessionPart
 * @date  08-12-2013 13:56:12
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "SessionPart.h"

namespace INZ_project {
namespace TcpStandardModule {

SessionPart::SessionPart(Session *session, int milis)
: m_session(session), m_timeout(milis)
{

}

SessionPart::~SessionPart()
{

}

void SessionPart::setSession(Session *session)
{
    m_session = session;
}

void SessionPart::setTimeout(int milis)
{
    m_timeout = milis;
}



} //namespace TcpStandardModule
} //namespace INZ_project
