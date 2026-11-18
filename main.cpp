#include <iostream>
#include "maze.h"
#include "pathfinder.h"
#include "renderer.h"

int main(int argc, char** argv) {
    int w = 10, h = 10;
    if (argc >= 3) { w = std::atoi(argv[1]); h = std::atoi(argv[2]); }
    else {
        std::cout << "Ширина и высота (например, 10 10): ";
        if (!(std::cin >> w >> h)) return 1;
    }
    if (w < 1 || h < 1) return 1;

    Maze maze(w, h);
    maze.generate();

    BacktrackingPathFinder finder;
    MazeRenderer renderer(maze, finder);
    renderer.run();
    return 0;
}
