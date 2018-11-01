#include <utility>

//
// Created by eric on 18-10-30.
//

#include <iostream>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Category.hh>


#include <HyperDistributor/HyperDistributor.h>


hd::HyperDistributor::HyperDistributor() {
    init("DefaultHyperDistributor");
}

hd::HyperDistributor::HyperDistributor(std::string instanceName) {
    init(std::move(instanceName));
}

hd::HyperDistributor::~HyperDistributor() {

}

void hd::HyperDistributor::push(Task *task) {
    log(LogPriority(INFO), "push task");

}

void hd::HyperDistributor::modify(Task *task, void *newData) {
    log(LogPriority(INFO), "modify task");

}

Task *hd::HyperDistributor::get() {
    log(LogPriority(INFO), "get task");
    return nullptr;
}

/**
 * private function
 */

/**
 * 初始化
 * @param instanceName
 */
void hd::HyperDistributor::init(std::string instanceName) {
    this->instanceName = std::move(instanceName);
    initLog();

    this->log(LogPriority(INFO), "HyperDistributor init finish");
}

/**
 * 初始化日志相关
 */
void hd::HyperDistributor::initLog() {
#ifdef LOG_LIB_LOG4CPP
    // 构造日志layout
    auto *layout = new log4cpp::PatternLayout();
    layout->setConversionPattern("[HD][%c][%p][%d{%H:%M:%S}]%x%m%n");
    log4cpp::OstreamAppender *appender = new log4cpp::OstreamAppender("HyperDistributor", &std::cout);
    appender->setLayout(layout);

    log4cpp::Category::getInstance(this->instanceName).addAppender(appender);

#ifdef DEMO_DEBUG
    log4cpp::Category::getInstance(this->instanceName).setPriority(log4cpp::Priority::DEBUG);
#else
    log4cpp::Category::getInstance(this->instanceName).setPriority(log4cpp::Priority::WARN);
#endif
#endif
}

/**
 * 打印日志
 * @param priority
 * @param s
 */
void hd::HyperDistributor::log(LogPriority priority, std::string s) {
#ifdef LOG_LIB_LOG4CPP
    log4cpp::Category::getInstance(this->instanceName).log(priority, s);
#else
    std::cout << priority << " " << s << std::endl;
#endif
}
