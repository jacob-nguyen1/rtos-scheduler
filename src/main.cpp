#include <iostream>
#include "Simulation.h"
#include "Scheduler.h"
#include "FCFSScheduler.h"
#include "PriorityArrayScheduler.h"
#include "Job.h"
#include "RoundRobinScheduler.h"
#include "BinaryHeapScheduler.h"
#include "SJFScheduler.h"

#include "Executable.h"

#include <unordered_map>
#include <cstring>
#include <thread>
#include <mutex>
#include <atomic>

using namespace Executable;

Scheduler* initSchedulerOf(SchedulerType type);

int main(int argc, char **argv) {

    std::unordered_map<std::string, SchedulerType> str_to_sched = {
        {"fcfs", SchedulerType::FirstComeFirstServe},
        {"pa", SchedulerType::PriorityArray},
        {"rr", SchedulerType::RoundRobin},
        {"bh", SchedulerType::BinaryHeap},
        {"sjf", SchedulerType::ShortestJobFirst}
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
        std::vector<Job> allJobs2 = allJobs;

        if (settings.render) { // add multithreading so both renderers render at same time
            sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
            int width = desktop.width / 2;
            int height = desktop.height - 150;

            Renderer renderer1(*scheduler1, width, height);
            Renderer renderer2(*scheduler2, width, height);
            renderer1.setWindowPos("left");
            renderer2.setWindowPos("right");

            std::mutex simMutex;
            std::mutex simMutex2;

            int time1(0);
            int time2(0);

            std::atomic<bool> done1(false);
            std::atomic<bool> done2(false);

            std::thread t1([&] {
                std::lock_guard<std::mutex> lock(simMutex);
                simulate(*scheduler1, allJobs, Executable::settings.simulationTime, true, &time1);
                done1 = true;
            });
            std::thread t2([&] {
                std::lock_guard<std::mutex> lock(simMutex2);
                simulate(*scheduler2, allJobs2, Executable::settings.simulationTime, true, &time2);
                done2 = true;
            });

            while (!done1 || !done2) {
                for (auto& job : allJobs2) {
                    if (job.state == JobState::COMPLETED && job.fadeTimer < 1.0f) {
                        job.fadeTimer += 0.05f; // adjust speed: 0.05 = slower fade
                    }
                }
                for (auto& job : allJobs) {
                    if (job.state == JobState::COMPLETED && job.fadeTimer < 1.0f) {
                        job.fadeTimer += 0.05f; // adjust speed: 0.05 = slower fade
                    }
                }

                if (!(renderer1.renderLive(allJobs, time1) && renderer2.renderLive(allJobs2, time2))) {
                    if (renderer2.isOpen()) renderer2.close();
                    if (renderer1.isOpen()) renderer1.close();
                }

                sf::sleep(sf::milliseconds(600));
            }

            t1.join();
            t2.join();
        } else {
            simulate(*scheduler1, allJobs, settings.simulationTime);
            simulate(*scheduler2, allJobs2, settings.simulationTime);
        }

        break;
    }
    case CommandType::LIST:
    {
        std::cout << "List of Available Schedulers to compare:\n"
                     "\tfcfs - First Come First Serve (non-preemptive)\n"
                     "\tpa - Priority Array (preemptive)\n"
                     "\trr - Round Robin (preemptive)\n"
                     "\tbh - Binary Heap (preemptive)\n";
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
    case SchedulerType::BinaryHeap:
    {
        return new BinaryHeapScheduler;
    }
    case SchedulerType::ShortestJobFirst:
    {
        return new SJFScheduler;
    }
    default:
    {
        error("Error: type of Scheduler is invalid.");
        return nullptr;
    }
    }
}

