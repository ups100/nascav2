/**
 * @file  DataProviderRegister.h
 * @brief  Definition of the #DATA_PROVIDER macro
 * @date  26-11-2013
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __DATA_PROVIDER_REGISTER_H__
#define __DATA_PROVIDER_REGISTER_H__

#include "DataProviderFactory.h"
#include "DataProvider.h"
#include "FactoryRegistrator.h"

/**
 * @warning Use only in INZ_project::Base namespace or
 * be sure to provide unique name
 */
#define DATA_PROVIDER(Provider, ProviderName) \
    struct DataProvider ## ProviderName ## DataProvider \
    { static const char* getName() { return #ProviderName; } }; \
    REGISTER_SUBCLASS(Provider, \
            INZ_project::Base::DataProvider, \
            DataProvider ## ProviderName ## DataProvider, \
            INZ_project::Base::DataProviderFactory)

#endif // __DATA_PROVIDER_REGISTER_H__
