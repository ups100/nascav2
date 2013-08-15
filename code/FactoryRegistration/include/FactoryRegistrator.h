/**
 * @file  FactoryRegistrator.h
 * @brief  Definition of #REGISTER_SUBCLASS macro
 * @date  15-08-2013
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __INZ_PROJECT_FACTORY_REGISTRATOR_H__
#define __INZ_PROJECT_FACTORY_REGISTRATOR_H__

#include "RegistrationHelper.h"

#define REGISTER_SUBCLASS(ToRegister, BaseClass, NameClass, FactoryClass) \
    class ToRegister : public BaseClass, \
    public ::INZ_project::FactoryRegistration:: \
    RegistrationHelper<ToRegister, BaseClass, NameClass, FactoryClass>

#endif //ifndef __INZ_PROJECT_FACTORY_REGISTRATOR_H__
