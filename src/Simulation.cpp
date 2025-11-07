#include "Simulation.h"
#include "Executable.h"

#include <iostream>
#include <chrono>

void simulate(Scheduler& scheduler,
            std::vector<Job> allJobs,
            int simTime,
            Renderer* renderer)
{
    int nextJobIdx = 0;
    std::vector<Job> completedJobs;
    int missedDeadlines = 0;
    std::vector<int> missedDeadlinePriorities(8, 0);
    std::vector<int> turnaroundByPriority(8, 0);
    std::vector<int> waitingByPriority(8, 0);
    std::vector<int> countByPriority(8, 0);
    Job* runningJob = nullptr;
    int jobStartTime = -1;

    auto start = std::chrono::steady_clock::now();
    for (int t = 0; t <= simTime; t++) {

        // insert newly arrived jobs
        while (nextJobIdx < allJobs.size() && allJobs[nextJobIdx].arrivalTime == t) {
            Job& newJob = allJobs[nextJobIdx++];
            newJob.state = JobState::READY;
            scheduler.insertJob(&newJob);
        }

        // check preemption
       if (runningJob && scheduler.shouldPreempt(runningJob)) {
            runningJob->state = JobState::READY;
            scheduler.insertJob(runningJob);
            runningJob = nullptr;
        }

        // if no job running, pick next
        if (!runningJob) {
            runningJob = scheduler.getNextJob();
            if (runningJob) {
                runningJob->state = JobState::RUNNING;
                jobStartTime = t;
            }
        }
        
        if (Executable::settings.timeStep != 0 && t % Executable::settings.timeStep == 0) {
            std::cout << scheduler.getName() << " | "
                    << "t=" << t << "ms: waiting=" << (allJobs.size() - nextJobIdx)
                    << " ready=" << scheduler.size()
                    << " running=" << (runningJob ? 1 : 0)
                    << " completed=" << completedJobs.size()
                    << " deadlines missed=" << missedDeadlines << std::endl;
        }

        // execute for one tick
        if (runningJob) {
            runningJob->remainingTime--;
            if (runningJob->remainingTime == 0) {
                runningJob->markCompleted(t + 1);
                completedJobs.push_back(*runningJob);
                if (runningJob->missedDeadline) {
                    missedDeadlines++;
                    missedDeadlinePriorities[runningJob->priority]++;
                }

                int turnaround = t+1-runningJob->arrivalTime;
                int waiting = turnaround - runningJob->executionTime;
                turnaroundByPriority[runningJob->priority] += turnaround;
                waitingByPriority[runningJob->priority] += waiting;
                countByPriority[runningJob->priority]++;

                runningJob = nullptr;
                jobStartTime = -1;
            }
        }
        
        if (renderer && renderer->isOpen()) {
            for (auto& job : allJobs) {
                if (job.state == JobState::COMPLETED && job.fadeTimer < 1.0f) {
                    job.fadeTimer += 0.05f; // adjust speed: 0.05 = slower fade
                }
            }
            renderer->renderLive(allJobs, t);
            sf::sleep(sf::milliseconds(1));
        }
    }
    auto end = std::chrono::steady_clock::now();
    auto elapsedWallClockTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    if (!renderer) {
        int totalTurnaround = 0, totalWaiting = 0;
        for (int i = 0; i < 8; i++) {
            totalTurnaround += turnaroundByPriority[i];
            totalWaiting += waitingByPriority[i];
        }
        
        std::cout << "===============================================\n";
        std::cout << "\n>>> " << scheduler.getName() << " <<<\n";
        std::cout << "Completed: " << completedJobs.size() << " jobs\n\n";
        
        std::cout << "Total missed deadlines: " << missedDeadlines << "\n";
        std::cout << "Average turnaround time: " << (totalTurnaround / completedJobs.size()) << "ms\n";
        std::cout << "Average waiting time: " << (totalWaiting / completedJobs.size()) << "ms\n\n";

        std::cout << "Elapsed wall clock time: " << elapsedWallClockTime.count() << "ms\n\n";
        
        std::cout << "Missed deadlines by priority:\n";
        for (int i = 0; i < 8; i++) {
            std::cout << "\tPriority " << i << ": " << missedDeadlinePriorities[i] << "\n";
        }
        std::cout << "\nAverage turnaround time by priority:\n";
        for (int i = 0; i < 8; i++) {
            if (countByPriority[i] > 0) {
                std::cout << "\tPriority " << i << ": " << (turnaroundByPriority[i] / countByPriority[i]) << "ms\n";
            } else {
                std::cout << "\tPriority " << i << ": " << "N/A\n";
            }
        }
        std::cout << "\nAverage waiting time by priority:\n";
        for (int i = 0; i < 8; i++) {
            if (countByPriority[i] > 0) {
                std::cout << "\tPriority " << i << ": " << (waitingByPriority[i] / countByPriority[i]) << "ms\n";
            } else {
                std::cout << "\tPriority " << i << ": " << "N/A\n";
            }
        }
        std::cout << "\n";
    }
    
}