//
// Created by eric on 19-1-8.
//

#ifndef HYPER_DISTRIBUTOR_DEMO_NODE_H
#define HYPER_DISTRIBUTOR_DEMO_NODE_H

#include <mutex>


class Node {
public:
    Node(Task *task);
    ~Node();

    Node* getNextNode();

private:
    Task *task;
    std::mutex mutex;
    Node* next;
};


#endif //HYPER_DISTRIBUTOR_DEMO_NODE_H
