#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;
using ll = long long;

template<typename T>
void log(const string& label, const vector<T>& v) {
    cout << label << ": [";
    for (size_t i = 0; i < v.size(); i++) {
        if (i) cout << ", ";
        cout << v[i];
    }
    cout << "]\n";
}

struct Operator {
    char op;
    size_t start;
    size_t end;
};

vector<Operator> parseOperators(const string& line, size_t maxLen) {
    vector<Operator> ops;
    for (size_t i = 0; i < line.size(); i++)
        if (line[i] != ' ') ops.push_back({line[i], i, 0});
    for (size_t i = 0; i + 1 < ops.size(); i++)
        ops[i].end = ops[i+1].start - 2;
    if (!ops.empty()) ops.back().end = maxLen - 1;
    return ops;
}

ll calc(const vector<ll>& nums, const char op) {
    if (op == '+') return accumulate(nums.begin(), nums.end(), 0LL);
    if (op == '*') return accumulate(nums.begin(), nums.end(), 1LL, multiplies<ll>());
    return 0;
}

int main() {
    ifstream file("day6.txt");
    string line, opLine;
    vector<string> dataRows;
    size_t maxLength = 0;
    while (getline(file, line)) {
        maxLength = max(maxLength, line.size());
        if (line.find('+') != string::npos || line.find('*') != string::npos) {
            opLine = line;
        } else {
            dataRows.push_back(line);
        }
    }
    for (auto& row : dataRows) row.resize(maxLength, ' ');
    vector<Operator> ops = parseOperators(opLine, maxLength);

    // Part 1
    ll part1 = 0;
    for (const auto& [op, start, end] : ops) {
        vector<ll> nums;
        for (size_t r = 0; r < dataRows.size(); r++)
            nums.push_back(stoll(dataRows[r].substr(start, end - start + 1)));
        part1 += calc(nums, op);
    }
    cout << "Part 1: " << part1 << "\n";

    // Part 2
    ll part2 = 0;
    for (const auto& [op, start, end] : ops) {
        vector<ll> nums;
        for (auto c = start; c <= end; c++) {
            string num;
            for (size_t r = 0; r < dataRows.size(); r++)
                if (dataRows[r][c] != ' ') num.push_back(dataRows[r][c]);
            assert(num.size() > 0);
            nums.push_back(stoll(num));
        }
        assert(nums.size() == end - start + 1);
        part2 += calc(nums, op);
    }
    cout << "Part 2: " << part2 << "\n";

    return 0;
}