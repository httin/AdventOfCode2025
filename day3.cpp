#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using ll = long long;

// Returns the lexicographically largest k-digit subsequence of s.
// Examples (k=2):
//   "987654321111111" -> already decrease, nothing ever pops, final stack is whole string, trim the tail -> "98"
//   "811111111111119" -> stack fills with "811...", when 9 arrives, pop all 1, then push 9 -> "89"
//   "234234234234278" -> stack from "2" to "3" to "4",... til "4444", when 7 arrives, pop all 4, then push 8 -> "78"
//   "818181911112111" -> stack fills with "888", when 9 arrives, pop all, then "92111", trim the tail -> "92"
string largestKDigits(const string& s, int k) {
    int toRemove = (int)s.length() - k;
    string stack;
    for (auto d : s) {
        while (!stack.empty() && stack.back() < d && toRemove > 0) {
            stack.pop_back();
            toRemove--;
        }
        stack.push_back(d);
    }
    while (toRemove > 0) {stack.pop_back(); toRemove--;}
    return stack;
}

int main() {
    ifstream file("day3.txt");
    vector<string> lines;
    string line;
    while (getline(file, line)) lines.push_back(line);

    // Part 1
    ll part1 = 0;
    for (auto line : lines) {
        string ans = largestKDigits(line, 2);
        part1 += stoll(ans);
    }
    cout << "Part 1: " << part1 << "\n";

    // Part 2
    ll part2 = 0;
    for (auto line : lines) {
        string ans = largestKDigits(line, 12);
        part2 += stoll(ans);
    }
    cout << "Part 2: " << part2 << "\n";

    return 0;
}