#include "renderer.h"
#include <iostream>
#include <algorithm>

MazeRenderer::MazeRenderer(Maze& m, IPathFinder& f)
    : maze(m), finder(f)
{
    path.Init();

    float W = (float)maze.getWidth();
    float H = (float)maze.getHeight();
    cellSize = std::max(300.f / std::max(W, H), 8.f);

    window.create(sf::VideoMode((unsigned)(W * cellSize), (unsigned)(H * cellSize)),
                  "Maze - Eller + Backtracking");
    window.setFramerateLimit(60);
    buildGeometry();
}

void MazeRenderer::buildGeometry() {
    walls.clear();
    walls.setPrimitiveType(sf::Lines);

    int W = maze.getWidth(), H = maze.getHeight();
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            const Cell& c = maze.getCell(x, y);
            float px = x * cellSize, py = y * cellSize;
            sf::Color col(220, 220, 220);

            auto line = [&](float x1, float y1, float x2, float y2) {
                walls.append(sf::Vertex({x1, y1}, col));
                walls.append(sf::Vertex({x2, y2}, col));
            };

            if (c.hasWall(Direction::Top))    line(px, py, px + cellSize, py);
            if (c.hasWall(Direction::Left))   line(px, py, px, py + cellSize);
            if (c.hasWall(Direction::Right))  line(px + cellSize, py, px + cellSize, py + cellSize);
            if (c.hasWall(Direction::Bottom)) line(px, py + cellSize, px + cellSize, py + cellSize);
        }
    }
}

void MazeRenderer::draw() {
    window.clear(sf::Color(30, 30, 40));
    window.draw(walls);

    if (path.MLen() > 0) {
        sf::VertexArray lines(sf::LinesStrip);
        for (int i = 0; i < path.MLen(); ++i) {
            Point p = path.MGet(i);
            float cx = p.x * cellSize + cellSize / 2;
            float cy = p.y * cellSize + cellSize / 2;
            lines.append(sf::Vertex({cx, cy}, sf::Color::Red));
        }
        window.draw(lines);
    }

    auto circle = [&](Point p, sf::Color col) {
        if (p.x == -1) return;
        sf::CircleShape s(cellSize / 3);
        s.setFillColor(col);
        s.setPosition(p.x * cellSize + cellSize / 6, p.y * cellSize + cellSize / 6);
        window.draw(s);
    };
    circle(start, sf::Color::Green);
    circle(finish, sf::Color::Blue);

    window.display();
}

void MazeRenderer::run() {
    std::cout << "ЛКМ: старт (зеленый), затем финиш (синий)\n"
              << "R: новый лабиринт, ESC: выход\n";

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) window.close();

            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Escape) window.close();
                if (e.key.code == sf::Keyboard::R) {
                    maze.generate();
                    start = finish = {-1, -1};
                    path.clear(); path.Init();
                    buildGeometry();
                }
            }

            if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
                int mx = (int)(e.mouseButton.x / cellSize);
                int my = (int)(e.mouseButton.y / cellSize);
                if (mx < 0 || mx >= maze.getWidth() || my < 0 || my >= maze.getHeight()) continue;

                if (start.x == -1) {
                    start = {mx, my};
                    finish = {-1, -1};
                    path.clear(); path.Init();
                } else if (finish.x == -1) {
                    if (mx != start.x || my != start.y) {
                        finish = {mx, my};
                        path.clear();
                        path = finder.findPath(maze, start, finish);
                    }
                } else {
                    start = {mx, my};
                    finish = {-1, -1};
                    path.clear(); path.Init();
                }
            }
        }
        draw();
    }
    path.clear();
}
