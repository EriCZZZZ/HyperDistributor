//
// Created by eric on 18-11-1.
//

#ifndef HYPER_DISTRIBUTOR_DEMO_TASK_H
#define HYPER_DISTRIBUTOR_DEMO_TASK_H


class Task {
public:
    Task(void* data);
    ~Task();
private:
    void *data;
};


#endif //HYPER_DISTRIBUTOR_DEMO_TASK_H
