/**
 * @file  SymetricAlgorithmRegister.h
 * @brief  Definition of the #SYMETRIC_ALGORITHM macro
 * @date  20-sie-2013
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __SYMETRIC_ALGORITHM_REGISTER_H__
#define __SYMETRIC_ALGORITHM_REGISTER_H__

#include "CryptographicFactory.h"
#include "SymetricAlgorithm.h"
#include "FactoryRegistrator.h"

/**
 * @warning Use only in INZ_project::Cryptographic namespace or
 * be sure to provide unique name
 */
#define SYMETRIC_ALGORITHM(NewAlgorithm, NewAlgorithmName) \
    struct SymetricAlgorithm ## NewAlgorithmName ## SymetricAlgorithm \
    { static const char* getName() { return #NewAlgorithmName; } }; \
    REGISTER_SUBCLASS(NewAlgorithm, \
            INZ_project::Cryptographic::SymetricAlgorithm, \
            SymetricAlgorithm ## NewAlgorithmName ## SymetricAlgorithm, \
            INZ_project::Cryptographic::CryptographicFactory)

#endif // __SYMETRIC_ALGORITHM_REGISTER_H__
