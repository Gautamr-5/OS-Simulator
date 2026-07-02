#ifndef PRIORITYSCHEDULER_H
#define PRIORITYSCHEDULER_H

#include "Scheduler.h"

// Non-Preemptive Priority Scheduler
// Note: Lower priority number = Higher priority
class PriorityScheduler : public Scheduler {
public:
    void schedule() override;
    std::string getName() const override;
};

#endif // PRIORITYSCHEDULER_H
