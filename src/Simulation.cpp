    #include "Simulation.h"
    #include <iostream>

    #include "Executable.h"

    void simulate(Scheduler& scheduler,
                std::vector<Job> allJobs,
                int simTime,
                Renderer* renderer)
    {
        int nextJobIdx = 0;
        std::vector<Job> completedJobs;
        int missedDeadlines = 0;
        std::vector<int> missedDeadlinePriorities(8, 0);

        Job* runningJob = nullptr;
        int jobStartTime = -1;

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
                    runningJob = nullptr;
                    jobStartTime = -1;
                }
            }
            for (auto& job : allJobs) {
                if (job.state == JobState::COMPLETED && job.fadeTimer < 1.0f) {
                    job.fadeTimer += 0.05f; // adjust speed: 0.05 = slower fade
                }
            }

            
            if (renderer && renderer->isOpen()) {
                renderer->renderLive(allJobs, t);
                sf::sleep(sf::milliseconds(600));
            }
        }
        std::cout << "\n>>> " << scheduler.getName() << " <<<\n";
        std::cout << "Completed: " << completedJobs.size() << " jobs\n";
        std::cout << "Total missed deadlines: " << missedDeadlines << "\n";

        if (missedDeadlines > 0) {
            for (int i = 0; i < 8; i++) {
                std::cout << "\tPriority " << i << ": " << missedDeadlinePriorities[i] << "\n";
            }
        }
        std::cout << "\n";
    }

