#include <iostream>
#include <vector>
#include <memory>
#include "FCFSScheduler.h"
#include "SJFScheduler.h"
#include "PriorityScheduler.h"
#include "HRRNScheduler.h"
#include "RRScheduler.h"
#include "SRTFScheduler.h"
#include "PreemptivePriorityScheduler.h"
#include "MLQScheduler.h"

void runScheduler(Scheduler& scheduler, const std::vector<Process>& processes) {
    for (const auto& p : processes) {
        scheduler.addProcess(p);
    }

    std::cout << "\nRunning " << scheduler.getName() << "..." << std::endl;
    scheduler.schedule();

    std::cout << "Results:" << std::endl;
    std::cout << "PID\tArrival\tBurst\tPriority\tCompletion\tTurnaround\tWaiting" << std::endl;
    for (const auto& p : scheduler.getProcesses()) {
        std::cout << p.id << "\t" 
                  << p.arrivalTime << "\t" 
                  << p.burstTime << "\t"
                  << p.priority << "\t\t"
                  << p.completionTime << "\t\t"
                  << p.turnaroundTime << "\t\t"
                  << p.waitingTime << std::endl;
    }

    std::cout << "\nStatistics:" << std::endl;
    std::cout << "Average Waiting Time:    " << scheduler.getAverageWaitingTime() << std::endl;
    std::cout << "Average Turnaround Time: " << scheduler.getAverageTurnaroundTime() << std::endl;
    std::cout << "CPU Utilization:         " << scheduler.getCpuUtilization() << "%" << std::endl;
    std::cout << "Context Switches:        " << scheduler.getContextSwitches() << std::endl;
    std::cout << "---------------------------------------" << std::endl;
}

int main() {
    std::cout << "Operating System Scheduling Simulator CLI" << std::endl;
    std::cout << "=======================================" << std::endl;

    std::vector<Process> processes = {
        Process(1, 0, 4, 2),
        Process(2, 1, 3, 3),
        Process(3, 2, 1, 4),
        Process(4, 3, 2, 5),
        Process(5, 4, 5, 1)
    };

    FCFSScheduler fcfs;
    SJFScheduler sjf;
    PriorityScheduler priority;
    HRRNScheduler hrrn;
    
    RRScheduler rr(2);
    SRTFScheduler srtf;
    PreemptivePriorityScheduler preemptivePriority;
    MLQScheduler mlq;

    // Phase 1
    runScheduler(fcfs, processes);
    runScheduler(sjf, processes);
    runScheduler(priority, processes);
    runScheduler(hrrn, processes);

    // Phase 2
    runScheduler(rr, processes);
    runScheduler(srtf, processes);
    runScheduler(preemptivePriority, processes);
    runScheduler(mlq, processes);

    return 0;
}
