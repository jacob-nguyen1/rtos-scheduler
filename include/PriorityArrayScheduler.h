#pragma once

#include "Scheduler.h"
#include <queue>

static const int NUM_PRIORITIES = 8;

class PriorityArrayScheduler : public Scheduler {
private:
    std::queue<Job*> readyJobs[NUM_PRIORITIES];
    int highestPriority = -1;
    int numJobs = 0;
    
public:
    void insertJob(Job* job) override;
    Job* getNextJob() override;
    bool shouldPreempt(Job* running) override;
    bool isEmpty() override;
    int size() override;
    std::string getName() override;
};