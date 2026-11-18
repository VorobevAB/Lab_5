#pragma once
#include "maze.h"

struct Point {
    int x, y;
    bool operator==(const Point& o) const { return x == o.x && y == o.y; }
};

class IPathFinder {
public:
    virtual ~IPathFinder() = default;
    virtual Array<Point> findPath(const Maze& maze, Point start, Point end) = 0;
};

class BacktrackingPathFinder : public IPathFinder {
public:
    Array<Point> findPath(const Maze& maze, Point start, Point end) override;
};
