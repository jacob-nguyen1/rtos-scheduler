#include "Job.h"

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