#ifndef MLQSCHEDULER_H
#define MLQSCHEDULER_H

#include "Scheduler.h"
#include <queue>

// Multilevel Queue Scheduler
// Queue 1: Priority 1-2 (Round Robin, Q=2) -> Highest Priority
// Queue 2: Priority 3-4 (Round Robin, Q=4)
// Queue 3: Priority 5+  (FCFS)             -> Lowest Priority
class MLQScheduler : public Scheduler {
public:
    void schedule() override;
    std::string getName() const override;
};

#endif // MLQSCHEDULER_H
