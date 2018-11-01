//
// Created by eric on 18-10-30.
//

#ifndef HYPER_DISTRIBUTOR_HYPERDISTRIBUTOR_H
#define HYPER_DISTRIBUTOR_HYPERDISTRIBUTOR_H

#include "Task.h"

#include <utils/LogPriority.h>
#include <string>

namespace hd {

    class HyperDistributor
    {
    public:
        HyperDistributor();

        explicit HyperDistributor(std::string instanceName);
        ~HyperDistributor();

        void push(Task* task);
        void modify(Task* task, void* newData);
        Task* get();

    private:
        void init(std::string instanceName);
        void initLog();
        void log(LogPriority priority, std::string s);

        std::string instanceName;
    };

}



#endif //HYPER_DISTRIBUTOR_HYPERDISTRIBUTOR_H
