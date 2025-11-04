
#include <string>
#include <unistd.h> // For readlink on Linux
#include <limits.h> // For PATH_MAX on Linux

#ifdef __APPLE__
#include <mach-o/dyld.h> // For _NSGetExecutablePath on macOS
#elif _WIN32
#include <windows.h>
#endif

namespace Executable {

std::string getPath() {
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

static const std::string path = getPath();

}