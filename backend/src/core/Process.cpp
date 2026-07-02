#include "Process.h"

Process::Process(int id, int arrivalTime, int burstTime, int priority, std::string color)
    : id(id), arrivalTime(arrivalTime), burstTime(burstTime), priority(priority),
      remainingTime(burstTime), startTime(-1), completionTime(0),
      turnaroundTime(0), waitingTime(0), responseTime(-1), color(color) {}

void Process::reset() {
    remainingTime = burstTime;
    startTime = -1;
    completionTime = 0;
    turnaroundTime = 0;
    waitingTime = 0;
    responseTime = -1;
}
