//
// Created by eric on 18-10-30.
//

#ifndef HYPER_DISTRIBUTOR_DEMO_HYPERDISTRIBUTORDEMO_H
#define HYPER_DISTRIBUTOR_DEMO_HYPERDISTRIBUTORDEMO_H

#define LOG_APPENDER_NAME "DEMO"

#include <string>
#include <map>
#include <vector>
#include <random>

#include <utils/LogPriority.h>

#include "HyperDistributor/HyperDistributor.h"

typedef void (*Handler)(SAE_EVENTS events, int fd, void* p_val);
typedef int HD_CTL_TYPE;
#define HD_CTL_ADD 1
#define HD_CTL_REMOVE 2
#define HD_CTL_OVERWRITE 3

using namespace hd;

class HyperDistributorDemo
{
public:
    HyperDistributorDemo();
    ~HyperDistributorDemo();

    /**
     * 演示
     */
    void display();
    /**
     * do benchmark
     */
    void benchmark();

private:
    void log(LogPriority priority, std::string s);
    void initLog();

    void registerFds(HyperDistributor *hd, Handler defaultHandler, std::map<int, Handler> *handlers);
    void producer(HyperDistributor *hd);
    void consumer(HyperDistributor* hd, std::string);

    bool ctlHandler(int fd, Handler handler, HD_CTL_TYPE ctl);
    unsigned int randomFd();
    SAE_EVENT randomEvent();
    void checkEventsCntMap();
    void setFdEventFlag(int fd);

    std::map<int, Handler> handlers;
    std::default_random_engine randomEngine;
    std::uniform_int_distribution<unsigned> fdsIdxRandomUniform = std::uniform_int_distribution<unsigned>(FD_START, FD_END);
    std::uniform_int_distribution<unsigned> eventsIdxRandomUniform = std::uniform_int_distribution<unsigned>(0, static_cast<unsigned int>(ACTIVE_EVENTS_SET_LEN));

    bool isProducerRunning;
    std::map<int, bool> eventsCntMap;
    std::mutex mtx_ecm;
};
#endif //HYPER_DISTRIBUTOR_DEMO_HYPERDISTRIBUTORDEMO_H
