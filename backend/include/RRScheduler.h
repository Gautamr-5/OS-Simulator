#ifndef RRSCHEDULER_H
#define RRSCHEDULER_H

#include "Scheduler.h"

class RRScheduler : public Scheduler {
private:
    int timeQuantum;

public:
    RRScheduler(int timeQuantum = 2);
    void setTimeQuantum(int tq);
    int getTimeQuantum() const;

    void schedule() override;
    std::string getName() const override;
};

#endif // RRSCHEDULER_H
