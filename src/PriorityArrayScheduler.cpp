#include "PriorityArrayScheduler.h"

void PriorityArrayScheduler::insertJob(Job* job) {
        readyJobs[job->priority].push(job);
        numJobs++;

        if (highestPriority == -1) {
            highestPriority = job->priority;
        } else {
            highestPriority = std::min(job->priority, highestPriority);
        }
}
    
Job* PriorityArrayScheduler::getNextJob() {
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

bool PriorityArrayScheduler::shouldPreempt(Job* running) {
    return numJobs > 0 && highestPriority < running->priority;
}

bool PriorityArrayScheduler::isEmpty() {
    return numJobs == 0;
}

int PriorityArrayScheduler::size() {
    return numJobs;
}

std::string PriorityArrayScheduler::getName() {
    return "Priority Array Scheduler";
}