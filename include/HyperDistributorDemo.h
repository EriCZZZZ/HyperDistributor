//
// Created by eric on 18-10-30.
//

#ifndef HYPER_DISTRIBUTOR_DEMO_HYPERDISTRIBUTORDEMO_H
#define HYPER_DISTRIBUTOR_DEMO_HYPERDISTRIBUTORDEMO_H

#include <string>

#include <LogPriority.h>

class HyperDistributorDemo
{
public:
    HyperDistributorDemo();
    ~HyperDistributorDemo();

    void log(LogPriority priority, std::string s);
private:
    void initLog();


};
#endif //HYPER_DISTRIBUTOR_DEMO_HYPERDISTRIBUTORDEMO_H
