#include <bits/stdc++.h>
using namespace std;

struct Node {
    int val;
    int id;
    bool operator<(Node const& other) const {
        if (val != other.val) return val < other.val; // max-heap by val
        return id > other.id; // earlier id first -> larger priority for smaller id
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int q;
    if (!(cin >> q)) return 0;
    vector<char> served(q + 5, 0);
    queue<int> fifo;
    priority_queue<Node> heap;
    vector<int> output;
    int curId = 0;
    for (int i = 0; i < q; ++i) {
        int type;
        cin >> type;
        if (type == 1) {
            int m;
            cin >> m;
            ++curId;
            fifo.push(curId);
            heap.push({m, curId});
        } else if (type == 2) {
            while (!fifo.empty() && served[fifo.front()]) fifo.pop();
            int id = fifo.front();
            fifo.pop();
            served[id] = 1;
            output.push_back(id);
        } else { // type == 3
            while (!heap.empty() && served[heap.top().id]) heap.pop();
            Node nd = heap.top();
            heap.pop();
            served[nd.id] = 1;
            output.push_back(nd.id);
        }
    }
    for (size_t i = 0; i < output.size(); ++i) {
        if (i) cout << ' ';
        cout << output[i];
    }
    cout << '\n';
    return 0;
}