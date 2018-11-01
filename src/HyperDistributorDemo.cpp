//
// Created by eric on 18-10-30.
//
#include "config.h"

#include <iostream>
#include <string>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/Priority.hh>

#include <HyperDistributorDemo.h>
#include <HyperDistributor/HyperDistributor.h>
#include <utils/LogPriority.h>


/**
 * launcher
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    auto *demo = new HyperDistributorDemo();
    demo->display();
    delete(demo);

    return 0;
}

/**
 * Hyper Distributor Demo
 */
HyperDistributorDemo::HyperDistributorDemo() {
    // 初始化日志
    initLog();

    // 输出基本信息
    std::string versionInfo = "VERSION: ";
    versionInfo.append(DEMO_VERSION);
    this->log(LogPriority(INFO), versionInfo);
#ifdef TASK_LOCK_FREE
    this->log(LogPriority(INFO), "TASK MODIFY POLICY : LOCK FREE");
#else
    this->log(LogPriority(INFO), "TASK MODIFY POLICY : USE LOCK");
#endif
}

HyperDistributorDemo::~HyperDistributorDemo() {
    log4cpp::Category::shutdown();
}

void HyperDistributorDemo::display() {
    auto *hyperDistributor = new hd::HyperDistributor("HD1");
    hyperDistributor->push(nullptr);
    hyperDistributor->modify(nullptr, nullptr);
    hyperDistributor->get();
    delete(hyperDistributor);
}


/**
 * private function
 */

/**
 * 初始化日志
 */
void HyperDistributorDemo::initLog() {
#ifdef LOG_LIB_LOG4CPP
    // 构造日志layout
    auto *layout = new log4cpp::PatternLayout();
    layout->setConversionPattern("[HDDemo][%c][%p][%d{%H:%M:%S}]%x%m%n");
    log4cpp::OstreamAppender *appender = new log4cpp::OstreamAppender("HyperDistributor", &std::cout);
    appender->setLayout(layout);

    log4cpp::Category::getInstance(LOG_APPENDER_NAME).addAppender(appender);

#ifdef DEMO_DEBUG
    log4cpp::Category::getInstance(LOG_APPENDER_NAME).setPriority(log4cpp::Priority::DEBUG);
#else
    log4cpp::Category::getInstance(LOG_APPENDER_NAME).setPriority(log4cpp::Priority::WARN);
#endif
#endif
}

/**
 * 打印日志
 * @param priority
 * @param s
 */
void HyperDistributorDemo::log(LogPriority priority, std::string s) {
#ifdef LOG_LIB_LOG4CPP
    log4cpp::Category::getInstance(LOG_APPENDER_NAME).log(priority, s);
#else
    std::cout << priority << " " << s << std::endl;
#endif
}

