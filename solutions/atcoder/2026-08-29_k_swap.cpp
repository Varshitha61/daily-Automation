#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, K;
    cin >> N >> K;
    
    vector<long long> A(N);
    for (int i = 0; i < N; i++) {
        cin >> A[i];
    }
    
    vector<long long> sorted_A = A;
    sort(sorted_A.begin(), sorted_A.end());
    
    // We can swap a[i] and a[i+K] for any i from 0 to N-K-1
    // This means elements at positions i and i+K can be swapped.
    // The positions form chains based on their index modulo K.
    // Specifically, position i can only be swapped with positions i+K, i+2K, etc.
    // So all positions with the same index modulo K form a connected component.
    // Within each component, we can permute the elements arbitrarily.
    
    // For each residue class r (0 <= r < K), collect all elements at positions r, r+K, r+2K, ...
    // Sort these elements.
    // Then check if the sorted array matches the target sorted array at those positions.
    
    for (int r = 0; r < K; r++) {
        vector<long long> elements;
        for (int i = r; i < N; i += K) {
            elements.push_back(A[i]);
        }
        sort(elements.begin(), elements.end());
        
        int idx = 0;
        for (int i = r; i < N; i += K) {
            if (elements[idx] != sorted_A[i]) {
                cout << "No" << endl;
                return 0;
            }
            idx++;
        }
    }
    
    cout << "Yes" << endl;
    return 0;
}