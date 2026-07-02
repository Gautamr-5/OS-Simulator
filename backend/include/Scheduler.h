#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"
#include <vector>
#include <string>

// Represents an execution interval for the Gantt Chart
struct ExecutionInterval {
    int processId; // -1 can mean Idle
    int startTime;
    int endTime;
};

class Scheduler {
protected:
    std::vector<Process> processes;
    std::vector<ExecutionInterval> ganttChart;
    
    // Statistics
    double averageWaitingTime;
    double averageTurnaroundTime;
    double averageResponseTime;
    double cpuUtilization;
    double throughput;
    
    int totalExecutionTime;
    int idleTime;
    int contextSwitches;

    // Helper to calculate statistics after scheduling
    void calculateStatistics();

public:
    Scheduler();
    virtual ~Scheduler() = default;

    // Adds a process to the scheduler
    void addProcess(const Process& p);

    // Main pure virtual function to be implemented by specific algorithms
    virtual void schedule() = 0;

    // Returns the name of the scheduling algorithm
    virtual std::string getName() const = 0;

    // Getters for results
    const std::vector<Process>& getProcesses() const;
    const std::vector<ExecutionInterval>& getGanttChart() const;
    
    double getAverageWaitingTime() const;
    double getAverageTurnaroundTime() const;
    double getAverageResponseTime() const;
    double getCpuUtilization() const;
    double getThroughput() const;
    int getContextSwitches() const;
};

#endif // SCHEDULER_H
