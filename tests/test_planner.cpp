#include "grid/AStarPlanner.hpp"

#include <gtest/gtest.h>
#include <cmath>

namespace {

double pathCost(const std::vector<GridIndex> &path) {
  double cost = 0.0;
  for (std::size_t i = 1; i < path.size(); ++i) {
    const int dx = std::abs(path[i].x - path[i - 1].x);
    const int dy = std::abs(path[i].y - path[i - 1].y);
    const int dz = std::abs(path[i].z - path[i - 1].z);
    const int changedAxes = (dx != 0) + (dy != 0) + (dz != 0);
    cost += changedAxes == 1 ? 1.0
                             : (changedAxes == 2 ? std::sqrt(2.0)
                                                 : std::sqrt(3.0));
  }
  return cost;
}

void expectValidPath(const OccupancyGrid &grid, const PlannerResult &result,
                     const GridIndex &start, const GridIndex &goal) {
  ASSERT_TRUE(result.success);
  ASSERT_FALSE(result.path.empty());
  EXPECT_EQ(result.path.front(), start);
  EXPECT_EQ(result.path.back(), goal);
  for (const auto &index : result.path) {
    EXPECT_TRUE(grid.isValid(index));
    EXPECT_FALSE(grid.isOccupied(index));
  }
}

} // namespace

TEST(AStarPlannerTest, FindsPathOnEmptyGrid) {
  OccupancyGrid grid(4, 4, 4, 1.0);
  const GridIndex start{0, 0, 0};
  const GridIndex goal{3, 3, 3};

  const PlannerResult result = AStarPlanner{}.plan(grid, start, goal);

  expectValidPath(grid, result, start, goal);
  EXPECT_DOUBLE_EQ(result.pathLength, 3.0 * std::sqrt(3.0));
  EXPECT_DOUBLE_EQ(result.pathLength, pathCost(result.path));
  EXPECT_GT(result.expandedNodes, 0U);
  EXPECT_GE(result.planningTimeMs, 0.0);
}

TEST(AStarPlannerTest, HandlesStartEqualToGoal) {
  OccupancyGrid grid(2, 2, 2, 1.0);
  const GridIndex state{1, 1, 1};

  const PlannerResult result = AStarPlanner{}.plan(grid, state, state);

  expectValidPath(grid, result, state, state);
  EXPECT_DOUBLE_EQ(result.pathLength, 0.0);
  EXPECT_EQ(result.expandedNodes, 0U);
}

TEST(AStarPlannerTest, RejectsOccupiedAndOutOfBoundsStates) {
  OccupancyGrid grid(3, 3, 3, 1.0);
  const GridIndex start{0, 0, 0};
  const GridIndex goal{2, 2, 2};
  AStarPlanner planner;

  grid.setOccupancy(start, true);
  EXPECT_FALSE(planner.plan(grid, start, goal).success);
  grid.setOccupancy(start, false);
  grid.setOccupancy(goal, true);
  EXPECT_FALSE(planner.plan(grid, start, goal).success);
  EXPECT_FALSE(planner.plan(grid, {-1, 0, 0}, goal).success);
  EXPECT_FALSE(planner.plan(grid, start, {3, 0, 0}).success);
}

TEST(AStarPlannerTest, ReportsUnreachableGoal) {
  OccupancyGrid grid(3, 3, 3, 1.0);
  const GridIndex start{0, 0, 0};
  const GridIndex goal{2, 2, 2};
  for (int x = 0; x < 3; ++x) {
    for (int y = 0; y < 3; ++y) {
      grid.setOccupancy({x, y, 1}, true);
    }
  }

  const PlannerResult result = AStarPlanner{}.plan(grid, start, goal);

  EXPECT_FALSE(result.success);
  EXPECT_TRUE(result.path.empty());
  EXPECT_DOUBLE_EQ(result.pathLength, 0.0);
  EXPECT_GT(result.expandedNodes, 0U);
}

TEST(AStarPlannerTest, FindsOptimalPathAroundObstacles) {
  OccupancyGrid grid(3, 3, 1, 1.0);
  grid.setOccupancy({1, 1, 0}, true);
  const GridIndex start{0, 0, 0};
  const GridIndex goal{2, 2, 0};

  const PlannerResult result = AStarPlanner{}.plan(grid, start, goal);

  expectValidPath(grid, result, start, goal);
  EXPECT_DOUBLE_EQ(result.pathLength, 4.0);
  EXPECT_DOUBLE_EQ(result.pathLength, pathCost(result.path));
}

TEST(AStarPlannerTest, AllowsFaceMovementBesideObstacle) {
  OccupancyGrid grid(2, 2, 1, 1.0);
  grid.setOccupancy({0, 1, 0}, true);

  const PlannerResult result =
      AStarPlanner{}.plan(grid, {0, 0, 0}, {1, 0, 0});

  expectValidPath(grid, result, {0, 0, 0}, {1, 0, 0});
  EXPECT_DOUBLE_EQ(result.pathLength, 1.0);
}

TEST(AStarPlannerTest, BlocksEdgeDiagonalCornerCutting) {
  OccupancyGrid grid(2, 2, 1, 1.0);
  grid.setOccupancy({1, 0, 0}, true);
  grid.setOccupancy({0, 1, 0}, true);

  const PlannerResult result =
      AStarPlanner{}.plan(grid, {0, 0, 0}, {1, 1, 0});

  EXPECT_FALSE(result.success);
}

TEST(AStarPlannerTest, BlocksCornerDiagonalCornerCutting) {
  OccupancyGrid grid(2, 2, 2, 1.0);
  const GridIndex start{0, 0, 0};
  const GridIndex goal{1, 1, 1};
  grid.setOccupancy({1, 0, 0}, true);
  grid.setOccupancy({0, 1, 0}, true);
  grid.setOccupancy({0, 0, 1}, true);
  grid.setOccupancy({1, 1, 0}, true);
  grid.setOccupancy({1, 0, 1}, true);
  grid.setOccupancy({0, 1, 1}, true);

  const PlannerResult result = AStarPlanner{}.plan(grid, start, goal);

  EXPECT_FALSE(result.success);
}

TEST(AStarPlannerTest, FindsPathThroughNarrowFreePassage) {
  OccupancyGrid grid(5, 3, 1, 1.0);
  for (int x = 0; x < 5; ++x) {
    grid.setOccupancy({x, 0, 0}, true);
    grid.setOccupancy({x, 2, 0}, true);
  }

  const PlannerResult result =
      AStarPlanner{}.plan(grid, {0, 1, 0}, {4, 1, 0});

  expectValidPath(grid, result, {0, 1, 0}, {4, 1, 0});
  EXPECT_DOUBLE_EQ(result.pathLength, 4.0);
  EXPECT_DOUBLE_EQ(result.pathLength, pathCost(result.path));
}

TEST(AStarPlannerTest, ReportsCompletelyEnclosedStart) {
  OccupancyGrid grid(3, 3, 3, 1.0);
  const GridIndex start{1, 1, 1};
  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      for (int dz = -1; dz <= 1; ++dz) {
        if (dx != 0 || dy != 0 || dz != 0) {
          grid.setOccupancy({start.x + dx, start.y + dy, start.z + dz},
                            true);
        }
      }
    }
  }

  const PlannerResult result =
      AStarPlanner{}.plan(grid, start, {2, 2, 2});

  EXPECT_FALSE(result.success);
  EXPECT_TRUE(result.path.empty());
}
