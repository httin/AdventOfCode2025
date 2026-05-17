#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <vector>

using namespace std;
using ll = long long;

bool binarySearch(const vector<pair<ll,ll>>& intervals, ll id) {
    int l = 0, r = static_cast<int>(intervals.size()) - 1;
    if (id < intervals[l].first || id > intervals[r].second)
        return false;
    while (l <= r) {
        if (l == r) break;
        ll m = (l + r) / 2;
        if (intervals[m].first <= id && id <= intervals[m].second)
            return true;
        if (intervals[m].second < id) {
            l = m + 1;
        } else {
            r = m - 1;
        }
    }
    return intervals[l].first <= id && id <= intervals[l].second;
}

int main() {
    ifstream file("day5.txt");
    vector<string> lines;
    string line;
    vector<pair<ll,ll>> freshIntervals;
    while (getline(file, line)) {
        if (line.empty()) break;
        ll a, b;
        sscanf(line.c_str(), "%lld-%lld", &a, &b);
        freshIntervals.push_back(make_pair(a,b));
    }

    sort(freshIntervals.begin(), freshIntervals.end());
    vector<pair<ll,ll>> merged;
    for (auto& [a, b]: freshIntervals) {
        if (!merged.empty() && a <= merged.back().second)
            merged.back().second = max(merged.back().second, b);
        else
            merged.push_back({a, b});
    }
    // Part 1
    ll part1 = 0;
    while (getline(file, line)) {
        ll id;
        sscanf(line.c_str(), "%lld", &id);
        part1 += binarySearch(merged, id);
    }
    cout << "Part 1: " << part1 << "\n";

    // Part 2
    ll part2 = 0;
    for (auto& [a, b] : merged)
        part2 += b - a + 1;
    cout << "Part 2: " << part2 << "\n";

    return 0;
}