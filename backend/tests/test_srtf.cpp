#include <gtest/gtest.h>
#include "SRTFScheduler.h"

TEST(SRTFSchedulerTest, BasicScheduling) {
    SRTFScheduler scheduler;
    scheduler.addProcess(Process(1, 0, 8));
    scheduler.addProcess(Process(2, 1, 4));
    scheduler.addProcess(Process(3, 2, 9));
    scheduler.addProcess(Process(4, 3, 5));

    scheduler.schedule();

    const auto& processes = scheduler.getProcesses();
    
    // P1: arrives 0, rem 8. Exec 0-1
    // P2: arrives 1, rem 4. P1 rem 7. P2 preempts! Exec 1-5 (completes)
    // at 5: P1(7), P3(9), P4(5). P4 shortest. Exec 5-10 (completes)
    // at 10: P1(7), P3(9). P1 shortest. Exec 10-17 (completes)
    // at 17: P3(9). Exec 17-26 (completes)
    
    EXPECT_EQ(processes[0].completionTime, 17);
    EXPECT_EQ(processes[1].completionTime, 5);
    EXPECT_EQ(processes[2].completionTime, 26);
    EXPECT_EQ(processes[3].completionTime, 10);
}
