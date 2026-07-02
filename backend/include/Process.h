#ifndef PROCESS_H
#define PROCESS_H

#include <string>

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;

    // Execution state
    int remainingTime;
    int startTime;
    int completionTime;
    
    // Statistics
    int turnaroundTime;
    int waitingTime;
    int responseTime;

    // For visualization
    std::string color;

    // Constructor
    Process(int id, int arrivalTime, int burstTime, int priority = 0, std::string color = "#FFFFFF");

    // Reset process state (useful for running multiple algorithms on the same process set)
    void reset();
};

#endif // PROCESS_H
