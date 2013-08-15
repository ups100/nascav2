/**
 * @file  RegistrationHelper.h
 * @brief  Definition of INZ_project::FactoryRegistration::RegistrationHelper template class
 * @date  15-08-2013
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */
#ifndef __INZ_PROJECT_REGISTRATION_HELPER_H__
#define __INZ_PROJECT_REGISTRATION_HELPER_H__

#include "Registrator.h"

namespace INZ_project {
namespace FactoryRegistration {

// only for eclipse resolution problems
template<typename ToRegister, typename BaseClass, typename NameClass,
        typename FactoryClass>
class Registrator;

/**
 * @brief Helper class for registration in factory
 * @details All class derive from this class.
 *
 * @note Derived class has to provide implementation of it's constructor
 *  outside class definition
 *
 *  @warning Subclass constructor can not be provided by compiler!
 */
template<typename ToRegister, typename BaseClass, typename NameClass,
        typename FactoryClass>
class RegistrationHelper
{
public:
    /**
     * @brief Constructor
     * @details Simply does nothing but is necessary for registration process
     */
    RegistrationHelper();

private:
    /**
     * @brief Static variable for registration process.
     */
    static Registrator<ToRegister, BaseClass, NameClass, FactoryClass> m_registerMe;
};

template<typename ToRegister, typename BaseClass, typename NameClass,
        typename FactoryClass>
RegistrationHelper<ToRegister, BaseClass, NameClass, FactoryClass>::RegistrationHelper()
{
    //Has to be here to enforce compiler to compile template source code
    m_registerMe.dummy();
}

template<typename ToRegister, typename BaseClass, typename NameClass,
        typename FactoryClass>
Registrator<ToRegister, BaseClass, NameClass, FactoryClass> RegistrationHelper<
        ToRegister, BaseClass, NameClass, FactoryClass>::m_registerMe;

} //namespace FactoryRegistration
} //namespace INZ_project

#endif // ifndef __INZ_PROJECT_REGISTRATION_HELPER_H__
