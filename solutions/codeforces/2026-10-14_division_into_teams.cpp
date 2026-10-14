#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if(!(cin >> n)) return 0;
    vector<pair<int,int>> v(n);
    for(int i=0;i<n;i++){
        cin >> v[i].first;
        v[i].second = i+1;
    }
    sort(v.begin(), v.end(), [](const auto& a, const auto& b){
        return a.first > b.first;
    });
    vector<int> team1, team2;
    for(int i=0;i<n;i++){
        if(i%2==0) team1.push_back(v[i].second);
        else team2.push_back(v[i].second);
    }
    cout << team1.size() << "\n";
    for(int id: team1) cout << id << " ";
    cout << "\n";
    cout << team2.size() << "\n";
    for(int id: team2) cout << id << " ";
    cout << "\n";
    return 0;
}