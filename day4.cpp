#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <array>
#include <string>
#include <vector>

using namespace std;
using ll = long long;

struct Point { int r, c; };

const array<pair<int,int>, 8> dirs = {{{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}}};

bool isValid(const vector<string>& grid, const Point& point) {
    if (point.r >= 0 && point.r < grid.size()
        && point.c >= 0 && point.c < grid[0].size()) {
        return true;
    }
    return false;
}

bool isRollAccessible(const vector<string>& grid, const Point& point) {
    if (grid[point.r][point.c] == '.') return false;
    int adjacentRolls = 0;
    for (auto [dr, dc] : dirs) {
        Point adjacent = {point.r + dr, point.c + dc};
        if (isValid(grid, adjacent)) {
            adjacentRolls += grid[adjacent.r][adjacent.c] == '@';
        }
    }
    return adjacentRolls < 4;
}

ll removeRolls(vector<string>& grid) {
    ll removed = 0;
    for (int r = 0; r < (int)grid.size(); r++) {
        for (int c = 0; c < (int)grid[0].size(); c++) {
            if (isRollAccessible(grid, Point{r, c})) {
                removed += 1;
                grid[r][c] = '.';
            }
        }
    }
    if (removed > 0) {
        removed += removeRolls(grid);
    }
    return removed;
}

int main() {
    ifstream file("day4.txt");
    vector<string> lines;
    string line;
    while (getline(file, line)) lines.push_back(line);

    // Part 1
    ll part1 = 0;
    for (int r = 0; r < lines.size(); r++) {
        for (int c = 0; c < lines[0].size(); c++) {
            part1 += isRollAccessible(lines, Point{r, c});
        }
    }
    cout << "Part 1: " << part1 << "\n";

    // Part 2
    ll part2 = removeRolls(lines);
    cout << "Part 2: " << part2 << "\n";

    return 0;
}