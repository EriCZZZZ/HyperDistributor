//
// Created by eric on 19-1-8.
//

#include "HyperDistributor/Node.h"

using namespace hd;

Node::Node() {
    pre = nullptr;
    next = nullptr;
    p_val = nullptr;
};

Node::Node(void* p_val) {
    this->p_val = p_val;
}

Node::~Node() = default;

Node* Node::getNextNode() {
    return this->next;
}

void Node::setNextNode(Node *next) {
    this->next = next;
}

Node* Node::getPreNode() {
    return this->pre;
}

void Node::setPreNode(Node *pre) {
    this->pre = pre;
}

void* Node::getValP() {
    return p_val;
}

void Node::setValP(void *p_val) {
    this->p_val = p_val;
}
