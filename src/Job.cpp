#include "Job.h"
#include <random>

Job::Job(int id, std::string task, int priority, int executionTime, int arrivalTime, int deadlineWindow) :
    jobID(id),
    task(task),
    priority(priority),
    executionTime(executionTime),
    arrivalTime(arrivalTime),
    remainingTime(executionTime),
    deadline(arrivalTime + deadlineWindow),
    completionTime(-1),
    missedDeadline(false) {}

void Job::markCompleted(int currentTime) {
    completionTime = currentTime + 1;
    missedDeadline = (completionTime > deadline);
}

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
        if (t % 10 == 0) jobs.push_back(Job(id++, "ABS_Control", 0, 3, t, 10));
        if (t % 20 == 0) jobs.push_back(Job(id++, "BrakePressure", 1, 4, t, 20));
        if (t % 10 == 0) jobs.push_back(Job(id++, "FuelInjection", 2, 3, t, 10));
        if (t % 20 == 0) jobs.push_back(Job(id++, "ThrottleControl", 3, 4, t, 20));
        if (t % 50 == 0) jobs.push_back(Job(id++, "TransmissionCtrl", 4, 8, t, 50));
        if (t % 100 == 0) jobs.push_back(Job(id++, "DashboardUpdate", 5, 12, t, 100));
        if (t % 100 == 0) jobs.push_back(Job(id++, "RadioDisplay", 6, 15, t, 100));
        if (t % 1000 == 0) jobs.push_back(Job(id++, "DataLogging", 7, 40, t, 2000));
    }
    
    return jobs;
}