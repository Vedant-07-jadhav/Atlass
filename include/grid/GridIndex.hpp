#pragma once

#include <cstddef>
#include <functional>

struct GridIndex{
    int x;
    int y;
    int z;

    bool operator==(const GridIndex& other) const{
        return x == other.x && y==other.y && z == other.z;
    }

    bool operator!=(const GridIndex& other)const{
        return !(*this == other);
    }
};


namespace std {
    template<>
    struct hash<GridIndex>{
        size_t operator()(const GridIndex& idx)const{
            size_t h1 = std::hash<int>{}(idx.x);
            size_t h2 = std::hash<int>{}(idx.y);
            size_t h3 = std::hash<int>{}(idx.z);
            return h1 ^ (h2 * 73856093) ^ (h3 * 19349663);
        }
    };
}