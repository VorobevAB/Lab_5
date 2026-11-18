#include "maze.h"
#include <chrono>

namespace {
    constexpr int kHorizontalPassChance = 45;
    constexpr int kVerticalPassChance   = 50;
}

struct DSU {
    Array<int> parent;

    void Init(int n) {
        parent.Init();
        for (int i = 0; i < n; ++i)
            parent.MPush(i);
    }

    int Find(int x) {
        if (parent.MGet(x) == x) return x;
        int root = Find(parent.MGet(x));
        parent.MGetRef(x) = root;
        return root;
    }

    void Unite(int a, int b) {
        int ra = Find(a), rb = Find(b);
        if (ra != rb) parent.MGetRef(ra) = rb;
    }

    bool Same(int a, int b) { return Find(a) == Find(b); }

    void clear() { parent.clear(); }
};

Maze::Maze(int w, int h)
    : width(w), height(h),
      rng((unsigned)std::chrono::steady_clock::now().time_since_epoch().count())
{
    cells.Init();
    for (int i = 0; i < width * height; ++i)
        cells.MPush(Cell());
}

Maze::~Maze() {
    cells.clear();
}

void Maze::generate() {
    for (int i = 0; i < width * height; ++i)
        cells.MGetRef(i) = Cell();

    const int N = width * height;

    DSU dsu;
    dsu.Init(N);

    Array<bool> hasConnection;
    hasConnection.Init();
    for (int i = 0; i < N; ++i)
        hasConnection.MPush(false);

    auto id = [&](int x, int y) { return y * width + x; };

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x + 1 < width; ++x) {
            int a = id(x, y), b = id(x + 1, y);
            if (dsu.Same(a, b)) continue;
            if ((rng() % 100) < kHorizontalPassChance) {
                cells.MGetRef(a).removeWall(Direction::Right);
                cells.MGetRef(b).removeWall(Direction::Left);
                dsu.Unite(a, b);
            }
        }

        if (y == height - 1) {
            for (int x = 0; x + 1 < width; ++x) {
                int a = id(x, y), b = id(x + 1, y);
                if (!dsu.Same(a, b)) {
                    cells.MGetRef(a).removeWall(Direction::Right);
                    cells.MGetRef(b).removeWall(Direction::Left);
                    dsu.Unite(a, b);
                }
            }
            break;
        }

        Array<int> roots;
        roots.Init();
        for (int x = 0; x < width; ++x) {
            int r = dsu.Find(id(x, y));
            bool found = false;
            for (int i = 0; i < roots.MLen(); ++i)
                if (roots.MGet(i) == r) { found = true; break; }
            if (!found) roots.MPush(r);
        }

        for (int k = 0; k < roots.MLen(); ++k) {
            int r = roots.MGet(k);

            Array<int> cols;
            cols.Init();
            for (int x = 0; x < width; ++x)
                if (dsu.Find(id(x, y)) == r)
                    cols.MPush(x);

            bool made = false;
            for (int j = 0; j < cols.MLen(); ++j) {
                int x = cols.MGet(j);
                bool mustOpen = (cols.MLen() == 1);
                if (mustOpen || (rng() % 100) < kVerticalPassChance) {
                    cells.MGetRef(id(x, y)).removeWall(Direction::Bottom);
                    cells.MGetRef(id(x, y + 1)).removeWall(Direction::Top);
                    dsu.Unite(id(x, y), id(x, y + 1));
                    hasConnection.MGetRef(id(x, y + 1)) = true;
                    made = true;
                }
            }

            if (!made && cols.MLen() > 0) {
                int x = cols.MGet(rng() % cols.MLen());
                cells.MGetRef(id(x, y)).removeWall(Direction::Bottom);
                cells.MGetRef(id(x, y + 1)).removeWall(Direction::Top);
                dsu.Unite(id(x, y), id(x, y + 1));
                hasConnection.MGetRef(id(x, y + 1)) = true;
            }

            cols.clear();
        }
        roots.clear();
    }

    dsu.clear();
    hasConnection.clear();
}
