/**
 * @file  SignAlgorithmRegister.h
 * @brief  Definition of the #SIGN_ALGORITHM macro
 * @date  21-11-2013
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __SIGN_ALGORITHM_REGISTER_H__
#define __SIGN_ALGORITHM_REGISTER_H__

#include "CryptographicFactory.h"
#include "SignAlgorithm.h"
#include "FactoryRegistrator.h"

/**
 * @warning Use only in INZ_project::Cryptographic namespace or
 * be sure to provide unique name
 */
#define SIGN_ALGORITHM(NewAlgorithm, NewAlgorithmName) \
    struct SignAlgorithm ## NewAlgorithmName ## SignAlgorithm \
    { static const char* getName() { return #NewAlgorithmName; } }; \
    REGISTER_SUBCLASS(NewAlgorithm, \
            INZ_project::Cryptographic::SignAlgorithm, \
            SignAlgorithm ## NewAlgorithmName ## SignAlgorithm, \
            INZ_project::Cryptographic::CryptographicFactory)

#endif // __SIGN_ALGORITHM_REGISTER_H__
