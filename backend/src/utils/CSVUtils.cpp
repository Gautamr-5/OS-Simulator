#include "CSVUtils.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<Process> CSVUtils::loadFromCSV(const std::string& filename) {
    std::vector<Process> processes;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open CSV file " << filename << std::endl;
        return processes;
    }

    std::string line;
    // Skip header line
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        int id = 0, arrival = 0, burst = 0, priority = 0;

        try {
            if (std::getline(ss, token, ',')) id = std::stoi(token);
            if (std::getline(ss, token, ',')) arrival = std::stoi(token);
            if (std::getline(ss, token, ',')) burst = std::stoi(token);
            if (std::getline(ss, token, ',')) priority = std::stoi(token);
            
            processes.push_back(Process(id, arrival, burst, priority));
        } catch (const std::exception& e) {
            std::cerr << "Error parsing CSV line: " << line << std::endl;
        }
    }

    file.close();
    return processes;
}

bool CSVUtils::saveToCSV(const std::string& filename, const std::vector<Process>& processes, 
                         double avgWait, double avgTurnaround, double cpuUtil, int contextSwitches) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not create CSV file " << filename << std::endl;
        return false;
    }

    // Write header
    file << "ID,ArrivalTime,BurstTime,Priority,CompletionTime,TurnaroundTime,WaitingTime\n";

    // Write processes
    for (const auto& p : processes) {
        file << p.id << ","
             << p.arrivalTime << ","
             << p.burstTime << ","
             << p.priority << ","
             << p.completionTime << ","
             << p.turnaroundTime << ","
             << p.waitingTime << "\n";
    }

    // Write stats at the end
    file << "\nStatistics\n";
    file << "Average Waiting Time," << avgWait << "\n";
    file << "Average Turnaround Time," << avgTurnaround << "\n";
    file << "CPU Utilization," << cpuUtil << "\n";
    file << "Context Switches," << contextSwitches << "\n";

    file.close();
    return true;
}
