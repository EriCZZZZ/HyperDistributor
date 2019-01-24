//
// Created by eric on 19-1-24.
//

#ifndef HYPER_DISTRIBUTOR_THREAD_SAFE_USE_LOCK_DEQUE_H
#define HYPER_DISTRIBUTOR_THREAD_SAFE_USE_LOCK_DEQUE_H

#include <mutex>
#include "HyperDistributor/Deque.h"

namespace hd {

    class ThreadSafeUseLockDeque: public Deque {
    public:
        ThreadSafeUseLockDeque();
        ~ThreadSafeUseLockDeque() override;

        Node* dequeue() override;
        void append(Node* node) override;
    private:
        Node* head;
        Node* tail;

        std::mutex mutex;
    };
}

#endif //HYPER_DISTRIBUTOR_THREAD_SAFE_USE_LOCK_DEQUE_H
