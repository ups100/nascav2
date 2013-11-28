/**
 * @file  DataConsumerRegister.h
 * @brief  Definition of the #DATA_CONSUMER macro
 * @date  26-11-2013
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#ifndef __DATA_CONSUMER_REGISTER_H__
#define __DATA_CONSUMER_REGISTER_H__

#include "DataConsumerFactory.h"
#include "DataConsumer.h"
#include "FactoryRegistrator.h"

/**
 * @warning Use only in INZ_project::Base namespace or
 * be sure to provide unique name
 */
#define DATA_CONSUMER(Consumer, ConsumerName) \
    struct DataConsumer ## ConsumerName ## DataConsumer \
    { static const char* getName() { return #ConsumerName; } }; \
    REGISTER_SUBCLASS(Consumer, \
            INZ_project::Base::DataConsumer, \
            DataConsumer ## ConsumerName ## DataConsumer, \
            INZ_project::Base::DataConsumerFactory)

#endif // __DATA_CONSUMER_REGISTER_H__
