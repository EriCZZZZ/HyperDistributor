//
// Created by eric on 19-1-24.
//

#ifndef HYPER_DISTRIBUTOR_DEQUE_H
#define HYPER_DISTRIBUTOR_DEQUE_H

#include "Node.h"

namespace hd {
    class Deque {
    public:
        virtual ~Deque() = default;
        virtual void append(Node * node) = 0;
        virtual Node* dequeue() = 0;
        virtual std::string allNodeToString() = 0;
    };
}

#endif //HYPER_DISTRIBUTOR_DEQUE_H
