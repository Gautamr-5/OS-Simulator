#include "RRScheduler.h"
#include <algorithm>
#include <queue>
#include <vector>

RRScheduler::RRScheduler(int timeQuantum) : timeQuantum(timeQuantum) {}

void RRScheduler::setTimeQuantum(int tq) {
    timeQuantum = tq;
}

int RRScheduler::getTimeQuantum() const {
    return timeQuantum;
}

void RRScheduler::schedule() {
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
    std::queue<int> readyQueue;
    
    ganttChart.clear();
    contextSwitches = 0;
    idleTime = 0;
    int lastProcessId = -1;

    // Reset remaining times just in case
    for(auto& p : processes) {
        p.remainingTime = p.burstTime;
        p.startTime = -1;
    }

    // Add processes that arrived at time 0
    int currentProcessIndex = 0;
    while(currentProcessIndex < n && processes[currentProcessIndex].arrivalTime <= currentTime) {
        readyQueue.push(currentProcessIndex);
        isInQueue[currentProcessIndex] = true;
        currentProcessIndex++;
    }

    while (completed != n) {
        if (!readyQueue.empty()) {
            int idx = readyQueue.front();
            readyQueue.pop();
            Process& p = processes[idx];

            if (p.startTime == -1) {
                p.startTime = currentTime;
                p.responseTime = p.startTime - p.arrivalTime;
            }

            int executeTime = std::min(timeQuantum, p.remainingTime);
            
            ExecutionInterval interval{p.id, currentTime, currentTime + executeTime};
            
            // Check if we can merge with previous interval to make Gantt chart cleaner
            if (!ganttChart.empty() && ganttChart.back().processId == p.id && ganttChart.back().endTime == currentTime) {
                ganttChart.back().endTime += executeTime;
            } else {
                ganttChart.push_back(interval);
                if (lastProcessId != -1 && lastProcessId != p.id) {
                    contextSwitches++;
                }
            }

            currentTime += executeTime;
            p.remainingTime -= executeTime;
            lastProcessId = p.id;

            // Check for new arrivals while this process was executing
            while (currentProcessIndex < n && processes[currentProcessIndex].arrivalTime <= currentTime) {
                readyQueue.push(currentProcessIndex);
                isInQueue[currentProcessIndex] = true;
                currentProcessIndex++;
            }

            if (p.remainingTime > 0) {
                readyQueue.push(idx); // Put back in queue
            } else {
                p.completionTime = currentTime;
                p.turnaroundTime = p.completionTime - p.arrivalTime;
                p.waitingTime = p.turnaroundTime - p.burstTime;
                completed++;
            }
        } else {
            // CPU is idle
            int nextArrival = processes[currentProcessIndex].arrivalTime;
            ExecutionInterval idleInterval{-1, currentTime, nextArrival};
            ganttChart.push_back(idleInterval);
            idleTime += (nextArrival - currentTime);
            currentTime = nextArrival;
            
            while (currentProcessIndex < n && processes[currentProcessIndex].arrivalTime <= currentTime) {
                readyQueue.push(currentProcessIndex);
                isInQueue[currentProcessIndex] = true;
                currentProcessIndex++;
            }
        }
    }

    totalExecutionTime = currentTime;
    calculateStatistics();
}

std::string RRScheduler::getName() const {
    return "Round Robin (RR) - Quantum: " + std::to_string(timeQuantum);
}
