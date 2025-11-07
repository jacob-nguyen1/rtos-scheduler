#include "BinaryHeapScheduler.h"

void BinaryHeapScheduler::insertJob(Job* job) {
    readyJobs.push(job);
}

Job* BinaryHeapScheduler::getNextJob() {
    if (readyJobs.empty()) return nullptr;
    Job* job = readyJobs.top();
    readyJobs.pop();
    return job;
}

bool BinaryHeapScheduler::shouldPreempt(Job* running)  {
    return !readyJobs.empty() && running->priority > readyJobs.top()->priority;
}

bool BinaryHeapScheduler::isEmpty() {
    return readyJobs.empty();
}

int BinaryHeapScheduler::size() {
    return readyJobs.size();
}

std::string BinaryHeapScheduler::getName() {
    return "Binary Heap Scheduler";
}
