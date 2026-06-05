#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using ll = long long;

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

int solve2(const Problem& p) {
    int best = INT_MAX;
    vector<int> rem = p.joltage;

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
    // goal is to minimize x0+x1+x2+x3+x4+x5, let's call this `best`
    ll part2 = 0;
    cout << "Part 2: " << part2 << "\n";

    return 0;
}