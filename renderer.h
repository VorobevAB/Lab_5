#pragma once
#include <SFML/Graphics.hpp>
#include "maze.h"
#include "pathfinder.h"

class IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual void run() = 0;
};

class MazeRenderer : public IRenderer {
public:
    MazeRenderer(Maze& m, IPathFinder& f);
    void run() override;

private:
    Maze& maze;
    IPathFinder& finder;
    sf::RenderWindow window;
    float cellSize = 0.f;
    sf::VertexArray walls;

    Point start{-1, -1}, finish{-1, -1};
    Array<Point> path;

    void buildGeometry();
    void draw();
};
