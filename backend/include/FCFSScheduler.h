#ifndef FCFSSCHEDULER_H
#define FCFSSCHEDULER_H

#include "Scheduler.h"

class FCFSScheduler : public Scheduler {
public:
    void schedule() override;
    std::string getName() const override;
};

#endif // FCFSSCHEDULER_H
