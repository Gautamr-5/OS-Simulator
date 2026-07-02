#include "HRRNScheduler.h"
#include <algorithm>
#include <vector>

void HRRNScheduler::schedule() {
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

    while (completed != n) {
        int idx = -1;
        double maxResponseRatio = -1.0;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && !isCompleted[i]) {
                int waitTime = currentTime - processes[i].arrivalTime;
                double responseRatio = (double)(waitTime + processes[i].burstTime) / processes[i].burstTime;

                if (responseRatio > maxResponseRatio) {
                    maxResponseRatio = responseRatio;
                    idx = i;
                } else if (responseRatio == maxResponseRatio) {
                    if (idx == -1 || processes[i].arrivalTime < processes[idx].arrivalTime) {
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            Process& p = processes[idx];
            
            p.startTime = currentTime;
            p.responseTime = p.startTime - p.arrivalTime;
            p.waitingTime = p.startTime - p.arrivalTime;
            
            ExecutionInterval interval{p.id, currentTime, currentTime + p.burstTime};
            ganttChart.push_back(interval);
            
            currentTime += p.burstTime;
            p.completionTime = currentTime;
            p.turnaroundTime = p.completionTime - p.arrivalTime;
            p.remainingTime = 0;
            
            isCompleted[idx] = true;
            completed++;

            if (lastProcessId != -1 && lastProcessId != p.id) {
                contextSwitches++;
            }
            lastProcessId = p.id;
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

std::string HRRNScheduler::getName() const {
    return "Highest Response Ratio Next (HRRN)";
}
