#include <gtest/gtest.h>
#include "HRRNScheduler.h"

TEST(HRRNSchedulerTest, BasicScheduling) {
    HRRNScheduler scheduler;
    scheduler.addProcess(Process(1, 0, 3));
    scheduler.addProcess(Process(2, 2, 6));
    scheduler.addProcess(Process(3, 4, 4));
    scheduler.addProcess(Process(4, 6, 5));
    scheduler.addProcess(Process(5, 8, 2));

    scheduler.schedule();

    const auto& processes = scheduler.getProcesses();
    
    // P1: arrives 0, starts 0, bursts 3 -> completes 3
    // At time 3, P2 is ready.
    // P2: starts 3, bursts 6 -> completes 9
    // At time 9, P3, P4, P5 are ready.
    // RR P3: (wait 9-4=5 + 4)/4 = 9/4 = 2.25
    // RR P4: (wait 9-6=3 + 5)/5 = 8/5 = 1.6
    // RR P5: (wait 9-8=1 + 2)/2 = 3/2 = 1.5
    // So P3 is scheduled.
    // P3: starts 9, bursts 4 -> completes 13
    // At time 13, P4, P5 ready.
    // RR P4: (wait 13-6=7 + 5)/5 = 12/5 = 2.4
    // RR P5: (wait 13-8=5 + 2)/2 = 7/2 = 3.5
    // So P5 is scheduled.
    // P5: starts 13, bursts 2 -> completes 15
    // So P4 is scheduled last.
    // P4: starts 15, bursts 5 -> completes 20

    EXPECT_EQ(processes[0].completionTime, 3);
    EXPECT_EQ(processes[1].completionTime, 9);
    EXPECT_EQ(processes[2].completionTime, 13);
    EXPECT_EQ(processes[3].completionTime, 20);
    EXPECT_EQ(processes[4].completionTime, 15);
}
