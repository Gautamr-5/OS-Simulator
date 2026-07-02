#include "Scheduler.h"

Scheduler::Scheduler()
    : averageWaitingTime(0.0), averageTurnaroundTime(0.0), averageResponseTime(0.0),
      cpuUtilization(0.0), throughput(0.0), totalExecutionTime(0), idleTime(0), contextSwitches(0) {}

void Scheduler::addProcess(const Process& p) {
    processes.push_back(p);
}

void Scheduler::calculateStatistics() {
    if (processes.empty()) return;

    double totalWaitingTime = 0;
    double totalTurnaroundTime = 0;
    double totalResponseTime = 0;

    for (const auto& p : processes) {
        totalWaitingTime += p.waitingTime;
        totalTurnaroundTime += p.turnaroundTime;
        totalResponseTime += p.responseTime;
    }

    averageWaitingTime = totalWaitingTime / processes.size();
    averageTurnaroundTime = totalTurnaroundTime / processes.size();
    averageResponseTime = totalResponseTime / processes.size();

    if (totalExecutionTime > 0) {
        cpuUtilization = ((double)(totalExecutionTime - idleTime) / totalExecutionTime) * 100.0;
        throughput = (double)processes.size() / totalExecutionTime;
    }
}

const std::vector<Process>& Scheduler::getProcesses() const {
    return processes;
}

const std::vector<ExecutionInterval>& Scheduler::getGanttChart() const {
    return ganttChart;
}

double Scheduler::getAverageWaitingTime() const { return averageWaitingTime; }
double Scheduler::getAverageTurnaroundTime() const { return averageTurnaroundTime; }
double Scheduler::getAverageResponseTime() const { return averageResponseTime; }
double Scheduler::getCpuUtilization() const { return cpuUtilization; }
double Scheduler::getThroughput() const { return throughput; }
int Scheduler::getContextSwitches() const { return contextSwitches; }
