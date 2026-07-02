#include <gtest/gtest.h>
#include "CSVUtils.h"
#include <fstream>
#include <cstdio>

TEST(CSVUtilsTest, SaveAndLoad) {
    std::string filename = "test_processes.csv";
    
    std::vector<Process> processes = {
        Process(1, 0, 5, 2),
        Process(2, 2, 3, 1)
    };

    // Save
    bool saved = CSVUtils::saveToCSV(filename, processes, 4.5, 8.0, 100.0, 2);
    EXPECT_TRUE(saved);

    // Load
    std::vector<Process> loaded = CSVUtils::loadFromCSV(filename);
    
    ASSERT_EQ(loaded.size(), 2);
    EXPECT_EQ(loaded[0].id, 1);
    EXPECT_EQ(loaded[0].arrivalTime, 0);
    EXPECT_EQ(loaded[0].burstTime, 5);
    EXPECT_EQ(loaded[0].priority, 2);

    EXPECT_EQ(loaded[1].id, 2);
    EXPECT_EQ(loaded[1].arrivalTime, 2);
    EXPECT_EQ(loaded[1].burstTime, 3);
    EXPECT_EQ(loaded[1].priority, 1);

    // Clean up
    std::remove(filename.c_str());
}
