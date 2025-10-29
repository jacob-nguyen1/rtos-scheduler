#include "Scheduler.h"
#include <queue>

static const int NUM_PRIORITIES = 8;

class PriorityArrayScheduler : public Scheduler {
private:
    std::queue<Job*> readyJobs[NUM_PRIORITIES];
    int highestPriority = -1;
    int numJobs = 0;
    
public:
    void insertJob(Job* job) override {
        readyJobs[job->priority].push(job);
        numJobs++;

        if (highestPriority == -1) {
            highestPriority = job->priority;
        } else {
            highestPriority = std::min(job->priority, highestPriority);
        }
    }
    
    Job* getNextJob() override {
        if (numJobs == 0) return nullptr;

        Job* job = readyJobs[highestPriority].front();
        readyJobs[highestPriority].pop();
        numJobs--;

        // if no more jobs, set curr priority to -1
        if (numJobs == 0) highestPriority = -1;
        // if no more jobs at curr priority, find next highest priority
        else if (readyJobs[highestPriority].size() == 0) {
            for (int i = 0; i < NUM_PRIORITIES; i++) {
                if (readyJobs[i].size() != 0) {
                    highestPriority = i;
                    break;
                }
            }
        }

        return job;
    }

    bool shouldPreempt(Job* running) override {
        return numJobs > 0 && highestPriority < running->priority;
    }
    
    bool isEmpty() override {
        return numJobs == 0;
    }
    
    int size() override {
        return numJobs;
    }
};