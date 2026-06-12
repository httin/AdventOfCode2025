#include <algorithm>
#include <cassert>
#include <climits>
#include <fstream>
#include <functional>
#include <numeric>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using ll = long long;

// exact rational, always stored in lowest terms with denominator > 0
struct Frac {
    ll num, den;
    Frac(ll n = 0, ll d = 1) {
        if (d < 0) { n = -n; d = -d; }
        ll g = gcd(n < 0 ? -n : n, d);
        if (g == 0) g = 1;
        num = n / g; den = d / g;
    }
    Frac operator+(const Frac& o) const { return Frac(num*o.den + o.num*den, den*o.den); }
    Frac operator-(const Frac& o) const { return Frac(num*o.den - o.num*den, den*o.den); }
    Frac operator*(const Frac& o) const { return Frac(num*o.num, den*o.den); }
    Frac operator/(const Frac& o) const { return Frac(num*o.den, den*o.num); }
    bool isZero() const { return num == 0; }
    bool isInt()  const { return den == 1; }
};

struct Problem {
    string light;
    vector<vector<int>> buttons;
    vector<int> joltage;

    // index: 0123456789
    //        #         -> 1
    //        #..       -> 1
    //        ##.       -> 011
    //        .##.#     -> 10110
    //        .###.#    -> 101110
    int light_to_int() {
        int res = 0;
        for (size_t i = 0; i < light.length(); i++) {
            if (light[i] == '#')
                res |= (1 << i); // set i(th) bit
        }
        return res;
    }
    void Log() const {
        cout << "[" << light << "] [";
        for (const auto& b : buttons) {
            cout << " [";
            const char* sep = "";
            for (int x : b) { cout << sep << x; sep = ","; }
            cout << "]";
        }
        cout << "] {";
        const char* sep = "";
        for (int x : joltage) { cout << sep << x; sep = ","; }
        cout << "}\n";
    }
};

int solve1(vector<Problem>& problems) {
    ll res = 0;
    for (auto& p : problems) {
        int target = p.light_to_int();
        int minPress = INT_MAX;
        int n = static_cast<int>(p.buttons.size());
        for (int mask = 1; mask < (1<<n); mask++) { // loop through all non-empty subsets of buttons
            int num = 0, numPress = 0;
            for (int i = 0; i < n; i++) {
                if (mask & (1 << i)) {
                    numPress++;
                    for (const auto& button: p.buttons[i]) {  // toggle i(th) button
                        num ^= (1 << button);
                    }
                }
            }
            if (num == target) minPress = min(minPress, numPress);
        }
        // cout << "target=" << target << ", minPress=" << minPress << endl;
        res += minPress;
    };
    return res;
}

// Minimize total presses to hit every counter's joltage exactly.
// Model: x_b >= 0 = presses of button b; for each counter c, sum of x_b over
// buttons touching c must equal joltage[c]. Minimize sum(x_b).
// The equality system A*x = joltage has a low-dimensional solution space
// (free dim = n - rank <= 3 here): row-reduce to express each basic button as
// an affine function of the few free buttons, then enumerate the free ones.
ll solve2(const Problem& p) {
    int m = static_cast<int>(p.joltage.size());   // counters (rows)
    int n = static_cast<int>(p.buttons.size());   // buttons  (cols)

    // augmented matrix [A | joltage], rows = counters, cols = buttons
    vector<vector<Frac>> M(m, vector<Frac>(n + 1, Frac(0)));
    for (int c = 0; c < m; c++) M[c][n] = Frac(p.joltage[c]);
    for (int b = 0; b < n; b++)
        for (int c : p.buttons[b]) M[c][b] = Frac(1);

    // Gauss-Jordan elimination -> reduced row echelon form
    vector<bool> isPivot(n, false);
    int rank = 0;
    for (int col = 0; col < n && rank < m; col++) {
        int sel = -1;
        for (int r = rank; r < m; r++) if (!M[r][col].isZero()) { sel = r; break; }
        if (sel < 0) continue;
        swap(M[rank], M[sel]);
        Frac pv = M[rank][col];
        for (int j = 0; j <= n; j++) M[rank][j] = M[rank][j] / pv;
        for (int r = 0; r < m; r++)
            if (r != rank && !M[r][col].isZero()) {
                Frac f = M[r][col];
                for (int j = 0; j <= n; j++) M[r][j] = M[r][j] - f * M[rank][j];
            }
        isPivot[col] = true;
        rank++;
    }

    // consistency: a zeroed-out row with nonzero RHS means no solution
    for (int r = rank; r < m; r++)
        if (!M[r][n].isZero()) return LLONG_MAX;

    // free buttons + their upper bounds (a button can't be pressed more than the
    // smallest joltage among counters it touches)
    vector<int> freeCols, ub;
    for (int c = 0; c < n; c++)
        if (!isPivot[c]) {
            freeCols.push_back(c);
            int best = INT_MAX;
            for (int cnt : p.buttons[c]) best = min(best, p.joltage[cnt]);
            ub.push_back(best);
        }

    // enumerate free buttons; each pivot row r gives
    //   x_pivot = M[r][n] - sum_{free fc} M[r][fc] * x_fc
    ll best = LLONG_MAX;
    vector<int> freeVal(freeCols.size());
    function<void(int)> rec = [&](int k) {
        if (k == static_cast<int>(freeCols.size())) {
            ll total = 0;
            for (int v : freeVal) total += v;
            for (int r = 0; r < rank; r++) {
                Frac v = M[r][n];
                for (size_t i = 0; i < freeCols.size(); i++)
                    v = v - M[r][freeCols[i]] * Frac(freeVal[i]);
                if (!v.isInt() || v.num < 0) return;  // not a valid non-negative integer
                total += v.num;
            }
            best = min(best, total);
            return;
        }
        for (int v = 0; v <= ub[k]; v++) { freeVal[k] = v; rec(k + 1); }
    };
    rec(0);
    return best;
}

int main() {
    ifstream file("day10.txt");
    assert(file.is_open());

    vector<Problem> problems;
    string line;
    while (getline(file, line)) {
        //   [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
        //   ^    ^     ^   ^                       ^       ^
        //  lsb  rsb    op  cp                     lcb     rcb
        size_t lsb, rsb, lcb, rcb;
        lsb = line.find('['), rsb = line.find(']');
        lcb = line.find('{'), rcb = line.find('}');
        assert(lsb == 0), assert(rcb == line.size() - 1);
        string light = line.substr(lsb + 1, rsb - lsb - 1);

        vector<int> joltage;
        stringstream js(line.substr(lcb + 1, rcb - lcb - 1));
        for (string tok; getline(js, tok, ',');) joltage.push_back(stoi(tok));

        vector<vector<int>> buttons;
        for (size_t i = rsb + 1; i < lcb; ) {
            size_t op = line.find('(', i);
            if (op == string::npos) break;
            assert(op < lcb);
            size_t cp = line.find(')', op);
            assert(cp < lcb);
            vector<int> button;
            stringstream bs(line.substr(op + 1, cp - op - 1));
            for (string tok; getline(bs, tok, ',');) button.push_back(stoi(tok));
            buttons.push_back(button);
            i = cp + 1;
        }
        problems.push_back(Problem{light, buttons, joltage});
    }

    // Part 1: find the minimum button presses
    cout << "Part 1: " << solve1(problems) << "\n";

    // Part 2: configure joltage level for each button with minimum button presses
    // counter: 0  1  2  3
    // joltage: 3  5  4  7
    // x0                1 (3)
    // x1          1     1 (1,3)
    // x2             1    (2)
    // x3             1  1 (2,3)
    // x4       1     1    (0,2)
    // x5       1  1       (0,1)
    //
    // x_b = times button b is pressed, x_b >= 0
    // counter 0 is touched only by x4 and x5 → x4 + x5 = 3
    // counter 1 is touched only by x1 and x5 → x1 + x5 = 5
    // counter 2 is touched by x2, x4, x4     → x2 + x3 + x4 = 4
    // counter 3 is touched by x0, x1, x4     → x0 + x1 + x3 = 7
    // Matrix equation: A·x = joltage (counters × buttons, counters is 0/1 matrix, buttons is variables)
    // goal is to minimize x0+x1+x2+x3+x4+x5, this is linear programming problem
    ll part2 = 0;
    for (const auto& p : problems) part2 += solve2(p);
    cout << "Part 2: " << part2 << "\n";

    return 0;
}