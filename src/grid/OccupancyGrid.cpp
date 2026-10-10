#include "grid/OccupancyGrid.hpp"
#include <cmath>
#include <limits>
#include <random>
#include <stdexcept>

OccupancyGrid::OccupancyGrid(int width, int height, int depth,
                             double resolution)
    : width_(width), height_(height), depth_(depth), resolution_(resolution) {

  if (width_ <= 0 || height_ <= 0 || depth_ <= 0) {
    throw std::invalid_argument("Grid dimensions must be strictly positive.");
  }
  if (resolution_ <= 0.0) {
    throw std::invalid_argument("Grid resolution must be strictly positive.");
  }

  const auto widthSize = static_cast<std::size_t>(width_);
  const auto heightSize = static_cast<std::size_t>(height_);
  const auto depthSize = static_cast<std::size_t>(depth_);
  const auto maxSize = std::numeric_limits<std::size_t>::max();
  if (heightSize > maxSize / widthSize ||
      depthSize > maxSize / (widthSize * heightSize)) {
    throw std::length_error("Grid dimensions are too large.");
  }

  data_.resize(widthSize * heightSize * depthSize, 0);
}

bool OccupancyGrid::isValid(const GridIndex &idx) const {
  return idx.x >= 0 && idx.x < width_ && idx.y >= 0 && idx.y < height_ &&
         idx.z >= 0 && idx.z < depth_;
}

std::size_t OccupancyGrid::getFlatIndex(const GridIndex &idx) const {
  const auto width = static_cast<std::size_t>(width_);
  const auto height = static_cast<std::size_t>(height_);
  return static_cast<std::size_t>(idx.x) +
         static_cast<std::size_t>(idx.y) * width +
         static_cast<std::size_t>(idx.z) * width * height;
}

bool OccupancyGrid::isOccupied(const GridIndex &idx) const {
  if (!isValid(idx)) {
    return true; // Out-of-bounds is implicitly an obstacle
  }
  return data_[getFlatIndex(idx)] != 0;
}

void OccupancyGrid::setOccupancy(const GridIndex &idx, bool occupied) {
  if (!isValid(idx)) {
    throw std::out_of_range("Attempted to set occupancy out of grid bounds.");
  }
  data_[getFlatIndex(idx)] = occupied ? 1 : 0;
}

GridIndex OccupancyGrid::worldToIndex(double x, double y, double z) const {
  const auto toIndex = [this](double coordinate) {
    const double scaled = std::floor(coordinate / resolution_);
    if (!std::isfinite(scaled) ||
        scaled < static_cast<double>(std::numeric_limits<int>::min()) ||
        scaled > static_cast<double>(std::numeric_limits<int>::max())) {
      throw std::out_of_range(
          "World coordinate cannot be represented as a grid index.");
    }
    return static_cast<int>(scaled);
  };

  return {toIndex(x), toIndex(y), toIndex(z)};
}

void OccupancyGrid::indexToWorld(const GridIndex &idx, double &x, double &y,
                                 double &z) const {
  if (!isValid(idx)) {
    throw std::out_of_range("Grid index is outside the grid bounds.");
  }

  // Return the center of the voxel
  x = (idx.x + 0.5) * resolution_;
  y = (idx.y + 0.5) * resolution_;
  z = (idx.z + 0.5) * resolution_;
}

void OccupancyGrid::generateRandomObstacles(unsigned int seed, double density) {
  if (density < 0.0 || density > 1.0) {
    throw std::invalid_argument("Density must be between 0.0 and 1.0");
  }

  std::mt19937 gen(seed);
  std::bernoulli_distribution d(density);

  for (size_t i = 0; i < data_.size(); ++i) {
    data_[i] = d(gen) ? 1 : 0;
  }
}