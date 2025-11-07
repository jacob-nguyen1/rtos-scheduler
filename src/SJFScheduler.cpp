#include "SJFScheduler.h"

void SJFScheduler::insertJob(Job* job) {
    this->readyJobs.push(job);
}

Job* SJFScheduler::getNextJob() {
    if (readyJobs.empty()) return nullptr;
    Job* job = readyJobs.top();
    readyJobs.pop();
    return job;
}

bool SJFScheduler::shouldPreempt(Job* running) {
    return !readyJobs.empty() && (running->remainingTime > readyJobs.top()->remainingTime);
}

bool SJFScheduler::isEmpty() {
    return readyJobs.empty();
}

int SJFScheduler::size() {
    return readyJobs.size();
}

std::string SJFScheduler::getName() {
    return "Shortest Job First Scheduler";
}