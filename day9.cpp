#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#define DEBUG
using namespace std;
using ll = long long;

struct Tile {
    int x, y;
};

#ifdef DEBUG
template<typename T>
void logGrid(const vector<vector<T>>& grid) {
    for (int r = 0; r < static_cast<int>(grid.size()); r++) {
        for (int c = 0; c < static_cast<int>(grid[0].size()); c++)
            cerr << grid[r][c];
        cerr << '\n';
    }
    cerr << "-----\n";
}

template<typename T>
void logVec(const vector<T>& vec) {
    for (int i = 0; i < static_cast<int>(vec.size()); i++)
        cerr << vec[i] << ' ';
    cerr << "\n-----\n";
}
#endif

struct CompressedAxis {
    vector<int> starts;
    vector<int> widths;

    CompressedAxis(vector<int> coordinates) {
        sort(coordinates.begin(), coordinates.end());
        coordinates.erase(unique(coordinates.begin(), coordinates.end()), coordinates.end());
        coordinates.insert(coordinates.begin(), coordinates.front() - 1); // left padding
        coordinates.push_back(coordinates.back() + 1); // right padding
        for (int i = 0; i + 1 < (int)coordinates.size(); i++) {
            starts.push_back(coordinates[i]);
            widths.push_back(1);
            int gap = coordinates[i + 1] - coordinates[i] - 1;
            if (gap > 0) {
                starts.push_back(coordinates[i] + 1);  // gap representative
                widths.push_back(gap);
            }
        }
        starts.push_back(coordinates.back());
        widths.push_back(1);
    }

    int compress(int v) const {
        return (int)(lower_bound(starts.begin(), starts.end(), v) - starts.begin());
    }

    int size() const { return (int)starts.size(); }
};

ll Area(const Tile &a, const Tile &b) {
    return static_cast<ll>((abs(a.x - b.x) + 1)) * (abs(a.y - b.y) + 1);
}

int main() {
    ifstream file("day9.txt");
    string line;
    vector<Tile> redTiles;
    while (getline(file, line)) {
        int x, y;
        sscanf(line.c_str(), "%d,%d", &x, &y);
        redTiles.push_back({x, y});
    }
    const int n = static_cast<int>(redTiles.size());

    // Part 1
    ll maxArea = 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            maxArea = max(maxArea, Area(redTiles[i], redTiles[j]));
    cout << "Part 1: " << maxArea << "\n";

    // Part 2
    //           offX
    //            minX     maxX
    //           ⌄⌄        ⌄
    //          0123456789        Boundary          Exterior          PrefixSum
    // offY > 0 ..............    .000000000000.    .111111111111.    .000000000000.
    // minY > 1 .......#XXX#..    .000000111110.    .111111000001.    .000000123455.
    //        2 .......X...X..    .000000100010.    .111111000001.    .0000002468xx.
    //        3 ..#XXXX#...X..    .011111100010.    .100000000001.    .0123458xxxxx.
    //        4 ..X........X..    .010000000010.    .100000000001.    .02468xxxxxxx.
    //        5 ..#XXXXXX#.X..    .011111111010.    .100000000001.    .0369xxxxxxxx.
    //        6 .........X.X..    .000000001010.    .111111110001.    .0369xxxxxxxx.
    // maxY > 7 .........#X#..    .000000001110.    .111111110001.    .0369xxxxxxxx.
    //        8 ..............    .000000000000.    .111111111111.    .0369xxxxxxxx.
    //
    // Cell compression
    //       x  x  x
    //       ↓  ↓  ↓
    //     0123456789
    //   0 ..........
    //   1 ..........
    // y→2 ..#XXXXX#.
    //   3 ..X.....X.
    //   4 ..X.....X.
    // y→5 ..X..#XX#.
    //   6 ..X..X....
    //   7 ..X..#X#..
    // y→8 ..#XX#X#..
    //   9 ..........
    // At the gap x=3,4. No polygon vertical edge crosses this band. So every column in x=3,4 sees exactly the same
    // polygon edges when you scan vertically, meaning x=3 and x=4 are always in/out together for any given y.
    // The same applies row-wise between consecutive boundary y-values.

    // Step 1: build compressed axes from red tile coordinates
    vector<int> xs, ys;
    for (auto& [x, y] : redTiles) { xs.push_back(x); ys.push_back(y); }
    CompressedAxis xAxis(xs);
    CompressedAxis yAxis(ys);
    int W = xAxis.size(), H = yAxis.size();

    // Step 2: mark boundary in compressed space
    // iterate compressed indices between endpoints so gap cells along the edge are also marked
    vector<vector<bool>> boundary(H, vector<bool>(W, false));
    for (int i = 0; i < n; i++) {
        auto [x1, y1] = redTiles[i];
        auto [x2, y2] = redTiles[(i + 1) % n];
        assert(x1 == x2 || y1 == y2);
        if (x1 == x2) {
            int cc = xAxis.compress(x1);
            for (int cr = yAxis.compress(min(y1, y2)); cr <= yAxis.compress(max(y1, y2)); cr++)
                boundary[cr][cc] = true;
        } else {
            int cr = yAxis.compress(y1);
            for (int cc = xAxis.compress(min(x1, x2)); cc <= xAxis.compress(max(x1, x2)); cc++)
                boundary[cr][cc] = true;
        }
    }
    // BFS to fill the exterior tiles with 1
    vector<vector<bool>> exterior(H, vector<bool>(W, false));
    queue<Tile> q;
    q.push({0, 0}); // without 1-cell padding, we can not BFS at (0,0)
    exterior[0][0] = true;
    const int dr[] = {0, 0, -1, 1};
    const int dc[] = {1, -1, 0, 0};
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr >= 0 && nr < H && nc >= 0 && nc < W) {
                if (boundary[nr][nc] || exterior[nr][nc]) continue;
                exterior[nr][nc] = true;
                q.push({nr, nc});
            }
        }
    }
    // Step 4: weighted prefix sum — each cell contributes (col_width * row_height) original tiles
    vector<vector<ll>> prefixSum(H, vector<ll>(W, 0));
    for (int r = 1; r < H; r++)
        for (int c = 1; c < W; c++) {
            ll tiles = exterior[r][c] ? 0 : (ll)xAxis.widths[c] * yAxis.widths[r];
            prefixSum[r][c] = tiles + prefixSum[r-1][c] + prefixSum[r][c-1] - prefixSum[r-1][c-1];
        }
    auto rectSum = [&](int r1, int c1, int r2, int c2) -> ll {
        return prefixSum[r2][c2] - prefixSum[r1-1][c2] - prefixSum[r2][c1-1] + prefixSum[r1-1][c1-1];
    };
    // Step 5: check all pairs using compressed coordinates
    ll maxArea2 = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            const auto [x1, y1] = redTiles[i];
            const auto [x2, y2] = redTiles[j];
            int c1 = xAxis.compress(min(x1, x2));
            int c2 = xAxis.compress(max(x1, x2));
            int r1 = yAxis.compress(min(y1, y2));
            int r2 = yAxis.compress(max(y1, y2));
            ll area = Area(redTiles[i], redTiles[j]);
            ll tileArea = rectSum(r1, c1, r2, c2);
            if (tileArea == area) maxArea2 = max(maxArea2, area);
        }
    }
    cout << "Part 2: " << maxArea2 << "\n";

    return 0;
}