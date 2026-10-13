#include "grid/AStarPlanner.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <limits>
#include <queue>
#include <vector>

namespace {

struct QueueEntry {
  double priority;
  double cost;
  std::size_t index;
};

struct QueueEntryCompare {
  bool operator()(const QueueEntry &lhs, const QueueEntry &rhs) const {
    if (lhs.priority != rhs.priority) {
      return lhs.priority > rhs.priority;
    }
    return lhs.cost > rhs.cost;
  }
};

std::size_t toFlatIndex(const GridIndex &idx, int width, int height) {
  const auto widthSize = static_cast<std::size_t>(width);
  const auto heightSize = static_cast<std::size_t>(height);
  return static_cast<std::size_t>(idx.x) +
         static_cast<std::size_t>(idx.y) * widthSize +
         static_cast<std::size_t>(idx.z) * widthSize * heightSize;
}

GridIndex fromFlatIndex(std::size_t index, int width, int height) {
  const auto widthSize = static_cast<std::size_t>(width);
  const auto heightSize = static_cast<std::size_t>(height);
  const auto layerSize = widthSize * heightSize;
  const auto z = index / layerSize;
  index %= layerSize;
  const auto y = index / widthSize;
  const auto x = index % widthSize;
  return {static_cast<int>(x), static_cast<int>(y), static_cast<int>(z)};
}

double heuristic(const GridIndex &from, const GridIndex &to) {
  const double dx = static_cast<double>(from.x - to.x);
  const double dy = static_cast<double>(from.y - to.y);
  const double dz = static_cast<double>(from.z - to.z);
  return std::sqrt(dx * dx + dy * dy + dz * dz);
}

bool transitionIsClear(const OccupancyGrid &grid, const GridIndex &from,
                       int dx, int dy, int dz) {
  const int xLimit = dx != 0 ? 1 : 0;
  const int yLimit = dy != 0 ? 1 : 0;
  const int zLimit = dz != 0 ? 1 : 0;
  for (int x = 0; x <= xLimit; ++x) {
    for (int y = 0; y <= yLimit; ++y) {
      for (int z = 0; z <= zLimit; ++z) {
        const bool isStart = x == 0 && y == 0 && z == 0;
        const bool isEnd = x == xLimit && y == yLimit && z == zLimit;
        if (isStart || isEnd) {
          continue;
        }

        const GridIndex touched{from.x + x * dx, from.y + y * dy,
                                from.z + z * dz};
        if (!grid.isValid(touched) || grid.isOccupied(touched)) {
          return false;
        }
      }
    }
  }
  return true;
}

} // namespace

PlannerResult AStarPlanner::plan(const OccupancyGrid &grid,
                                 const GridIndex &start,
                                 const GridIndex &goal) const {
  const auto begin = std::chrono::steady_clock::now();
  PlannerResult result;

  const auto finish = [&begin, &result]() {
    const auto elapsed = std::chrono::steady_clock::now() - begin;
    result.planningTimeMs =
        std::chrono::duration<double, std::milli>(elapsed).count();
  };

  if (!grid.isValid(start) || !grid.isValid(goal) ||
      grid.isOccupied(start) || grid.isOccupied(goal)) {
    finish();
    return result;
  }

  if (start == goal) {
    result.success = true;
    result.path.push_back(start);
    finish();
    return result;
  }

  const int width = grid.getWidth();
  const int height = grid.getHeight();
  const int depth = grid.getDepth();
  const auto nodeCount = static_cast<std::size_t>(width) *
                         static_cast<std::size_t>(height) *
                         static_cast<std::size_t>(depth);
  const double infinity = std::numeric_limits<double>::infinity();
  const std::size_t noParent = nodeCount;

  std::vector<double> bestCost(nodeCount, infinity);
  std::vector<std::size_t> parent(nodeCount, noParent);
  std::priority_queue<QueueEntry, std::vector<QueueEntry>,
                      QueueEntryCompare>
      openSet;

  const auto startIndex = toFlatIndex(start, width, height);
  const auto goalIndex = toFlatIndex(goal, width, height);
  bestCost[startIndex] = 0.0;
  openSet.push({heuristic(start, goal), 0.0, startIndex});

  while (!openSet.empty()) {
    const QueueEntry current = openSet.top();
    openSet.pop();

    if (current.cost != bestCost[current.index]) {
      continue;
    }
    ++result.expandedNodes;

    if (current.index == goalIndex) {
      result.success = true;
      result.pathLength = current.cost;
      for (std::size_t index = goalIndex; index != noParent;
           index = parent[index]) {
        result.path.push_back(fromFlatIndex(index, width, height));
      }
      std::reverse(result.path.begin(), result.path.end());
      finish();
      return result;
    }

    const GridIndex currentIndex =
        fromFlatIndex(current.index, width, height);
    for (int dx = -1; dx <= 1; ++dx) {
      for (int dy = -1; dy <= 1; ++dy) {
        for (int dz = -1; dz <= 1; ++dz) {
          if (dx == 0 && dy == 0 && dz == 0) {
            continue;
          }

          const GridIndex neighbour{currentIndex.x + dx, currentIndex.y + dy,
                                    currentIndex.z + dz};
          if (!grid.isValid(neighbour) || grid.isOccupied(neighbour)) {
            continue;
          }
          if (!transitionIsClear(grid, currentIndex, dx, dy, dz)) {
            continue;
          }

          const int changedAxes = (dx != 0) + (dy != 0) + (dz != 0);
          const double movementCost =
              changedAxes == 1 ? 1.0
                               : (changedAxes == 2 ? std::sqrt(2.0)
                                                   : std::sqrt(3.0));
          const auto neighbourIndex = toFlatIndex(neighbour, width, height);
          const double tentativeCost = current.cost + movementCost;
          if (tentativeCost >= bestCost[neighbourIndex]) {
            continue;
          }

          bestCost[neighbourIndex] = tentativeCost;
          parent[neighbourIndex] = current.index;
          openSet.push(
              {tentativeCost + heuristic(neighbour, goal), tentativeCost,
               neighbourIndex});
        }
      }
    }
  }

  finish();
  return result;
}
