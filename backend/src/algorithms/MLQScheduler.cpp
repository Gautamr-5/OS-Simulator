#include "MLQScheduler.h"
#include <algorithm>
#include <vector>

void MLQScheduler::schedule() {
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
    
    std::vector<bool> isInQueue(n, false);
    
    std::queue<int> q1; // Priority 1-2, RR(2)
    std::queue<int> q2; // Priority 3-4, RR(4)
    std::queue<int> q3; // Priority 5+, FCFS
    
    ganttChart.clear();
    contextSwitches = 0;
    idleTime = 0;
    int lastProcessId = -1;

    for(auto& p : processes) {
        p.remainingTime = p.burstTime;
        p.startTime = -1;
    }

    int currentProcessIndex = 0;
    
    auto addToCorrectQueue = [&](int idx) {
        if (processes[idx].priority <= 2) {
            q1.push(idx);
        } else if (processes[idx].priority <= 4) {
            q2.push(idx);
        } else {
            q3.push(idx);
        }
    };

    while (currentProcessIndex < n && processes[currentProcessIndex].arrivalTime <= currentTime) {
        addToCorrectQueue(currentProcessIndex);
        isInQueue[currentProcessIndex] = true;
        currentProcessIndex++;
    }

    while (completed != n) {
        int idx = -1;
        int maxExecuteTime = 1e9;
        bool isRR = false;
        
        if (!q1.empty()) {
            idx = q1.front(); q1.pop();
            maxExecuteTime = 2; // RR Q=2
            isRR = true;
        } else if (!q2.empty()) {
            idx = q2.front(); q2.pop();
            maxExecuteTime = 4; // RR Q=4
            isRR = true;
        } else if (!q3.empty()) {
            idx = q3.front(); q3.pop();
            maxExecuteTime = processes[idx].remainingTime; // FCFS
            isRR = false;
        }

        if (idx != -1) {
            Process& p = processes[idx];

            if (p.startTime == -1) {
                p.startTime = currentTime;
                p.responseTime = p.startTime - p.arrivalTime;
            }

            int executeTime = std::min(maxExecuteTime, p.remainingTime);
            
            // For FCFS (Queue 3) and RR, it might be preempted by a higher priority queue process arriving
            int actualExecuteTime = executeTime;
            for (int t = 1; t <= executeTime; t++) {
                int checkTime = currentTime + t;
                
                // Add any newly arrived processes
                bool higherPriorityArrived = false;
                while (currentProcessIndex < n && processes[currentProcessIndex].arrivalTime == checkTime) {
                    addToCorrectQueue(currentProcessIndex);
                    isInQueue[currentProcessIndex] = true;
                    
                    // Check if new arrival preempts current
                    if (processes[currentProcessIndex].priority <= 2 && p.priority > 2) {
                        higherPriorityArrived = true;
                    } else if (processes[currentProcessIndex].priority <= 4 && p.priority > 4) {
                        higherPriorityArrived = true;
                    }
                    
                    currentProcessIndex++;
                }
                
                if (higherPriorityArrived) {
                    actualExecuteTime = t;
                    break;
                }
            }
            
            ExecutionInterval interval{p.id, currentTime, currentTime + actualExecuteTime};
            if (!ganttChart.empty() && ganttChart.back().processId == p.id && ganttChart.back().endTime == currentTime) {
                ganttChart.back().endTime = currentTime + actualExecuteTime;
            } else {
                ganttChart.push_back(interval);
                if (lastProcessId != -1 && lastProcessId != p.id) {
                    contextSwitches++;
                }
            }

            currentTime += actualExecuteTime;
            p.remainingTime -= actualExecuteTime;
            lastProcessId = p.id;
            
            // Re-check for arrivals at exactly currentTime just in case loop missed it if actualExecuteTime == 0
            while (currentProcessIndex < n && processes[currentProcessIndex].arrivalTime <= currentTime && !isInQueue[currentProcessIndex]) {
                addToCorrectQueue(currentProcessIndex);
                isInQueue[currentProcessIndex] = true;
                currentProcessIndex++;
            }

            if (p.remainingTime > 0) {
                if (isRR || p.priority > 2) { // Need to go back to its queue
                    addToCorrectQueue(idx);
                } else {
                    q3.push(idx); // FCFS push back (shouldn't happen unless preempted)
                }
            } else {
                p.completionTime = currentTime;
                p.turnaroundTime = p.completionTime - p.arrivalTime;
                p.waitingTime = p.turnaroundTime - p.burstTime;
                completed++;
            }
        } else {
            // Idle time
            int nextArrival = processes[currentProcessIndex].arrivalTime;
            ExecutionInterval idleInterval{-1, currentTime, nextArrival};
            ganttChart.push_back(idleInterval);
            idleTime += (nextArrival - currentTime);
            currentTime = nextArrival;
            
            while (currentProcessIndex < n && processes[currentProcessIndex].arrivalTime <= currentTime) {
                addToCorrectQueue(currentProcessIndex);
                isInQueue[currentProcessIndex] = true;
                currentProcessIndex++;
            }
        }
    }

    totalExecutionTime = currentTime;
    calculateStatistics();
}

std::string MLQScheduler::getName() const {
    return "Multilevel Queue Scheduling (MLQ)";
}
