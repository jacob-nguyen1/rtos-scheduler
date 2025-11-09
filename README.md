# RTOS Scheduling Suite

This is a CLI application used to analyze the performance of different scheduling algorithms.

We focused on comparing preemptive algorithms as those are primarily used in Real Time Operating Systems.

Included are:
 - Binary Heap Scheduler
 - Priority Array Scheduler
 - Round Robin Scheduler
 - Shortest Job First Scheduler
 - First Come First Serve Scheduler (non-preemptive)


### Compiling

This project depends on SFML 2.6.2 and uses the C++ 14 standard.

Compiling should look the same across MacOS, Linux and Windows.

Clone the project and once you have changed into its directory.

Simply run,
```
cmake -S . -B <build-dir>
cmake --build <build-dir>
```

Provided SFML is installed, it will compile.

### Usage

The CLI allows for basic comparison by using the `-compare` command.

`rtosSuite -compare fcfs pa` will compare `fcfs` and `pa` and spit out the metrics of each to be compared.

The compare command can also be run with different flags.

`rtosSuite -compare <first> <second> --simTime <time> --timeStep <step> [--render]`

Every flag is optional but changes how the simulation is run.

- The simulation time is default `100000 ms`.
- The time step is default off with a value of `0 ms` and affects how often updates are printed if they are.
- The render flag is default off and if on displays two windows showing off a visual of what the scheduling algorithm is doing.

There is also `-list` to list the different schedulers and `-help` to display a help prompt.

### Resources
 - https://www.geeksforgeeks.org/dsa/first-come-first-serve-cpu-scheduling-non-preemptive/
 - https://www.geeksforgeeks.org/dsa/program-for-shortest-job-first-or-sjf-cpu-scheduling-set-1-non-preemptive/
 - https://www.geeksforgeeks.org/operating-systems/round-robin-scheduling-in-operating-system/

