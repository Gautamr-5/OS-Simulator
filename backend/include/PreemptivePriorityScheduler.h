#ifndef PREEMPTIVEPRIORITYSCHEDULER_H
#define PREEMPTIVEPRIORITYSCHEDULER_H

#include "Scheduler.h"

class PreemptivePriorityScheduler : public Scheduler {
public:
    void schedule() override;
    std::string getName() const override;
};

#endif // PREEMPTIVEPRIORITYSCHEDULER_H
