#pragma once
#include <string>

struct Job {
    int jobID;
    std::string task;
    int priority;        // 0-31 where 0 is highest priority (32 priorities is FreeRTOS standard)
    int executionTime;   // total time required to execute the task
    int arrivalTime;     // timestamp for when the task is ready
    int remainingTime;   // remaining time for executing the task
    int deadline;        // timestamp for when the task must complete by
    int completionTime;  // timestamp for when the task is completed (-1 if not completed)
    bool missedDeadline; // boolean for if a completed task missed its deadline
    
    Job(int id, std::string task, int priority, int executionTime, int arrivalTime, int deadlineWindow);
    void markCompleted(int currentTime);
};