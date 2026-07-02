#include "PreemptivePriorityScheduler.h"
#include <algorithm>
#include <vector>

void PreemptivePriorityScheduler::schedule() {
    if (processes.empty()) return;

    // Sort by arrival time initially
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        if (a.arrivalTime == b.arrivalTime) {
            return a.id < b.id; 
        }
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;
    int completed = 0;
    int n = processes.size();
    
    std::vector<bool> isCompleted(n, false);
    ganttChart.clear();
    contextSwitches = 0;
    idleTime = 0;
    int lastProcessId = -1;

    for(auto& p : processes) {
        p.remainingTime = p.burstTime;
        p.startTime = -1;
    }

    while (completed != n) {
        int idx = -1;
        int maxPriority = 1e9; // Lower number = higher priority

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && !isCompleted[i]) {
                if (processes[i].priority < maxPriority) {
                    maxPriority = processes[i].priority;
                    idx = i;
                } else if (processes[i].priority == maxPriority) {
                    if (idx == -1 || processes[i].arrivalTime < processes[idx].arrivalTime) {
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            Process& p = processes[idx];

            if (p.startTime == -1) {
                p.startTime = currentTime;
                p.responseTime = p.startTime - p.arrivalTime;
            }

            // Find next event (completion or higher priority process arrival)
            int nextEventTime = currentTime + p.remainingTime;
            for (int i = 0; i < n; i++) {
                if (processes[i].arrivalTime > currentTime && processes[i].arrivalTime < nextEventTime && !isCompleted[i]) {
                    if (processes[i].priority < p.priority) {
                        nextEventTime = processes[i].arrivalTime;
                        break;
                    }
                }
            }

            int executeTime = nextEventTime - currentTime;
            
            ExecutionInterval interval{p.id, currentTime, nextEventTime};
            if (!ganttChart.empty() && ganttChart.back().processId == p.id && ganttChart.back().endTime == currentTime) {
                ganttChart.back().endTime = nextEventTime;
            } else {
                ganttChart.push_back(interval);
                if (lastProcessId != -1 && lastProcessId != p.id) {
                    contextSwitches++;
                }
            }

            currentTime = nextEventTime;
            p.remainingTime -= executeTime;
            lastProcessId = p.id;

            if (p.remainingTime == 0) {
                p.completionTime = currentTime;
                p.turnaroundTime = p.completionTime - p.arrivalTime;
                p.waitingTime = p.turnaroundTime - p.burstTime;
                isCompleted[idx] = true;
                completed++;
            }
        } else {
            // Idle time
            int nextArrival = 1e9;
            for (int i = 0; i < n; i++) {
                if (!isCompleted[i] && processes[i].arrivalTime > currentTime) {
                    nextArrival = std::min(nextArrival, processes[i].arrivalTime);
                }
            }
            if (nextArrival != 1e9) {
                ExecutionInterval idleInterval{-1, currentTime, nextArrival};
                ganttChart.push_back(idleInterval);
                idleTime += (nextArrival - currentTime);
                currentTime = nextArrival;
            }
        }
    }

    totalExecutionTime = currentTime;
    calculateStatistics();
}

std::string PreemptivePriorityScheduler::getName() const {
    return "Preemptive Priority Scheduling";
}
