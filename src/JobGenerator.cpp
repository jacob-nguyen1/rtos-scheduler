#include "JobGenerator.h"
#include <random>

static std::random_device rd;
static std::mt19937 gen(rd());

static int randInt(int min, int max) {
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

std::vector<Job> generateJobs(int simTime) {
    std::vector<Job> jobs;
    int id = 0;
    
    for (int t = 0; t < simTime; t++) {
        // this is gonna change probably
        if (t % 10 == 0) jobs.push_back(Job(id++, "ABS_Control", 1, 3, t, 10));
        if (t % 20 == 0) jobs.push_back(Job(id++, "BrakePressure", 2, 4, t, 20));
        if (t % 10 == 0) jobs.push_back(Job(id++, "FuelInjection", 4, 3, t, 10));
        if (t % 20 == 0) jobs.push_back(Job(id++, "ThrottleControl", 5, 4, t, 20));
        if (t % 50 == 0) jobs.push_back(Job(id++, "TransmissionCtrl", 7, 8, t, 50));
        if (t % 100 == 0) jobs.push_back(Job(id++, "DashboardUpdate", 12, 12, t, 100));
        if (t % 100 == 0) jobs.push_back(Job(id++, "RadioDisplay", 15, 15, t, 100));
        if (t % 1000 == 0) jobs.push_back(Job(id++, "DataLogging", 15, 40, t, 2000));
    }
    
    return jobs;
}