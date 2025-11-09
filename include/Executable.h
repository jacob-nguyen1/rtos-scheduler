#pragma once

#include "Scheduler.h"

#include <string>
#include <iostream>

#ifdef __APPLE__
#include <mach-o/dyld.h> // For _NSGetExecutablePath on macOS
#elif _WIN32
#include <windows.h>
#elif __linux__
#include <unistd.h> // For readlink on Linux
#include <limits.h> // For PATH_MAX on Linux
#endif

namespace Executable {

enum CommandType {
    NONE = -1,
    COMPARE = 0,
    LIST,
    HELP,
};

struct Command {
    CommandType typ;
    union {
        struct { SchedulerType x, y; } compare;
    } args;
};

struct Settings {
    int simulationTime = 100000;
    int timeStep = 0;
    bool render = false;

    Command command = Command { NONE };
};

extern Settings settings; // global settings

std::string getPath();

void helpPrompt();
void error(const char* message);

}