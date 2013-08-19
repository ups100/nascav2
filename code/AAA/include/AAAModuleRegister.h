/**
 * @file  AAAModuleRegister.h
 * @brief  Definition of the #AAAMODULE macro
 * @date  19-sie-2013
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __AAA_MODULE_REGISTER_H__
#define __AAA_MODULE_REGISTER_H__

#include "AAAFactory.h"
#include "AAAModule.h"
#include "FactoryRegistrator.h"

/**
 * @warning Use only in INZ_project::AAA namespace or
 * be sure to provide unique name
 */
#define AAAMODULE(NewModule, NewModuleName) \
    struct AAAModule ## NewModuleName ## AAAModule \
    { static const char* getName() { return #NewModuleName; } }; \
    REGISTER_SUBCLASS(NewModule, INZ_project::AAA::AAAModule, \
    AAAModule ## NewModuleName ## AAAModule, INZ_project::AAA::AAAFactory)

#endif // __AAA_MODULE_REGISTER_H__
