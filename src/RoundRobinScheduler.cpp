#include "RoundRobinScheduler.h"

RoundRobinScheduler::RoundRobinScheduler(int q) : quantum(q), elapsedSlice(0), insertionCounter(0) {}
RoundRobinScheduler::RoundRobinScheduler() : quantum(6), elapsedSlice(0), insertionCounter(0) {}


void RoundRobinScheduler::insertJob(Job* job) {
    job->readyAge = ++insertionCounter;
    this->readyQueue.push(job);
}

Job* RoundRobinScheduler::getNextJob() {
    if (this->readyQueue.empty()) return nullptr;

    Job* job = this->readyQueue.front();
    this->readyQueue.pop();
    this->elapsedSlice = 0;
    return job;
}

bool RoundRobinScheduler::shouldPreempt(Job* running) {
    if (!running) return false;

    this->elapsedSlice++;
    if (this->elapsedSlice >= this->quantum) {
        this->elapsedSlice = 0;
        return true;
    }
    return false;
}

bool RoundRobinScheduler::isEmpty() {
    return this->readyQueue.empty();
}

int RoundRobinScheduler::size() {
    return this->readyQueue.size();
}

std::string RoundRobinScheduler::getName() {
    return "Round Robin Scheduler";
}