#pragma once

#include "Scheduler.h"
#include <queue>

class BinaryHeapScheduler : public Scheduler {
private:
    struct CompareJobs {
        bool operator()(const Job* a, const Job* b) const {
            if (a->priority != b->priority)
                return a->priority > b->priority;
            return a->arrivalTime > b->arrivalTime;
        }
    };

    std::priority_queue<Job*, std::vector<Job*>, CompareJobs> readyJobs;
    
public:
    void insertJob(Job* job) override;
    Job* getNextJob() override;
    bool shouldPreempt(Job* running) override;
    bool isEmpty() override;
    int size() override;
    std::string getName() override;
};