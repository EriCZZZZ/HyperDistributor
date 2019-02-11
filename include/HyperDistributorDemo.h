//
// Created by eric on 18-10-30.
//

#ifndef HYPER_DISTRIBUTOR_DEMO_HYPERDISTRIBUTORDEMO_H
#define HYPER_DISTRIBUTOR_DEMO_HYPERDISTRIBUTORDEMO_H

#define LOG_APPENDER_NAME "DEMO"

#include <string>

#include <utils/LogPriority.h>

#include "HyperDistributor/HyperDistributor.h"

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

    void producerWithCount(HyperDistributor* hd, int cnt);
    void consumer(HyperDistributor* hd, std::string);
};
#endif //HYPER_DISTRIBUTOR_DEMO_HYPERDISTRIBUTORDEMO_H
