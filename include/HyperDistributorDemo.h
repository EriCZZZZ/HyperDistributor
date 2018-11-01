//
// Created by eric on 18-10-30.
//

#ifndef HYPER_DISTRIBUTOR_DEMO_HYPERDISTRIBUTORDEMO_H
#define HYPER_DISTRIBUTOR_DEMO_HYPERDISTRIBUTORDEMO_H

#define LOG_APPENDER_NAME "DEMO"

#include <string>

#include <utils/LogPriority.h>

class HyperDistributorDemo
{
public:
    HyperDistributorDemo();
    ~HyperDistributorDemo();

    void display();

private:
    void log(LogPriority priority, std::string s);
    void initLog();


};
#endif //HYPER_DISTRIBUTOR_DEMO_HYPERDISTRIBUTORDEMO_H
