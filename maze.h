#pragma once
#include "containers.h"
#include "cell.h"
#include <random>

class Maze {
private:
    int width, height;
    Array<Cell> cells;
    std::mt19937 rng;
    
public:
    Maze(int w, int h);

    void generate();

    int getWidth()  const { return width; }
    int getHeight() const { return height; }

    Cell& getCell(int x, int y) { return cells.MGetRef(y * width + x); }
    const Cell& getCell(int x, int y) const { return cells.MGetRef(y * width + x); }
    ~Maze();
};
