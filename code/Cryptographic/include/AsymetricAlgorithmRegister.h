/**
 * @file  AsymetricAlgorithmRegister.h
 * @brief  Definition of the #ASYMETRIC_ALGORITHM macro
 * @date  20-sie-2013
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __ASYMETRIC_ALGORITHM_REGISTER_H__
#define __ASYMETRIC_ALGORITHM_REGISTER_H__

#include "CryptographicFactory.h"
#include "AsymetricAlgorithm.h"
#include "FactoryRegistrator.h"

/**
 * @warning Use only in INZ_project::Cryptographic namespace or
 * be sure to provide unique name
 */
#define ASYMETRIC_ALGORITHM(NewAlgorithm, NewAlgorithmName) \
    struct AsymetricAlgorithm ## NewAlgorithmName ## AsymetricAlgorithm \
    { static const char* getName() { return #NewAlgorithmName; } }; \
    REGISTER_SUBCLASS(NewAlgorithm, \
            INZ_project::Cryptographic::AsymetricAlgorithm, \
            AsymetricAlgorithm ## NewAlgorithmName ## AsymetricAlgorithm, \
            INZ_project::Cryptographic::CryptographicFactory)

#endif // __ASYMETRIC_ALGORITHM_REGISTER_H__
