#pragma once
#include "Scheduler.h"
#include <queue>

class RoundRobinScheduler : public Scheduler {
    std::queue<Job*> readyQueue;
    int quantum;
    int elapsedSlice;
    int insertionCounter;

public: 
    RoundRobinScheduler(int q);
    RoundRobinScheduler();

    void insertJob(Job* job) override;
    Job* getNextJob() override;
    bool shouldPreempt(Job* running) override;
    bool isEmpty() override;
    int size() override;
};