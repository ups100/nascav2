/**
 * @file  LogEntry.h
 * @brief  Definition of #LOG_ENTRY macro
 * @date  15-08-2013
 * @author Krysztof Opasiak <ups100@tlen.pl>
 */
#ifndef __LOG_ENTRY_H__
#define __LOG_ENTRY_H__

#include <Logger.h>

#define GLOBAL_LOG_LEVEL 0

#define CHECK_LOG_LEVEL(Level) (Level) >= (GLOBAL_LOG_LEVEL)

#define LOG_ENTRY(LogLevel, LogMessage)  ::MyLogger::Logger::appendLog<CHECK_LOG_LEVEL(LogLevel)>( \
            LogLevel, __LINE__, __FILE__ , __func__, \
            (*(::MyLogger::Logger::getNewStream()))<<LogMessage );

#endif //ifndef __LOG_ENTRY_H__
