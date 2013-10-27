/**
 * @file  Registrator.h
 * @brief  Definition of INZ_project::FactoryRegistration::Registrator template class
 * @date  15-08-2013
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __INZ_PROJECT_REGISTRATOR_H__
#define __INZ_PROJECT_REGISTRATOR_H__

#include <boost/function.hpp>

namespace INZ_project {
namespace FactoryRegistration {

/**
 * @brief Class used for registration in factories.
 * @details Provides function which creates object associated with name provided
 * by NameClass::getName method.
 *
 * @warning Each Factory should provide
 * registerFunc(const char*, BaseClass* (*fun)()) method.
 *
 * @warning Each ToRegister class should provide default constructor (without parameters)
 */
template<typename ToRegister, typename BaseClass, typename NameClass,
        typename FactoryClass>
class Registrator
{
public:
    /**
     * @brief Constructor
     * @details Registers object in factory
     */
    Registrator();

    /**
     * @brief Function which creates object passed as ToRegister template parameter.
     */
    static BaseClass* creator();

    /**
     * @brief Function to suppress "Statement has no effect" compiler warning
     */
    void dummy();
};

template<typename ToRegister, typename BaseClass, typename NameClass,
        typename FactoryClass>
Registrator<ToRegister, BaseClass, NameClass, FactoryClass>::Registrator()
{
    //dodac loga
    FactoryClass::registerFunc(boost::function<BaseClass* ()>(creator),
            NameClass::getName());
}

template<typename ToRegister, typename BaseClass, typename NameClass,
        typename FactoryClass>
BaseClass* Registrator<ToRegister, BaseClass, NameClass, FactoryClass>::creator()
{
    return new ToRegister();
}

template<typename ToRegister, typename BaseClass, typename NameClass,
        typename FactoryClass>
void Registrator<ToRegister, BaseClass, NameClass, FactoryClass>::dummy()
{
    return;
}
} //namespace FactoryRegistration
} //namespace INZ_project

#endif // ifndef __INZ_PROJECT_REGISTRATOR_H__
