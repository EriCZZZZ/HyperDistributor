//
// Created by eric on 18-10-30.
//

#ifndef HYPER_DISTRIBUTOR_H
#define HYPER_DISTRIBUTOR_H

#include <string>
#include <map>
#include <mutex>

#include <utils/LogPriority.h>
#include "HyperDistributor/Deque.h"

namespace hd {

    class HyperDistributor
    {
    public:
        HyperDistributor();

        explicit HyperDistributor(std::string instanceName);
        ~HyperDistributor();

        Node* get();
        // todo move to private
        void append(Node* node);

        void schedule(Node* node);
        Node* getNodeByFd(int fd);

        std::string status();
    private:
        void init(std::string instanceName);
        void initLog();
        void log(LogPriority priority, std::string s);

        Deque* deque;

        // todo avl-tree to store all node
        // todo to find node when producer want to modify task
        std::map<int, Node*> map;

        std::mutex mtx_createNode;

        std::string instanceName;
    };

}

#endif /* HYPER_DISTRIBUTOR_H */
