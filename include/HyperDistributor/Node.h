//
// Created by eric on 19-1-8.
//

#ifndef HYPER_DISTRIBUTOR_NODE_H
#define HYPER_DISTRIBUTOR_NODE_H

namespace hd {
    class Node {
    public:
        Node();
        explicit Node(void* p_val);
        ~Node();

        /* Node options */
        void setNextNode(Node* next);
        Node* getNextNode();
        void setPreNode(Node* pre);
        Node* getPreNode();

        /* value */
        void* getValP();
        void setValP(void* p_val);
    private:
        Node* next;
        Node* pre;
        void* p_val;
    };
}



#endif /* HYPER_DISTRIBUTOR_NODE_H */
