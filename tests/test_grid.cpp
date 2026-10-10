#include <gtest/gtest.h>
#include "grid/OccupancyGrid.hpp"

TEST(OccupancyGridTest, Initialization) {
    OccupancyGrid grid(10, 20, 30, 0.5);
    EXPECT_EQ(grid.getWidth(), 10);
    EXPECT_EQ(grid.getHeight(), 20);
    EXPECT_EQ(grid.getDepth(), 30);
    EXPECT_DOUBLE_EQ(grid.getResolution(), 0.5);
}

TEST(OccupancyGridTest, InvalidInitialization) {
    EXPECT_THROW(OccupancyGrid(0, 10, 10, 1.0), std::invalid_argument);
    EXPECT_THROW(OccupancyGrid(10, 10, 10, -0.1), std::invalid_argument);
}

TEST(OccupancyGridTest, BoundsChecking) {
    OccupancyGrid grid(5, 5, 5, 1.0);
    EXPECT_TRUE(grid.isValid({0, 0, 0}));
    EXPECT_TRUE(grid.isValid({4, 4, 4}));
    
    EXPECT_FALSE(grid.isValid({-1, 0, 0}));
    EXPECT_FALSE(grid.isValid({0, 5, 0}));
}

TEST(OccupancyGridTest, OccupancyReadWrite) {
    OccupancyGrid grid(5, 5, 5, 1.0);
    GridIndex idx{2, 2, 2};
    
    EXPECT_FALSE(grid.isOccupied(idx));
    grid.setOccupancy(idx, true);
    EXPECT_TRUE(grid.isOccupied(idx));
    
    // Out of bounds is treated as occupied (obstacle)
    EXPECT_TRUE(grid.isOccupied({-1, -1, -1}));
}

TEST(OccupancyGridTest, CoordinateConversion) {
    OccupancyGrid grid(10, 10, 10, 0.5);
    
    // Origin mapping
    GridIndex origin = grid.worldToIndex(0.2, 0.3, 0.4);
    EXPECT_EQ(origin.x, 0);
    EXPECT_EQ(origin.y, 0);
    EXPECT_EQ(origin.z, 0);
    
    // Voxel center mapping
    double wx, wy, wz;
    grid.indexToWorld({1, 2, 3}, wx, wy, wz);
    EXPECT_DOUBLE_EQ(wx, 0.75); // (1 + 0.5) * 0.5
    EXPECT_DOUBLE_EQ(wy, 1.25); // (2 + 0.5) * 0.5
    EXPECT_DOUBLE_EQ(wz, 1.75); // (3 + 0.5) * 0.5
}

TEST(OccupancyGridTest, DeterministicRandomObstacles) {
    OccupancyGrid grid1(10, 10, 10, 1.0);
    OccupancyGrid grid2(10, 10, 10, 1.0);
    
    grid1.generateRandomObstacles(42, 0.2);
    grid2.generateRandomObstacles(42, 0.2);
    
    // Ensure both grids are identical given the same seed
    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            for (int z = 0; z < 10; ++z) {
                EXPECT_EQ(grid1.isOccupied({x, y, z}), grid2.isOccupied({x, y, z}));
            }
        }
    }
}