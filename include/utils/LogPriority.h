//
// Created by eric on 18-11-1.
//

#ifndef HYPER_DISTRIBUTOR_DEMO_LOGPRIORITY_H
#define HYPER_DISTRIBUTOR_DEMO_LOGPRIORITY_H

#include <config.h>

#ifdef LOG_LIB_LOG4CPP

#include <log4cpp/Priority.hh>

enum LogPriority{
    FATAL = log4cpp::Priority::FATAL,
    ERROR = log4cpp::Priority::ERROR,
    WARN = log4cpp::Priority::WARN,
    INFO = log4cpp::Priority::INFO,
    DEBUG = log4cpp::Priority::DEBUG,
};

#else

enum LogPriority{
    FATAL = 0,
    ERROR = 1,
    WARN = 2,
    INFO = 3,
    DEBUG = 4,
};

#endif

#endif //HYPER_DISTRIBUTOR_DEMO_LOGPRIORITY_H
