#include <iostream>
#include "Simulation.h"
#include "Scheduler.h"
#include "FCFSScheduler.h"
#include "PriorityArrayScheduler.h"
#include "Job.h"
#include "RoundRobinScheduler.h"

#include "Executable.h"

using namespace Executable;

Scheduler* initSchedulerOf(SchedulerType type);

int main(int argc, char **argv) {

    std::unordered_map<std::string, SchedulerType> str_to_sched = {
        {"fcfs", SchedulerType::FirstComeFirstServe},
        {"pa", SchedulerType::PriorityArray},
        {"rr", SchedulerType::RoundRobin},
    };

    if (argc == 1) {
        helpPrompt();
        return 0;
    }

    bool commandFound = false;

    for (int i = 1; i < argc; i++) { // ignore first arg (executable name)
        if (!commandFound) {
            if (strncmp(argv[i], "-compare", 9) == 0) {
                if (i + 2 >= argc) return -1;

                if (str_to_sched.find(argv[i + 1]) == str_to_sched.end() || str_to_sched.find(argv[i + 2]) == str_to_sched.end())
                    error("Error: Cannot recognize one or both arguments for '-compare'.");

                settings.command.typ = CommandType::COMPARE;
                settings.command.args.compare.x = str_to_sched.at(argv[i + 1]);
                settings.command.args.compare.y = str_to_sched.at(argv[i + 2]);
            }
            else if (strncmp(argv[i], "-list", 6) == 0) {
                settings.command.typ = CommandType::LIST;
            }
            else if (strncmp(argv[i], "-help", 6) == 0) {
                settings.command.typ = CommandType::HELP;
                return 0;
            }
        }

        if(strncmp(argv[i], "--simTime", 10) == 0) {
            if (i + 1 >= argc) error("Error: missing argument for '--simTime'.");
            settings.simulationTime = std::atoi(argv[i + 1]);;

            i++;
        } else if (strncmp(argv[i], "--timeStep", 11) == 0) {
            if (i + 1 >= argc) error("Error: missing argument for '--timeStep'.");
            settings.timeStep = std::atoi(argv[i + 1]);

            i++;
        } else if (strncmp(argv[i], "--render", 9) == 0) {
            settings.render = true;
        }
    }
    
    switch (settings.command.typ)
    {
    case CommandType::NONE:
    case CommandType::HELP:
    {
        helpPrompt();
        break;
    }
    case CommandType::COMPARE:
    {
        std::cout << "Comparing schedulers...\n";

        Scheduler* scheduler1 = initSchedulerOf(settings.command.args.compare.x);
        Scheduler* scheduler2 = initSchedulerOf(settings.command.args.compare.y);

        std::vector<Job> allJobs = generateJobs(settings.simulationTime);

        if (settings.render) { // add multithreading so both renderers render at same time
            Renderer renderer1;
            Renderer renderer2;

            simulate(*scheduler1, allJobs, settings.simulationTime, &renderer1);
            simulate(*scheduler2, allJobs, settings.simulationTime, &renderer2);
        } else {
            simulate(*scheduler2, allJobs, settings.simulationTime);
            simulate(*scheduler1, allJobs, settings.simulationTime);
        }

        std::cout << "Simulation complete.\n";
        break;
    }
    case CommandType::LIST:
    {
        std::cout << "List of Available Schedulers to compare:\n"
                     "\tfcfs - First Come First Serve (non-preemptive)\n"
                     "\tpa - Priority Array (preemptive)\n"
                     "\trr - Round Robin (preemptive)\n";
        break;
    }
    }   
    return 0;   
}

Scheduler* initSchedulerOf(SchedulerType type) {
    switch (type) {
    case SchedulerType::FirstComeFirstServe:
    {
        return new FCFSScheduler;
    }
    case SchedulerType::PriorityArray:
    {
        return new PriorityArrayScheduler;
    }
    case SchedulerType::RoundRobin:
    {
        return new RoundRobinScheduler;
    }
    default:
    {
        error("Error: type of Scheduler is invalid.");
    }
    }
}

