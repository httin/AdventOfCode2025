#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
using ll = long long;

vector<string> split(const string &s, char delimiter) {
    vector<string> elems;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delimiter)) {
        elems.push_back(item);
    }
    return elems;
}

ll no_of_digits(ll n) {
    ll count = 0;
    while (n != 0) {
        n = n / 10;
        ++count;
    }
    return count;
}

vector<int> findDivisors(ll n) {
    vector<int> divisors;
    // Iterate from 1 up to the square root of number of digits of n
    for (int i = 1; i * i <= no_of_digits(n); ++i) {
        if (n % i == 0) {
            divisors.push_back(i);
            // Check if i and n/i are the same (e.g., for perfect squares)
            if (i * i != n) {
                // If not, add the other divisor n/i
                divisors.push_back(n / i);
            }
        }
    }
    std::sort(divisors.begin(), divisors.end());
    return divisors;
}

bool isInvalid(ll n, int k) {
    ll count = no_of_digits(n);
    if (count % k != 0) return false;
    ll tenX = 1;
    for (int i = 0; i < count / k; i++) tenX *= 10;
    ll first = n % tenX;
    for (int i = 1; i < k; i++) {
        n /= tenX;
        if ((n % tenX) != first) return false;
    }
    return true;
}

int main() {
    ifstream file("day2.txt", ios::binary);
    string s(
        (istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>()
    );
    vector<string> ranges = split(s, ',');
    ll res = 0;
    bool part2 = true;
    for (auto& range : ranges ) {
        auto vrange = split(range, '-');
        ll from = stoll(vrange[0]);
        ll to = stoll(vrange[1]);
        for (ll i = from; i <= to; ++i) {
            if (!part2) {
                if (isInvalid(i, 2))
                    res += i;
            } else {
                for (int k=2; k <= no_of_digits(i); ++k) {
                    if (isInvalid(i, k)) {
                        res += i;
                        break;
                    }
                }
            }
        }
    }
    cout << res << endl;
    return 0;
}