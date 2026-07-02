#ifndef HRRNSCHEDULER_H
#define HRRNSCHEDULER_H

#include "Scheduler.h"

// Highest Response Ratio Next Scheduler (Non-Preemptive)
class HRRNScheduler : public Scheduler {
public:
    void schedule() override;
    std::string getName() const override;
};

#endif // HRRNSCHEDULER_H
