#include <gtest/gtest.h>
#include "MLQScheduler.h"

TEST(MLQSchedulerTest, BasicScheduling) {
    MLQScheduler scheduler;
    scheduler.addProcess(Process(1, 0, 5, 2)); // Queue 1
    scheduler.addProcess(Process(2, 1, 3, 4)); // Queue 2
    scheduler.addProcess(Process(3, 2, 4, 1)); // Queue 1

    scheduler.schedule();

    const auto& processes = scheduler.getProcesses();
    
    // P1 (Q1, RR=2): Exec 0-2 (rem 3). Queue1: [P3(1)] -> P3 arrived at 2.
    // P3 (Q1, RR=2): Exec 2-4 (rem 2). Queue1: [P1]
    // P1 (Q1, RR=2): Exec 4-6 (rem 1). Queue1: [P3]
    // P3 (Q1, RR=2): Exec 6-8 (rem 0, comp 8). Queue1: [P1]
    // P1 (Q1, RR=2): Exec 8-9 (rem 0, comp 9). Queue1: empty
    // P2 (Q2, RR=4): Exec 9-12 (rem 0, comp 12). Queue2: empty
    
    EXPECT_EQ(processes[0].completionTime, 9);
    EXPECT_EQ(processes[1].completionTime, 12);
    EXPECT_EQ(processes[2].completionTime, 8);
}
