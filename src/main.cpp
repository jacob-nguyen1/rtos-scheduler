#include <iostream>
#include "Simulation.h"
#include "Scheduler.h"
#include "FCFSScheduler.h"
#include "PriorityArrayScheduler.h"
#include "Job.h"
#include "RoundRobinScheduler.h"

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

int main(int argc, char **argv) {
    // test visuals
    RoundRobinScheduler rr;
    Renderer visuals(1200, 800);
    std::vector<Job> allJobs = generateJobs(200);
    simulate(rr, allJobs, 200, &visuals);

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

        // use command arguments to decide which schedulers to use.
        break;
    }
    default:
        break;
    }   
    std::cout << "Simulation complete.\n";
    return 0;   
}

