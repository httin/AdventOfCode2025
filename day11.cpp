#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using ll = long long;

// Directed graph of "node: neighbor neighbor ..." adjacency lines.
// The puzzle graph is a DAG, so path counts are well-defined and memoizable.
class Graph {
    unordered_map<string, vector<string>> adj_;

public:
    static Graph fromFile(const string& path) {
        ifstream file(path);
        assert(file.is_open());

        Graph g;
        string line;
        while (getline(file, line)) {
            size_t colon = line.find(':');
            string src = line.substr(0, colon);
            stringstream ss(line.substr(colon + 2)); // skip ": "
            for (string dst; getline(ss, dst, ' ');) g.adj_[src].push_back(dst);
        }
        return g;
    }

    // Number of distinct paths from src to dst (DAG, memoized per call).
    ll countPaths(const string& src, const string& dst) const {
        unordered_map<string, ll> memo;
        return countPaths(src, dst, memo);
    }

private:
    ll countPaths(const string& cur, const string& dst,
                  unordered_map<string, ll>& memo) const {
        if (cur == dst) return 1;
        if (memo.find(cur) != memo.end()) return memo[cur];

        ll res = 0;
        auto neighbors = adj_.find(cur);
        if (neighbors != adj_.end()) {
            for (const string& next : neighbors->second) {
                res += countPaths(next, dst, memo);
            }
        }
        memo[cur] = res;
        return res;
    }
};

ll solve1(const Graph& g) {
    return g.countPaths("you", "out");
}

// Every svr->out path threads through both fft and dac, in one order or the
// other. In a DAG the count splits multiplicatively over the forced waypoints.
ll solve2(const Graph& g) {
    return g.countPaths("svr", "fft") * g.countPaths("fft", "dac") * g.countPaths("dac", "out")
         + g.countPaths("svr", "dac") * g.countPaths("dac", "fft") * g.countPaths("fft", "out");
}

int main() {
    Graph g = Graph::fromFile("day11.txt");
    cout << "Part 1: " << solve1(g) << "\n";
    cout << "Part 2: " << solve2(g) << "\n";
    return 0;
}
