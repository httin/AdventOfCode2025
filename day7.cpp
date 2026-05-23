#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using ll = long long;

struct Point {
    int r, c;
};

// Counts reachable '^' splitters from position p (Part 1).
// Uses `seen` to visit each cell at most once, so a splitter hit by two converging
// paths is counted only once.
// e.g.
// ^ splits at col 7; left child col 6 and right child col 8,
// ^ splits at col 9; left child col 8 and right child col 10,
// both producing a child at col 8 — `seen` ensures the col 8 is counted once, not twice.
ll dfs(const vector<string>& grid, vector<vector<bool>>& seen, const Point& p) {
    if (p.r < 0 || p.r >= (int)grid.size() || p.c < 0 || p.c >= (int)grid[0].size()) return 0;
    if (seen[p.r][p.c]) return 0;
    seen[p.r][p.c] = true;
    char next = grid[p.r + 1][p.c];
    ll result = 0;
    if (next == '.') result = dfs(grid, seen, Point{p.r + 1, p.c});
    if (next == '^') result = 1 + dfs(grid, seen, Point{p.r + 1, p.c - 1}) + dfs(grid, seen, Point{p.r + 1, p.c + 1});
    return result;
}

// Counts distinct timelines (paths to the bottom row) from position p (Part 2).
// Uses `memo` so converging paths don't recompute the same subtree.
// e.g.
// ^ splits at col 7; left child col 6 and right child col 8,
// ^ splits at col 9; left child col 8 and right child col 10,
// both reach col 8 — memo[r][8] is computed once and reused, not traversed twice.
ll timelinesDFS(const vector<string>& grid, vector<vector<ll>>& memo, Point p) {
    if (p.r < 0 || p.r >= (int)grid.size() || p.c < 0 || p.c >= (int)grid[0].size()) return 0;
    if (p.r == (int)grid.size() - 1) return 1;
    if (memo[p.r][p.c] >= 0) return memo[p.r][p.c];
    char next = grid[p.r + 1][p.c];
    ll result = 0;
    if (next == '.') result = timelinesDFS(grid, memo, Point{p.r + 1, p.c});
    else if (next == '^') result = timelinesDFS(grid, memo, Point{p.r + 1, p.c - 1}) + timelinesDFS(grid, memo, Point{p.r + 1, p.c + 1});
    return memo[p.r][p.c] = result;
}

int main() {
    ifstream file("day7.txt");
    vector<string> grid;
    string line;
    while (getline(file, line)) grid.push_back(line);
    vector<string> grid2 = grid;
    vector<vector<bool>> seen(grid.size(), vector<bool>(grid[0].size(), false));

    int sc = 0;
    for (sc = 0; sc < (int)grid[0].size(); ++sc)
        if (grid[0][sc] == 'S') break;
    // Part 1
    ll part1 = 0;
    part1 = dfs(grid, seen, Point{0, sc});
    cout << "Part 1: " << part1 << "\n";

    // Part 2
    vector<vector<ll>> memo(grid2.size(), vector<ll>(grid2[0].size(), -1));
    ll part2 = timelinesDFS(grid2, memo, Point{0, sc});
    cout << "Part 2: " << part2 << "\n";

    return 0;
}