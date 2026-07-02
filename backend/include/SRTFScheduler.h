#ifndef SRTFSCHEDULER_H
#define SRTFSCHEDULER_H

#include "Scheduler.h"

// Shortest Remaining Time First (Preemptive SJF)
class SRTFScheduler : public Scheduler {
public:
    void schedule() override;
    std::string getName() const override;
};

#endif // SRTFSCHEDULER_H
