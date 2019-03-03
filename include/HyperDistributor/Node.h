//
// Created by eric on 19-1-8.
//

#ifndef HYPER_DISTRIBUTOR_NODE_H
#define HYPER_DISTRIBUTOR_NODE_H

#include "utils/SAE.h"

#ifndef TASK_LOCK_FREE
#include <mutex>
#endif

namespace hd {
    class Node {
    public:
        Node();
        explicit Node(int fd, void* p_val, SAE_BITS sae);
        ~Node();

        /* Node operations */
        /* !!! only Deque can use this !!! */
        void setNextNode(Node* next);
        Node* getNextNode();
        void setPreNode(Node* pre);
        Node* getPreNode();

        /* value */
        void* getValP();
        void* setValP(void *p_val);
        int getFd();

        /* Node status and events */
        SAE_BITS getStatusAndEvents();
        bool casStatusAndEvents(SAE_BITS newSAE, SAE_BITS oldSAE);
    private:
        /* Deque Node */
        Node* next;
        Node* pre;

        /* Distributor node status */
        int fd;
        SAE_BITS sae;

        /* user val */
        void* p_val;

        /* functions */
        /* value */
        // 并发安全版本
        void* ms_getValP();
        void* ms_setValP(void* p_val);

#ifndef TASK_LOCK_FREE
        std::mutex mutex;
#endif
    };
}

#endif /* HYPER_DISTRIBUTOR_NODE_H */

