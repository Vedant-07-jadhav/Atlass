#pragma once

#include "GridIndex.hpp"
#include "OccupancyGrid.hpp"
#include <cstddef>
#include <vector>

struct PlannerResult {
  bool success = false;
  std::vector<GridIndex> path;
  double pathLength = 0.0;
  std::size_t expandedNodes = 0;
  double planningTimeMs = 0.0;
};

class AStarPlanner {
public:
  // Diagonal moves use a conservative no-corner-cutting policy: every
  // axis-aligned intermediate voxel touched by the move must be free.
  PlannerResult plan(const OccupancyGrid &grid, const GridIndex &start,
                     const GridIndex &goal) const;
};
