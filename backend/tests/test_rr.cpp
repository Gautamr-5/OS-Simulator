#include <gtest/gtest.h>
#include "RRScheduler.h"

TEST(RRSchedulerTest, BasicScheduling) {
    RRScheduler scheduler(2); // Quantum = 2
    scheduler.addProcess(Process(1, 0, 5));
    scheduler.addProcess(Process(2, 1, 4));
    scheduler.addProcess(Process(3, 2, 2));
    scheduler.addProcess(Process(4, 3, 1));

    scheduler.schedule();

    const auto& processes = scheduler.getProcesses();
    
    // Check completion times
    // P1: arrives 0, Q2 -> executes 0-2 (rem 3). Queue: [P2(1), P1]
    // P2: arrives 1, Q2 -> executes 2-4 (rem 2). Queue: [P3(2), P1, P4(3), P2]
    // P3: arrives 2, Q2 -> executes 4-6 (rem 0, completes). Queue: [P1, P4, P2]
    // P1: Q2 -> executes 6-8 (rem 1). Queue: [P4, P2, P1]
    // P4: arrives 3, Q1 -> executes 8-9 (rem 0, completes). Queue: [P2, P1]
    // P2: Q2 -> executes 9-11 (rem 0, completes). Queue: [P1]
    // P1: Q1 -> executes 11-12 (rem 0, completes).
    
    EXPECT_EQ(processes[0].completionTime, 12);
    EXPECT_EQ(processes[1].completionTime, 11);
    EXPECT_EQ(processes[2].completionTime, 6);
    EXPECT_EQ(processes[3].completionTime, 9);
}
