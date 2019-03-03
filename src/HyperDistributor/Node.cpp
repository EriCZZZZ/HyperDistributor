//
// Created by eric on 19-1-8.
//

#include <utils/exceptions/NotImplementedException.h>
#include "HyperDistributor/Node.h"

using namespace hd;

Node::Node() {
    pre = nullptr;
    next = nullptr;
    p_val = nullptr;
};

Node::Node(int fd, void* p_val, SAE_BITS sae) {
    this->fd = fd;
    this->p_val = p_val;
    this->sae = sae;
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
    return this->ms_getValP();
}

void* Node::setValP(void *p_val) {
    return this->ms_setValP(p_val);
}
int Node::getFd() {
    return this->fd;
}

SAE_BITS Node::getStatusAndEvents() {
#ifndef TASK_LOCK_FREE
    return this->sae;
#else
    return this->sae;
#endif
}

bool Node::casStatusAndEvents(SAE_BITS newSAE, SAE_BITS oldSAE) {
#ifndef TASK_LOCK_FREE
    bool modifyResult = false;
    this->mutex.lock();
    if(this->sae == oldSAE) {
        this->sae = newSAE;
        modifyResult = true;
    }
    this->mutex.unlock();
    return modifyResult;
#else
    NotImplementedException nie;
    throw nie;
#endif
}

void* Node::ms_getValP() {
#ifndef TASK_LOCK_FREE
    this->mutex.lock();
    void* tmp = this->p_val;
    this->mutex.unlock();
    return tmp;
#else
    NotImplementedException nie;
    throw nie;
#endif
}

void* Node::ms_setValP(void* p_val) {
#ifndef TASK_LOCK_FREE
    this->mutex.lock();
    void* tmp = this->p_val;
    this->p_val = p_val;
    this->mutex.unlock();
    return tmp;
#else
    NotImplementedException nie;
    throw nie;
#endif
}
