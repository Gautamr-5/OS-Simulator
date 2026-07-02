#include <gtest/gtest.h>
#include "FCFSScheduler.h"

TEST(FCFSSchedulerTest, BasicScheduling) {
    FCFSScheduler scheduler;
    scheduler.addProcess(Process(1, 0, 5));
    scheduler.addProcess(Process(2, 1, 3));
    scheduler.addProcess(Process(3, 2, 8));
    scheduler.addProcess(Process(4, 3, 6));

    scheduler.schedule();

    const auto& processes = scheduler.getProcesses();
    
    // Check completion times
    // P1: arrives 0, bursts 5 -> completes 5
    // P2: arrives 1, starts 5, bursts 3 -> completes 8
    // P3: arrives 2, starts 8, bursts 8 -> completes 16
    // P4: arrives 3, starts 16, bursts 6 -> completes 22

    EXPECT_EQ(processes[0].completionTime, 5);
    EXPECT_EQ(processes[1].completionTime, 8);
    EXPECT_EQ(processes[2].completionTime, 16);
    EXPECT_EQ(processes[3].completionTime, 22);

    // Check waiting times
    EXPECT_EQ(processes[0].waitingTime, 0); // 5-5 = 0? No, start - arrival: 0-0=0
    EXPECT_EQ(processes[1].waitingTime, 4); // start - arrival: 5-1=4
    EXPECT_EQ(processes[2].waitingTime, 6); // start - arrival: 8-2=6
    EXPECT_EQ(processes[3].waitingTime, 13); // start - arrival: 16-3=13
    
    // Average Waiting Time = (0 + 4 + 6 + 13) / 4 = 5.75
    EXPECT_DOUBLE_EQ(scheduler.getAverageWaitingTime(), 5.75);
}

TEST(FCFSSchedulerTest, IdleTimeHandling) {
    FCFSScheduler scheduler;
    scheduler.addProcess(Process(1, 0, 2));
    scheduler.addProcess(Process(2, 5, 2));

    scheduler.schedule();

    const auto& processes = scheduler.getProcesses();
    const auto& gantt = scheduler.getGanttChart();

    EXPECT_EQ(processes[0].completionTime, 2);
    EXPECT_EQ(processes[1].completionTime, 7);

    // Gantt chart should have 3 intervals: P1(0-2), Idle(2-5), P2(5-7)
    ASSERT_EQ(gantt.size(), 3);
    EXPECT_EQ(gantt[0].processId, 1);
    EXPECT_EQ(gantt[1].processId, -1); // Idle
    EXPECT_EQ(gantt[1].startTime, 2);
    EXPECT_EQ(gantt[1].endTime, 5);
    EXPECT_EQ(gantt[2].processId, 2);
}
