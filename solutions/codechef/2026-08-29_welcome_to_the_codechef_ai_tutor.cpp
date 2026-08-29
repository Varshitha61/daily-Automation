#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    int score1 = 0, score2 = 0;
    int maxLead = 0;
    int winner = 1;
    
    for (int i = 0; i < n; i++) {
        int s, t;
        cin >> s >> t;
        score1 += s;
        score2 += t;
        
        int lead = abs(score1 - score2);
        if (lead > maxLead) {
            maxLead = lead;
            winner = (score1 > score2) ? 1 : 2;
        }
    }
    
    cout << winner << " " << maxLead << endl;
    
    return 0;
}