#include <gtest/gtest.h>
#include "PriorityScheduler.h"

TEST(PrioritySchedulerTest, BasicScheduling) {
    PriorityScheduler scheduler;
    scheduler.addProcess(Process(1, 0, 10, 3));
    scheduler.addProcess(Process(2, 0, 1, 1));
    scheduler.addProcess(Process(3, 0, 2, 4));
    scheduler.addProcess(Process(4, 0, 1, 5));
    scheduler.addProcess(Process(5, 0, 5, 2));

    scheduler.schedule();

    const auto& processes = scheduler.getProcesses();
    
    // Priority: lower number = higher priority
    // Sequence based on priorities at time 0: P2(1), P5(2), P1(3), P3(4), P4(5)
    // P2: 1 burst -> completes 1
    // P5: 5 burst -> completes 6
    // P1: 10 burst -> completes 16
    // P3: 2 burst -> completes 18
    // P4: 1 burst -> completes 19

    EXPECT_EQ(processes[0].completionTime, 16); // P1
    EXPECT_EQ(processes[1].completionTime, 1);  // P2
    EXPECT_EQ(processes[2].completionTime, 18); // P3
    EXPECT_EQ(processes[3].completionTime, 19); // P4
    EXPECT_EQ(processes[4].completionTime, 6);  // P5
}
