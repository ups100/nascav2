/**
 * @file  AlwaysAllow.cpp
 * @brief  Definition of the Class INZ_project::AAA::AlwaysAllow
 * @date  28-11-2013 18:18:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "AlwaysAllow.h"

namespace INZ_project {
namespace AAA {

AlwaysAllow::AlwaysAllow()
{

}

AlwaysAllow::~AlwaysAllow()
{

}

AAAModule* AlwaysAllow::clone()
{
    return new AlwaysAllow();
}

void AlwaysAllow::setAdditionalData(const QString &additionalData)
{

}

void AlwaysAllow::run()
{
    emit authorizationFinished(true);
    return;
}

void AlwaysAllow::newDataArrived()
{
    return;
}

} //namespace AAA
} //namespace INZ_project
