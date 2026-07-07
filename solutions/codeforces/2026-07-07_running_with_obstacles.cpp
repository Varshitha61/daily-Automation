#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n, m, s, d;
    cin >> n >> m >> s >> d;

    vector<int> obstacles(n);
    for (int i = 0; i < n; i++) {
        cin >> obstacles[i];
    }

    sort(obstacles.begin(), obstacles.end());

    int current = 0;
    bool possible = true;

    for (int i = 0; i < n; i++) {
        if (obstacles[i] - current < s) {
            possible = false;
            break;
        }

        if (obstacles[i] - current >= s) {
            cout << "RUN " << s << endl;
            current += s;

            if (obstacles[i] - current > d) {
                possible = false;
                break;
            }

            cout << "JUMP " << obstacles[i] - current << endl;
            current = obstacles[i];
        }
    }

    if (!possible) {
        cout << "IMPOSSIBLE" << endl;
        return 0;
    }

    if (current + s <= m) {
        cout << "RUN " << s << endl;
        current += s;
    }

    if (current < m) {
        cout << "JUMP " << m - current << endl;
    }

    return 0;
}