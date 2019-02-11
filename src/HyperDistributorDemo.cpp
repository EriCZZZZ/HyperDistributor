//
// Created by eric on 18-10-30.
//
#include "config.h"

#include <iostream>
#include <string>
#include <thread>
#include <cstdlib>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/Priority.hh>

#include <HyperDistributorDemo.h>
#include <HyperDistributor/HyperDistributor.h>
#include <utils/LogPriority.h>

#include "utils/exceptions/NotImplementedException.h"

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
#endif /* TASK_LOCK_FREE */

#ifdef DISTRIBUTOR_LOCK_FREE
    this->log(LogPriority(INFO), "DISTRIBUTOR POLICY : LOCK FREE");
#else
    this->log(LogPriority(INFO), "DISTRIBUTOR POLICY : USE LOCK");
#endif /* DISTRIBUTOR_LOCK_FREE */
}

HyperDistributorDemo::~HyperDistributorDemo() {
    log4cpp::Category::shutdown();
}

/**
 * 展示
 */
void HyperDistributorDemo::display() {
    try {
        log(LogPriority(INFO), "display start");
        auto *hyperDistributor = new hd::HyperDistributor("HD1");
        printf("display: this=%p\n", this);
        std::thread t_producer(&HyperDistributorDemo::producerWithCount, this, hyperDistributor, 10000);
        t_producer.join();
        log(LogPriority(INFO), hyperDistributor->status());
        std::thread t_consumer1(&HyperDistributorDemo::consumer, this, hyperDistributor, "c1");
        std::thread t_consumer2(&HyperDistributorDemo::consumer, this, hyperDistributor, "c2");
        std::thread t_consumer3(&HyperDistributorDemo::consumer, this, hyperDistributor, "c3");
        std::thread t_consumer4(&HyperDistributorDemo::consumer, this, hyperDistributor, "c4");
        t_consumer1.join();
        t_consumer2.join();
        t_consumer3.join();
        t_consumer4.join();
        log(LogPriority(INFO), hyperDistributor->status());
        delete(hyperDistributor);

        log(LogPriority(INFO), "display finish");
    } catch (NotImplementedException& nie) {
        this->log(LogPriority(FATAL), nie.what());
    }
}

/**
 * do benchmark
 */
void HyperDistributorDemo::benchmark() {
    // todo benchmark
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
#else /* DEMO_DEBUG */
    log4cpp::Category::getInstance(LOG_APPENDER_NAME).setPriority(log4cpp::Priority::WARN);
#endif /* DEMO_DEBUG */
#else /* LOG_LIB_LOG4CPP */
    // Do NOTHING when don't use log4cpp
#endif /* LOG_LIB_LOG4CPP */
}

/**
 * 打印日志
 * @param priority
 * @param s
 */
void HyperDistributorDemo::log(LogPriority priority, std::string s) {
#ifdef LOG_LIB_LOG4CPP
    log4cpp::Category::getInstance(LOG_APPENDER_NAME).log(priority, s);
#else /* LOG_LIB_LOG4CPP */
    std::cout << priority << " " << s << std::endl;
#endif /* LOG_LIB_LOG4CPP */
}

/*
 * Test functions
 */

void HyperDistributorDemo::producerWithCount(HyperDistributor *hd, int cnt) {
    printf("hdd=%p, hd=%p, cnt=%d\n", this, hd, cnt);
    for(int i = 0; i < cnt; i++) {
        char* buf = new char[128];
        sprintf(buf, "Node no: %d / %d", i, cnt - 1);
        hd->append(new Node((void*) buf));
        printf("buf : %d s=%s addr=%p\n", i, buf, buf);
    }
}

void HyperDistributorDemo::consumer(HyperDistributor *hd, std::string id) {
    std::ostringstream  idStream;
//    idStream << std::this_thread::get_id();
    idStream << id;
    printf("consumer %s start stream_pointer=%p\n", idStream.str().c_str(), &idStream);
    for(int i = 0; i < 2500; ) {
        Node* n = hd->get();
        if(n != nullptr) {
            printf("Consumer id=%s n=%p val=%s\n", idStream.str().c_str(), n, (char*) n->getValP());
            delete((char*)n->getValP());
            i++;
            for(int j = 0; j < 1000; j++) {

            }
        }
    }

    std::ostringstream exitInfo;
    exitInfo << "consumer " << id << " exit";

    log(LogPriority(DEBUG), exitInfo.str());
}
