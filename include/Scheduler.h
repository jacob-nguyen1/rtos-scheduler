#pragma once
#include "Job.h"

class Scheduler {
public:
    virtual void insertJob(Job* job) = 0;
    virtual Job* getNextJob() = 0;
    virtual bool shouldPreempt(Job* running) = 0;
    virtual bool isEmpty() = 0;
    virtual int size() = 0;
    virtual std::string getName() = 0;
    virtual ~Scheduler() = default;
};

enum SchedulerType {
    FirstComeFirstServe = 0,
    PriorityArray = 1,
    RoundRobin,
};