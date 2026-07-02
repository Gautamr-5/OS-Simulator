#ifndef SJFSCHEDULER_H
#define SJFSCHEDULER_H

#include "Scheduler.h"

class SJFScheduler : public Scheduler {
public:
    void schedule() override;
    std::string getName() const override;
};

#endif // SJFSCHEDULER_H
