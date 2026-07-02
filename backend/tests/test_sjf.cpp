#include <gtest/gtest.h>
#include "SJFScheduler.h"

TEST(SJFSchedulerTest, BasicScheduling) {
    SJFScheduler scheduler;
    scheduler.addProcess(Process(1, 0, 8));
    scheduler.addProcess(Process(2, 1, 4));
    scheduler.addProcess(Process(3, 2, 9));
    scheduler.addProcess(Process(4, 3, 5));

    scheduler.schedule();

    const auto& processes = scheduler.getProcesses();
    
    // Check completion times
    // P1: arrives 0, bursts 8 -> completes 8
    // at time 8, P2(4), P3(9), P4(5) are ready. P2 is shortest.
    // P2: starts 8, bursts 4 -> completes 12
    // at time 12, P3(9), P4(5) are ready. P4 is shortest.
    // P4: starts 12, bursts 5 -> completes 17
    // P3: starts 17, bursts 9 -> completes 26

    EXPECT_EQ(processes[0].completionTime, 8); // P1
    EXPECT_EQ(processes[1].completionTime, 12); // P2
    EXPECT_EQ(processes[2].completionTime, 26); // P3
    EXPECT_EQ(processes[3].completionTime, 17); // P4
}
