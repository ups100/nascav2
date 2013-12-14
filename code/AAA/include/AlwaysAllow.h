/**
 * @file  AlwaysAllow.h
 * @brief  Definition of the Class INZ_project::AAA::AlwaysAllow
 * @date  28-11-2013 18:18:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __ALWAYS_ALLOW_H__
#define __ALWAYS_ALLOW_H__

#include "AAAModuleRegister.h"

namespace INZ_project {
namespace AAA {

AAAMODULE(AlwaysAllow, AlwaysAllow)
{
public:
    AlwaysAllow();
    virtual ~AlwaysAllow();
    virtual AAAModule* clone();
    virtual void setAdditionalData(const QString &additionalData);
    virtual void run();
    virtual void newDataArrived();
};

} //namespace AAA
} //namespace INZ_project
#endif // __ALWAYS_ALLOW_H__
