#include <gtest/gtest.h>
#include "PreemptivePriorityScheduler.h"

TEST(PreemptivePrioritySchedulerTest, BasicScheduling) {
    PreemptivePriorityScheduler scheduler;
    scheduler.addProcess(Process(1, 0, 4, 3));
    scheduler.addProcess(Process(2, 1, 3, 1)); // High priority
    scheduler.addProcess(Process(3, 2, 1, 4));
    scheduler.addProcess(Process(4, 3, 2, 5));
    scheduler.addProcess(Process(5, 4, 5, 2)); // 2nd high priority

    scheduler.schedule();

    const auto& processes = scheduler.getProcesses();
    
    // P1(4, p3): Exec 0-1 (rem 3)
    // P2(3, p1) arrives. Preempts P1. Exec 1-4 (completes 4)
    // at 4: P1(3, p3), P3(1, p4), P4(2, p5), P5(5, p2).
    // P5 has highest priority (2). Exec 4-9 (completes 9)
    // at 9: P1(p3), P3(p4), P4(p5). P1 highest. Exec 9-12
    // at 12: P3(p4), P4(p5). P3 highest. Exec 12-13
    // at 13: P4(p5) Exec 13-15

    EXPECT_EQ(processes[0].completionTime, 12);
    EXPECT_EQ(processes[1].completionTime, 4);
    EXPECT_EQ(processes[2].completionTime, 13);
    EXPECT_EQ(processes[3].completionTime, 15);
    EXPECT_EQ(processes[4].completionTime, 9);
}
