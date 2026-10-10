#pragma once

#include "GridIndex.hpp"
#include <cstdint>
#include <vector>

class OccupancyGrid {
public:
  OccupancyGrid(int width, int height, int depth, double resolution);

  bool isValid(const GridIndex &idx) const;
  bool isOccupied(const GridIndex &idx) const;
  void setOccupancy(const GridIndex &idx, bool occupied);

  // Converts physical world coordinates to discrete voxel indices
  GridIndex worldToIndex(double x, double y, double z) const;

  // Returns the physical center of the specified voxel
  void indexToWorld(const GridIndex &idx, double &x, double &y,
                    double &z) const;

  // Populates grid with random obstacles (0.0 to 1.0 density)
  void generateRandomObstacles(unsigned int seed, double density);

  int getWidth() const { return width_; }
  int getHeight() const { return height_; }
  int getDepth() const { return depth_; }
  double getResolution() const { return resolution_; }

private:
  std::size_t getFlatIndex(const GridIndex &idx) const;

  int width_;
  int height_;
  int depth_;
  double resolution_;

  // Using uint8_t instead of vector<bool> for thread-safety and predictability
  std::vector<uint8_t> data_;
};