#pragma once
#include "Job.h"
#include "Scheduler.h"
#include "Visualization.h"

void simulate(Scheduler& scheduler,
              std::vector<Job>& allJobs,
              int simTime,
              Renderer* renderer = nullptr);
