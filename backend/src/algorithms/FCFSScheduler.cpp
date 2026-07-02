#include "FCFSScheduler.h"
#include <algorithm>

void FCFSScheduler::schedule() {
    if (processes.empty()) return;

    // Sort processes by arrival time (FCFS logic)
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        if (a.arrivalTime == b.arrivalTime) {
            return a.id < b.id; // Secondary sort by ID for stability
        }
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;
    
    // Clear previous results just in case
    ganttChart.clear();
    contextSwitches = 0;
    idleTime = 0;

    for (size_t i = 0; i < processes.size(); ++i) {
        Process& p = processes[i];

        // If CPU is idle before this process arrives
        if (currentTime < p.arrivalTime) {
            ExecutionInterval idleInterval{-1, currentTime, p.arrivalTime};
            ganttChart.push_back(idleInterval);
            idleTime += (p.arrivalTime - currentTime);
            currentTime = p.arrivalTime;
        }

        // Process execution
        p.startTime = currentTime;
        p.responseTime = p.startTime - p.arrivalTime;
        p.waitingTime = p.startTime - p.arrivalTime;
        
        ExecutionInterval interval{p.id, currentTime, currentTime + p.burstTime};
        ganttChart.push_back(interval);
        
        currentTime += p.burstTime;
        p.completionTime = currentTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.remainingTime = 0;
        
        // Context switch happens when switching to a different process
        if (i > 0) {
            contextSwitches++;
        }
    }

    totalExecutionTime = currentTime;
    calculateStatistics();
}

std::string FCFSScheduler::getName() const {
    return "First Come First Serve (FCFS)";
}
