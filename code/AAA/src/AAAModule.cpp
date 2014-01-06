/**
 * @file  AAAModule.cpp
 * @brief  Implementation of the Class INZ_project::AAA::AAAModule
 * @date  Created on:      13-sie-2013 17:38:09
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */

#include "AAAModule.h"

namespace INZ_project {
namespace AAA {

AAAModule::AAAModule()
        : m_conversationInterface(0L)
{

}

AAAModule::~AAAModule()
{

}

void AAAModule::setConversationInterface(ConversationInterface* conv)
{
    m_conversationInterface = conv;
    if (conv) {
        connect(conv, SIGNAL(newDataArrived()), this, SLOT(newDataArrived()));
    }
}

} //namespace AAA
} //namespace INZ_project
