#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;
using ll = long long;

struct Point {
    ll x, y, z;
};

struct Edge {
    int p1, p2;
    double dist;
    Edge(int a, int b, double d) : p1(min(a, b)), p2(max(a, b)), dist(d) {}
    bool operator==(const Edge& o) const { return p1 == o.p1 && p2 == o.p2; }
    bool operator<(const Edge& o) const { return dist < o.dist; }
};

class DisjointSet {
    vector<int> parent;
    vector<int> rank_;
    vector<int> size_;
    int numComponents_;
public:
    explicit DisjointSet(int n) : parent(n), rank_(n, 0), size_(n, 1), numComponents_(n) {
        iota(parent.begin(), parent.end(), 0);
    }

    int Find(int u) {
        if (parent[u] != u)
            parent[u] = Find(parent[u]);
        return parent[u];
    }

    void Union(int u, int v) {
        int pu = Find(u), pv = Find(v);
        if (pu == pv) return;
        if (rank_[pu] < rank_[pv]) swap(pu, pv);
        parent[pv] = pu;
        size_[pu] += size_[pv];
        if (rank_[pu] == rank_[pv]) rank_[pu]++;
        numComponents_--;
    }

    int Size(int u) { return size_[Find(u)]; }
    int NumComponents() const { return numComponents_; }
    bool IsAllConnected() const { return numComponents_ == 1; }

    // Returns one size per component (root nodes only).
    vector<int> ComponentSizes() const {
        vector<int> sizes;
        for (int u = 0; u < parent.size(); u++)
            if (u == parent[u]) sizes.push_back(size_[u]);
        return sizes;
    }

#ifdef DEBUG
    void Log() const {
        for (int i = 0; i < static_cast<int>(parent.size()); i++)
            cerr << "  node " << i << " -> root " << parent[i]
                 << " rank=" << rank_[i] << " size=" << size_[i] << "\n";
    }
#endif
};

double euclidean(const Point& a, const Point& b) {
    auto dx = a.x - b.x, dy = a.y - b.y, dz = a.z - b.z;
    return sqrt(static_cast<double>(dx*dx + dy*dy + dz*dz));
}

#ifdef DEBUG
void logEdges(const vector<Edge>& edges) {
    for (const auto& e : edges)
        cerr << "Edge(" << e.p1 << ", " << e.p2 << ") dist=" << fixed << e.dist << "\n";
}
#endif

int main() {
    ifstream file("day8.txt");
    assert(file.is_open());

    vector<Point> points;
    string line;
    while (getline(file, line)) {
        Point p;
        sscanf(line.c_str(), "%lld,%lld,%lld", &p.x, &p.y, &p.z);
        points.push_back(p);
    }

    const int n = static_cast<int>(points.size());
    vector<Edge> edges;
    edges.reserve(n * (n - 1) / 2);
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            edges.push_back({i, j, euclidean(points[i], points[j])});
    sort(edges.begin(), edges.end());

    // Part 1: connect 1000 closest pairs, multiply the three largest circuit sizes.
    assert(edges.size() >= 1000);
    DisjointSet dsu(n);
    for (int i = 0; i < 1000; i++)
        dsu.Union(edges[i].p1, edges[i].p2);

    auto sizes = dsu.ComponentSizes();
    assert(sizes.size() >= 3);
    sort(sizes.rbegin(), sizes.rend());
    ll part1 = static_cast<ll>(sizes[0]) * sizes[1] * sizes[2];
    cout << "Part 1: " << part1 << "\n";

    // Part 2: keep connecting until all boxes are in one circuit.
    int e = 1001;
    while (!dsu.IsAllConnected()) {
        assert(e < static_cast<int>(edges.size()));
        dsu.Union(edges[e].p1, edges[e].p2);
        e++;
    }
    e--;
    cout << "Part 2: " << points[edges[e].p1].x * points[edges[e].p2].x << "\n";

    return 0;
}