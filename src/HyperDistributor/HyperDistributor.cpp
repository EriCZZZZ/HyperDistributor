#include <utility>

//
// Created by eric on 18-10-30.
//

#include <iostream>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Category.hh>

#include "HyperDistributor/HyperDistributor.h"
#include "HyperDistributor/Deque.h"
#include "HyperDistributor/Node.h"

#include "utils/exceptions/NotImplementedException.h"

#ifdef DISTRIBUTOR_LOCK_FREE
#else /* DISTRIBUTOR_LOCK_FREE */
#include "HyperDistributor/ThreadSafeUseLockDeque.h"
#endif /* DISTRIBUTOR_LOCK_FREE */

using namespace hd;

HyperDistributor::HyperDistributor() {
    init("DefaultHyperDistributor");
}

HyperDistributor::HyperDistributor(std::string instanceName) {
    init(std::move(instanceName));
}

HyperDistributor::~HyperDistributor() {
    delete deque;
}

/**
 * task function
 */

Node* HyperDistributor::get() {
    return deque->dequeue();
}

void HyperDistributor::append(Node *node) {
    deque->append(node);
}

void HyperDistributor::schedule(Node* node) {
    if(node != nullptr) {
        SAE_BITS sae = node->getStatusAndEvents();
        while(!node->casStatusAndEvents(NODE_SET_STATUS(sae, NODE_STATUS_IN_QUEUE), sae)) {
            sae = node->getStatusAndEvents();
        }
        this->append(node);
    }
}

Node* HyperDistributor::getNodeByFd(int fd) {
    Node* r = this->map[fd];
    if(r == nullptr) {
        mtx_createNode.lock();
        if((r = this->map[fd]) == nullptr) {
            r = new Node(fd, nullptr, NODE_SAE_INIT);
            this->map[fd] = r;
        }
        mtx_createNode.unlock();
    }
    return r;
}

/**
 * monitor function
 */

std::string HyperDistributor::status() {
    std::ostringstream buf;
    buf << "Distributor status" << std::endl;
    buf << "====deque content====" << std::endl << deque->allNodeToString() << "====end====" << std::endl;
    buf << "Distributor status end";
    return buf.str();
}

/**
 * private function
 */

/**
 * 初始化
 * @param instanceName
 */
void HyperDistributor::init(std::string instanceName) {
    this->instanceName = std::move(instanceName);
    initLog();

    // init Deque
#ifdef DISTRIBUTOR_LOCK_FREE
#else /* DISTRIBUTOR_LOCK_FREE */
    this->deque = new ThreadSafeUseLockDeque();
#endif /* DISTRIBUTOR_LOCK_FREE */
    this->log(LogPriority(INFO), "Successfully init deque");

    this->log(LogPriority(INFO), "HyperDistributor init finish");
}

/**
 * 初始化日志相关
 */
void HyperDistributor::initLog() {
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
    log4cpp::Category::getInstance(this->instanceName).setPriority(log4cpp::Priority::INFO);
#endif
#endif
}

/**
 * 打印日志
 * @param priority
 * @param s
 */
void HyperDistributor::log(LogPriority priority, std::string s) {
#ifdef LOG_LIB_LOG4CPP
    log4cpp::Category::getInstance(this->instanceName).log(priority, s);
#else
    std::cout << priority << " " << s << std::endl;
#endif
}
