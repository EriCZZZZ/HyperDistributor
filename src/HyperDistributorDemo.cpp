//
// Created by eric on 18-10-30.
//
#include "config.h"

#include <iostream>
#include <string>
#include <thread>
#include <cstdlib>
#include <map>
#include <random>
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

    // init random engine
    this->randomEngine.seed(DEMO_RANDOM_SEED);

    std::cout << "FD CNT:" << FD_CNT << " START:" << FD_START << " END:" << FD_END << std::endl;

    this->isProducerRunning = true;
}

HyperDistributorDemo::~HyperDistributorDemo() {
    log4cpp::Category::shutdown();
}

/**
 * 展示
 */
void HyperDistributorDemo::display() {
    try {
        log(LogPriority(INFO), "Display start");

        auto *hd = new hd::HyperDistributor("HD1");

        std::map<int, bool> masks;

        // register handlers
        Handler handler = [](SAE_EVENTS events, int fd, void* p_val) {
            bool isReadable = false;
            bool isWritable = false;

            if (NODE_CHECK_EVENT(events, NODE_EVENTS_READABLE)) {
                isReadable = true;
            }
            if (NODE_CHECK_EVENT(events, NODE_EVENTS_WRITABLE)) {
                isWritable = true;
            }

            printf("HANDLER fd:%d events:%#06x readable[%d] writable[%d]\n", fd, events, isReadable, isWritable);
        };
        this->registerFds(hd, handler, nullptr);

        // start producer thread to create events
        std::thread t_producer(&HyperDistributorDemo::producer, this, hd);

        // start consumers to consume events
        std::thread t_consumer1(&HyperDistributorDemo::consumer, this, hd, "c1");
        std::thread t_consumer2(&HyperDistributorDemo::consumer, this, hd, "c2");
        std::thread t_consumer3(&HyperDistributorDemo::consumer, this, hd, "c3");
        std::thread t_consumer4(&HyperDistributorDemo::consumer, this, hd, "c4");
        std::thread t_consumer5(&HyperDistributorDemo::consumer, this, hd, "c5");

        t_producer.join();
        this->isProducerRunning = false;

        t_consumer1.join();
        t_consumer2.join();
        t_consumer3.join();
        t_consumer4.join();
        t_consumer5.join();
        log(LogPriority(INFO), hd->status());

        log(LogPriority(INFO), "Display finish");

        delete(hd);
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
void HyperDistributorDemo::registerFds(HyperDistributor *hd, Handler defaultHandler, std::map<int, Handler> *handlers) {
    int fd = FD_START;
    while(fd <= FD_END) {
        Handler h = (handlers == nullptr) ? defaultHandler : (*handlers)[fd];
        if (h == nullptr) {
            h = defaultHandler;
        }
        Node* n = hd->getNodeByFd(fd);
        while(true) {
            SAE_BITS oldSAE = n->getStatusAndEvents();
            SAE_STATUS status = NODE_STATUS(oldSAE);
            if(status != NODE_STATUS_UN_INIT) {
                break;
            }
            SAE_BITS newSAE = oldSAE;
            newSAE = NODE_SET_STATUS(newSAE, NODE_STATUS_WAIT_EVENT);
            if(n->casStatusAndEvents(newSAE, oldSAE)) {
                break;
            }
        }
        this->ctlHandler(fd, h, HD_CTL_ADD);
        fd++;
    }
}

void HyperDistributorDemo::producer(HyperDistributor *hd) {
    // do producer
    this->log(LogPriority(INFO), "Producer start");
//    while(this->isProducerRunning) {
    int fd = FD_START;
    for(int i = 0; i < 2; i++) {
        while(fd <= FD_END) {
            // random a fd to add new events
            Node* n = hd->getNodeByFd(fd);
            SAE_BITS oldSAE, newSAE;
            SAE_STATUS status;
            // random a event to create
            do {
                oldSAE = n->getStatusAndEvents();
                SAE_EVENT newEvent = this->randomEvent();
                status = NODE_STATUS(oldSAE);
                newSAE = NODE_ADD_EVENT(oldSAE, newEvent);
            } while(!n->casStatusAndEvents(newSAE, oldSAE));
            // if WAIT_EVENT do schedule
            if (NODE_STATUS_WAIT_EVENT == status) {
                hd->schedule(n);
            }

            std::this_thread::yield();
            fd++;
        }
    }
    this->log(LogPriority(INFO), "Producer exit");
}

void HyperDistributorDemo::consumer(HyperDistributor *hd, std::string id) {
    std::ostringstream startInfo;
    startInfo << "consumer " << id << " start" << std::endl;
    log(LogPriority(INFO), startInfo.str());

    Node* node = hd->get();
    // main loop exit when program exit
    while(this->isProducerRunning || node != nullptr) {
        // fd loop exit when fd has NO event to deal
        while(node != nullptr && !node->casStatusAndEvents(NODE_SAE_WAIT_EVENT, NODE_SAE_AFTER_DEAL)) {
            SAE_BITS sae = node->getStatusAndEvents();
            while(!node->casStatusAndEvents(NODE_CLEAR_ALL_EVENTS(NODE_SET_STATUS(sae, NODE_STATUS_CONSUMER)), sae)) {
                sae = node->getStatusAndEvents();
            }

            SAE_EVENTS events = NODE_EVENTS(sae);
            int fd = node->getFd();
            Handler h = this->handlers[fd];
            if(h != nullptr) {
                h(events, node->getFd(), node->getValP());
            }
        }
        node = hd->get();
    }

    std::ostringstream exitInfo;
    exitInfo << "consumer " << id << " exit";

    log(LogPriority(INFO), exitInfo.str());
}

bool HyperDistributorDemo::ctlHandler(int fd, Handler handler, HD_CTL_TYPE ctl) {
    bool r = false;
    switch (ctl) {
        case HD_CTL_ADD:
            if(nullptr == handlers[fd]) {
                handlers[fd] = handler;
                r = true;
            }
            break;
        case HD_CTL_REMOVE:
            handlers[fd] = nullptr;
            r = true;
            break;
        case HD_CTL_OVERWRITE:
            handlers[fd] = handler;
            r = true;
            break;
        default:
            r = false;
    }
    return r;
}

/**
 * 随机生成一个fd
 * @return
 */
unsigned int HyperDistributorDemo::randomFd() {
    return this->fdsIdxRandomUniform(this->randomEngine);
}

/**
 * 从ACTIVE_EVENTS_SET中随机生成一个event
 * @return
 */
SAE_EVENT HyperDistributorDemo::randomEvent() {
    return ACTIVE_EVENTS_SET[this->eventsIdxRandomUniform(this->randomEngine)];
}

void HyperDistributorDemo::checkEventsCntMap() {
    int fd = FD_START;
    std::ostringstream oss, oss_falseFds;
    int cnt = 0;
    while(fd <= FD_END) {
        if(this->eventsCntMap[fd] == false) {
            if(cnt == 0) {
                oss_falseFds << fd;
            } else {
                oss_falseFds << "," << fd;
            }
            cnt++;
        }
        fd++;
    }
    oss << cnt << " fds has no event happen. They are: " << oss_falseFds.str();
    log(LogPriority(INFO), oss.str());
}
