#include "Scheduler.h"
#include <queue>

class FCFSScheduler : public Scheduler {
private:
    std::queue<Job*> readyJobs;
    
public:
    void insertJob(Job* job) override {
        readyJobs.push(job);
    }
    
    Job* getNextJob() override {
        if (readyJobs.empty()) return nullptr;
        Job* job = readyJobs.front();
        readyJobs.pop();
        return job;
    }
    
    bool isEmpty() override {
        return readyJobs.empty();
    }
    
    int size() override {
        return readyJobs.size();
    }
};