#include "Executable.h"

std::string Executable::getPath() {
    char buffer[PATH_MAX];
    #ifdef __linux__
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1) {
        buffer[len] = '\0';
        std::string::size_type pos = std::string(buffer).find_last_of("/");
        return std::string(buffer).substr(0, pos);
    }
    #elif __APPLE__
    uint32_t len = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &len) == 0) {
        std::string::size_type pos = std::string(buffer).find_last_of("/");
        return std::string(buffer).substr(0, pos);
    }
    #elif _WIN32
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
#endif
    return ""; // Handle error or unsupported OS
}

void Executable::helpPrompt() {
    std::cout << "Welcome to rtosSuite - this is a program to compare different scheduling algorithms for Real Time Operating Systems\n"
                 "Commands:\n"
                 "\t-compare {x} {y} - Compares two different scheduling algorithms. Choose by using acronyms found with '-list'.\n"
                 "\t-list - lists the available scheduling algorithms. \n"
                 "\t-help - shows this\n"
                 "Flags:\n"
                 "\t--render - signals that a window will be opened to visualize the completion and processing of jobs.\n"
                 "\t--simTime {value: int} - modifies the default simulation time to desired value. (default: 1000000)\n"
                 "\t--timeStep {value: int} - modifies the default timestep to desired value. If 0, no time step prints. (default: 1000)\n";
}

void Executable::error(const char* message) {
    std::cerr << message << std::endl;
    exit(-1);
}

Executable::Settings Executable::settings; // global settings