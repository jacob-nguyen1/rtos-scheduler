#include "Job.h"
#include "FCFSScheduler.h"
#include <iostream>
#include <vector>

int main() {
    int simTime = 1000000;
    std::vector<Job> allJobs = generateJobs(simTime);
    int nextJobIdx = 0;
    std::vector<Job> completedJobs;

    FCFSScheduler scheduler;
    Job* runningJob = nullptr;
    
    for (int t = 0; t <= simTime; t++) {
        // insert arrived jobs
        while (nextJobIdx < allJobs.size() && allJobs[nextJobIdx].arrivalTime == t) {
            scheduler.insertJob(&allJobs[nextJobIdx++]);
        }

        // clear running job if it should be preempted (i.e deemed lower priority by the scheduler)
        if (runningJob != nullptr && scheduler.shouldPreempt(runningJob)) {
            scheduler.insertJob(runningJob);
            runningJob = nullptr;
        }
        
        // set running job
        if (runningJob == nullptr) runningJob = scheduler.getNextJob();

        if (!(t%100000)) {
            std::cout << "t=" << t << "ms: waiting=" << (allJobs.size() - nextJobIdx)
                    << " ready=" << scheduler.size()
                    << " running=" << (runningJob != nullptr ? 1 : 0)
                    << " completed=" << completedJobs.size() << std::endl;
        }
        
        // execute job
        if (runningJob != nullptr) {
            runningJob->remainingTime--;
            
            // move completed jobs to completed jobs
            if (runningJob->remainingTime == 0) {
                runningJob->markCompleted(t);
                completedJobs.push_back(*runningJob);
                runningJob = nullptr;
            }
        }
    }
    
    return 0;
}