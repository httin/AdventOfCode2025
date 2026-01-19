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

bool isInvalid(ll n) {
    ll count = no_of_digits(n);
    if (count % 2 == 1) return false;
    ll half = count / 2;
    ll tenX = 1;
    while (half > 0) {
        tenX = tenX * 10;
        --half;
    }
    ll firstHalf = n / tenX;
    ll secondHalf = n % tenX;
    return firstHalf == secondHalf;
}

int main() {
    ifstream file("day2.txt", ios::binary);
    string s(
        (istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>()
    );
    vector<string> ranges = split(s, ',');
    ll res = 0;
    for (auto& range : ranges ) {
        auto vrange = split(range, '-');
        ll from = stoll(vrange[0]);
        ll to = stoll(vrange[1]);
        for (ll i = from; i <= to; ++i) {
            if (isInvalid(i))
                res += i;
        }
        // cout << from << " " << to << endl;
    }
    cout << res << endl;
    return 0;
}