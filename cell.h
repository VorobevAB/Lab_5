#pragma once

enum class Direction { Top, Right, Bottom, Left };

class Cell {
public:
    bool walls[4] = { true, true, true, true };

    bool hasWall(Direction d) const { return walls[(int)d]; }
    void removeWall(Direction d) { walls[(int)d] = false; }
};
