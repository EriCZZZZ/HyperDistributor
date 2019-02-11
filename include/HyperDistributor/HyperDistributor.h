//
// Created by eric on 18-10-30.
//

#ifndef HYPER_DISTRIBUTOR_H
#define HYPER_DISTRIBUTOR_H

#include <utils/LogPriority.h>
#include <string>
#include "HyperDistributor/Deque.h"

namespace hd {

    class HyperDistributor
    {
    public:
        HyperDistributor();

        explicit HyperDistributor(std::string instanceName);
        ~HyperDistributor();

        Node* get();
        void append(Node* node);

        std::string status();
    private:
        void init(std::string instanceName);
        void initLog();
        void log(LogPriority priority, std::string s);

        Deque* deque;

        std::string instanceName;
    };

}



#endif /* HYPER_DISTRIBUTOR_H */
