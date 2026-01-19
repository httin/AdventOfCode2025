#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

using ll = long long;

int mod(int x, int m) {
    return (x%m + m) % m;
}

// Euclidean division: a = q*b + r with 0 <= r < |b|
std::pair<int,int> euclid_div(int a, int b) {
    if (b == 0) throw std::runtime_error("division by zero");
    int q = a / b;
    int r = a % b; // r has the same sign as a
    if (r < 0) {
        // fix to make remainder non-negative
        if (b > 0) { q -= 1; r += b; }
        else { q += 1; r -= b; }
    }
    return {q, r};
}

int main() {
    std::ifstream file("day1.txt");
    std::string s;
    int pos = 50;
    int res = 0;
    bool part2 = true;
    while (std::getline(file, s))
    {
        assert(s.size() >= 2);
        int sign = s[0] == 'L' ? -1 : 1;
        int val = std::stoi(s.substr(1));
        int sum = pos + sign * val;
        if (part2) {
            auto [q, r] = euclid_div(sum, 100);
            if (pos == 0) {
                // do not count if start with 0
                res += std::abs(sum/100);
            } else if (sum == 0) {
                // count if end with 0
                res += 1;
            } else {
                // rotate left: 5 - 105 = -100 = -1*100 + 0 -> +2
                // rotate right: 5 + 95 = 100 = 1*100 + 0 -> +1
                res += std::abs(q);
                res += (r == 0 && q < 0) ? 1 : 0;
            }
            pos = r;
        } else {
            pos = mod(sum, 100);
            res += pos == 0 ? 1 : 0;
        }
        std::cout << s << " sign=" << sign << " val=" << val << " pos=" << pos << " res=" << res << std::endl;
    }
    std::cout << res << std::endl;
    return 0;
}
