#pragma once
#include <string>
#include <vector>

enum class JobState {
    WAITING,
    READY,
    RUNNING,
    COMPLETED
};

struct Job {
    int jobID;
    std::string task;
    int priority;        // 0-7(?) where 0 is highest priority
    int executionTime;   // total time required to execute the task
    int arrivalTime;     // timestamp for when the task is ready
    int remainingTime;   // remaining time for executing the task
    int deadline;        // timestamp for when the task must complete by
    int completionTime;  // timestamp for when the task is completed (-1 if not completed)
    bool missedDeadline; // boolean for if a completed task missed its deadline

    //for visuals
    JobState state;
    float fadeTimer = 0.0f;
    float visualX = 0.0f;
    float visualY = 0.0f;
    int readyAge = 0;
    
    Job(int id, std::string task, int priority, int executionTime, int arrivalTime, int deadlineWindow);

    void markCompleted(int currentTime);
    float progress() const;
};

std::vector<Job> generateJobs(int simTime);