#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using ll = long long;

int main() {
    ifstream file("dayDAY_N.txt");
    assert(file.is_open());

    vector<string> lines;
    string line;
    while (getline(file, line)) lines.push_back(line);

    // Part 1
    ll part1 = 0;
    cout << "Part 1: " << part1 << "\n";

    // Part 2
    ll part2 = 0;
    cout << "Part 2: " << part2 << "\n";

    return 0;
}