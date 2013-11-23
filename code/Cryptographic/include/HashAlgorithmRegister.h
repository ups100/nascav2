/**
 * @file  HashAlgorithmRegister.h
 * @brief  Definition of the #HASH_ALGORITHM macro
 * @date  21-11-2013
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __Hash_ALGORITHM_REGISTER_H__
#define __Hash_ALGORITHM_REGISTER_H__

#include "CryptographicFactory.h"
#include "HashAlgorithm.h"
#include "FactoryRegistrator.h"

/**
 * @warning Use only in INZ_project::Cryptographic namespace or
 * be sure to provide unique name
 */
#define HASH_ALGORITHM(NewAlgorithm, NewAlgorithmName) \
    struct HashAlgorithm ## NewAlgorithmName ## HashAlgorithm \
    { static const char* getName() { return #NewAlgorithmName; } }; \
    REGISTER_SUBCLASS(NewAlgorithm, \
            INZ_project::Cryptographic::HashAlgorithm, \
            HashAlgorithm ## NewAlgorithmName ## HashAlgorithm, \
            INZ_project::Cryptographic::CryptographicFactory)

#endif // __Hash_ALGORITHM_REGISTER_H__
