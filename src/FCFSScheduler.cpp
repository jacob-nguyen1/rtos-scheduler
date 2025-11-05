#include "FCFSScheduler.h"

void FCFSScheduler::insertJob(Job* job) {
    readyJobs.push(job);
}

Job* FCFSScheduler::getNextJob() {
    if (readyJobs.empty()) return nullptr;
    Job* job = readyJobs.front();
    readyJobs.pop();
    return job;
}

bool FCFSScheduler::shouldPreempt(Job* running)  {
    return false;
}

bool FCFSScheduler::isEmpty() {
    return readyJobs.empty();
}

int FCFSScheduler::size() {
    return readyJobs.size();
}

std::string FCFSScheduler::getName() {
    return "FCFS Scheduler";
}
