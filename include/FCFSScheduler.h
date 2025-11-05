#pragma once

#include "Scheduler.h"
#include <queue>

class FCFSScheduler : public Scheduler {
private:
    std::queue<Job*> readyJobs;
    
public:
    void insertJob(Job* job) override;
    Job* getNextJob() override;
    bool shouldPreempt(Job* running) override;
    bool isEmpty() override;
    int size() override;
    std::string getName() override;
};