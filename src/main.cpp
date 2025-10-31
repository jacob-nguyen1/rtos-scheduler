#include <iostream>

#include "Scheduler.h"
#include "FCFSScheduler.h"
#include "PriorityArrayScheduler.h"
#include "Job.h"

void helpPrompt() {
    std::cout << "Welcome to rtosSuite - this is a program to compare different scheduling algorithms for Real Time Operating Systems\n"
                 "Commands:\n"
                 "\t-compare {x} {y} - \n"
                 "Flags:\n"
                 "\t--render - signals that a window will be opened to visualize the completion and processing of jobs.\n"
                 "\t--help - shows this\n"
                 "\t--simTime {value: int} - modifies the default simulation time to desired value\n";
}

enum CommandType {
    NONE = -1,
    COMPARE = 0,
};

struct Command {
    CommandType typ;
    union {
        struct { int x, y; } compare;
    } args;
};

struct Settings {
    int simulationTime = 1000000;
    bool render = false;

    Command command = Command { NONE };
};

void simulate(Scheduler &scheduler, std::vector<Job> allJobs, int simTime) {
    int nextJobIdx = 0;
    std::vector<Job> completedJobs;

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
    
}

int main(int argc, char **argv) {
    Settings settings;

    if (argc == 1) {
        helpPrompt();
        return 0;
    }

    bool commandFound = false;

    for (int i = 1; i < argc; i++) { // ignore first arg (executable name)
        if (!commandFound) {
            if (strncmp(argv[i], "-compare", 9) == 0) {
                if (i + 2 >= argc) return -1;

                // TODO: Sanitize inputs if two ints aren't supplied

                settings.command.typ = CommandType::COMPARE;
                settings.command.args.compare.x = std::atoi(argv[i + 1]);
                settings.command.args.compare.y = std::atoi(argv[i + 2]);
            }
        }

        if (strncmp(argv[i], "--help", 7) == 0) {
            helpPrompt();
            return 0;
        } else if(strncmp(argv[i], "--simTime", 10) == 0) {
            if (i + 1 >= argc) return -1;
            int simTime = std::atoi(argv[i + 1]);

            settings.simulationTime = simTime;

            i++;
        }
    }
    
    switch (settings.command.typ)
    {
    case CommandType::NONE:
        break;
    case CommandType::COMPARE:
    {
        std::cout << "Comparing schedulers...\n";
        std::vector<Job> allJobs = generateJobs(settings.simulationTime);

        // use command arguments to decide which schedulers to use.

        FCFSScheduler fcfs;
        PriorityArrayScheduler pa;
        simulate(fcfs, allJobs, settings.simulationTime);
        simulate(pa, allJobs, settings.simulationTime);
        break;
    }
    default:
        break;
    }

    return 0;   
}