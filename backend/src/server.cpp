#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <iomanip>

#include "FCFSScheduler.h"
#include "SJFScheduler.h"
#include "PriorityScheduler.h"
#include "HRRNScheduler.h"
#include "RRScheduler.h"
#include "SRTFScheduler.h"
#include "PreemptivePriorityScheduler.h"
#include "MLQScheduler.h"

// Helper to escape JSON strings
std::string escapeJSON(const std::string& s) {
    std::ostringstream o;
    for (auto c = s.cbegin(); c != s.cend(); c++) {
        if (*c == '"' || *c == '\\' || ('\x00' <= *c && *c <= '\x1f')) {
            o << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)*c;
        } else {
            o << *c;
        }
    }
    return o.str();
}

std::string generateJSONResponse(Scheduler& scheduler) {
    std::stringstream ss;
    ss << "{";
    ss << "\"algorithm\": \"" << escapeJSON(scheduler.getName()) << "\",";
    
    // Processes
    ss << "\"processes\": [";
    const auto& processes = scheduler.getProcesses();
    for (size_t i = 0; i < processes.size(); ++i) {
        const auto& p = processes[i];
        ss << "{";
        ss << "\"id\": " << p.id << ",";
        ss << "\"arrivalTime\": " << p.arrivalTime << ",";
        ss << "\"burstTime\": " << p.burstTime << ",";
        ss << "\"priority\": " << p.priority << ",";
        ss << "\"completionTime\": " << p.completionTime << ",";
        ss << "\"turnaroundTime\": " << p.turnaroundTime << ",";
        ss << "\"waitingTime\": " << p.waitingTime << ",";
        ss << "\"responseTime\": " << p.responseTime;
        ss << "}";
        if (i < processes.size() - 1) ss << ",";
    }
    ss << "],";

    // Gantt Chart
    ss << "\"gantt\": [";
    const auto& gantt = scheduler.getGanttChart();
    for (size_t i = 0; i < gantt.size(); ++i) {
        const auto& interval = gantt[i];
        ss << "{";
        ss << "\"processId\": " << interval.processId << ","; // -1 for idle
        ss << "\"startTime\": " << interval.startTime << ",";
        ss << "\"endTime\": " << interval.endTime;
        ss << "}";
        if (i < gantt.size() - 1) ss << ",";
    }
    ss << "],";

    // Statistics
    ss << "\"statistics\": {";
    ss << "\"averageWaitingTime\": " << scheduler.getAverageWaitingTime() << ",";
    ss << "\"averageTurnaroundTime\": " << scheduler.getAverageTurnaroundTime() << ",";
    ss << "\"cpuUtilization\": " << scheduler.getCpuUtilization() << ",";
    ss << "\"contextSwitches\": " << scheduler.getContextSwitches() << ",";
    ss << "\"throughput\": " << scheduler.getThroughput();
    ss << "}";
    
    ss << "}";
    return ss.str();
}

void parseQueryProcesses(const std::string& data, std::vector<Process>& processes) {
    // Expected format: id,arr,burst,pri;id,arr,burst,pri
    std::stringstream ss(data);
    std::string procStr;
    
    while (std::getline(ss, procStr, ';')) {
        std::stringstream pss(procStr);
        std::string token;
        int id=0, arr=0, burst=0, pri=0;
        if (std::getline(pss, token, ',')) id = std::stoi(token);
        if (std::getline(pss, token, ',')) arr = std::stoi(token);
        if (std::getline(pss, token, ',')) burst = std::stoi(token);
        if (std::getline(pss, token, ',')) pri = std::stoi(token);
        processes.push_back(Process(id, arr, burst, pri));
    }
}

// Usage: OSServer.exe <algorithm> <processes_string> [quantum]
int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: OSServer.exe <algorithm> <processes_string> [quantum]" << std::endl;
        return 1;
    }

    std::string algo = argv[1];
    std::string processData = argv[2];
    int quantum = (argc >= 4) ? std::stoi(argv[3]) : 2;

    std::vector<Process> processes;
    parseQueryProcesses(processData, processes);

    std::unique_ptr<Scheduler> scheduler;
    if (algo == "sjf") scheduler = std::make_unique<SJFScheduler>();
    else if (algo == "priority") scheduler = std::make_unique<PriorityScheduler>();
    else if (algo == "hrrn") scheduler = std::make_unique<HRRNScheduler>();
    else if (algo == "rr") scheduler = std::make_unique<RRScheduler>(quantum);
    else if (algo == "srtf") scheduler = std::make_unique<SRTFScheduler>();
    else if (algo == "preemptive_priority") scheduler = std::make_unique<PreemptivePriorityScheduler>();
    else if (algo == "mlq") scheduler = std::make_unique<MLQScheduler>();
    else scheduler = std::make_unique<FCFSScheduler>(); // default

    for (const auto& p : processes) {
        scheduler->addProcess(p);
    }

    scheduler->schedule();

    // Print JSON to stdout
    std::cout << generateJSONResponse(*scheduler) << std::endl;

    return 0;
}
