#include "pathfinder.h"
#include <chrono>
#include <random>

Array<Point> BacktrackingPathFinder::findPath(const Maze& maze, Point start, Point end) {
    Array<Point> path;
    path.Init();

    int W = maze.getWidth(), H = maze.getHeight();

    Array<bool> visited;
    visited.Init();
    for (int i = 0; i < W * H; ++i) visited.MPush(false);

    Stack<Point> stack;
    stack.SPush(start);
    visited.MGetRef(start.y * W + start.x) = true;

    std::mt19937 rng((unsigned)std::chrono::steady_clock::now().time_since_epoch().count());

    while (!stack.SEmpty()) {
        Point cur = stack.STop();

        if (cur == end) {
            Stack<Point> temp;
            while (!stack.SEmpty()) { temp.SPush(stack.STop()); stack.SPop(); }
            while (!temp.SEmpty()) { path.MPush(temp.STop()); temp.SPop(); }
            visited.clear();
            return path;
        }

        Array<Point> neighbors;
        neighbors.Init();
        const Cell& c = maze.getCell(cur.x, cur.y);

        if (!c.hasWall(Direction::Top)    && !visited.MGet((cur.y-1)*W + cur.x)) neighbors.MPush({cur.x, cur.y-1});
        if (!c.hasWall(Direction::Right)  && !visited.MGet(cur.y*W + cur.x+1))   neighbors.MPush({cur.x+1, cur.y});
        if (!c.hasWall(Direction::Bottom) && !visited.MGet((cur.y+1)*W + cur.x)) neighbors.MPush({cur.x, cur.y+1});
        if (!c.hasWall(Direction::Left)   && !visited.MGet(cur.y*W + cur.x-1))   neighbors.MPush({cur.x-1, cur.y});

        if (neighbors.MLen() > 0) {
            Point next = neighbors.MGet(rng() % neighbors.MLen());
            visited.MGetRef(next.y * W + next.x) = true;
            stack.SPush(next);
        } else {
            stack.SPop();
        }
        neighbors.clear();
    }

    visited.clear();
    return path;
}
