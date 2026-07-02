#ifndef CSVUTILS_H
#define CSVUTILS_H

#include <vector>
#include <string>
#include "Process.h"

class CSVUtils {
public:
    // Loads processes from a CSV file. Expected columns: ID, ArrivalTime, BurstTime, Priority
    static std::vector<Process> loadFromCSV(const std::string& filename);

    // Saves scheduled processes and statistics to a CSV file.
    static bool saveToCSV(const std::string& filename, const std::vector<Process>& processes, 
                          double avgWait, double avgTurnaround, double cpuUtil, int contextSwitches);
};

#endif // CSVUTILS_H
