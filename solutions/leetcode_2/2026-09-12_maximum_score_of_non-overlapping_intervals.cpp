#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> maxScoreIndices(vector<vector<int>>& intervals) {
        int n = intervals.size();
        struct Node{
            long long l,r,w;
            int idx;
        };
        vector<Node> a;
        a.reserve(n);
        for(int i=0;i<n;i++){
            a.push_back({intervals[i][0], intervals[i][1], intervals[i][2], i});
        }
        sort(a.begin(), a.end(), [](const Node& A, const Node& B){
            if (A.r != B.r) return A.r < B.r;
            return A.l < B.l;
        });
        vector<long long> ends(n+1);
        for(int i=1;i<=n;i++) ends[i]=a[i-1].r;
        vector<int> p(n+1,0);
        for(int i=1;i<=n;i++){
            long long start = a[i-1].l;
            int lo=1, hi=i-1, ans=0;
            while(lo<=hi){
                int mid=(lo+hi)/2;
                if(ends[mid] < start){
                    ans=mid;
                    lo=mid+1;
                }else hi=mid-1;
            }
            p[i]=ans;
        }
        const long long NEG = -4e18;
        vector<array<long long,5>> dpW(n+1);
        vector<array<vector<int>,5>> dpS(n+1);
        for(int c=0;c<=4;c++){
            dpW[0][c]=0;
            dpS[0][c]=vector<int>();
        }
        for(int i=1;i<=n;i++){
            for(int c=0;c<=4;c++){
                dpW[i][c]=dpW[i-1][c];
                dpS[i][c]=dpS[i-1][c];
            }
            for(int c=1;c<=4;c++){
                long long prevW = dpW[p[i]][c-1];
                if(prevW==NEG) continue;
                long long candW = prevW + a[i-1].w;
                if(candW > dpW[i][c]){
                    dpW[i][c]=candW;
                    dpS[i][c]=dpS[p[i]][c-1];
                    dpS[i][c].push_back(a[i-1].idx);
                    sort(dpS[i][c].begin(), dpS[i][c].end());
                }else if(candW == dpW[i][c]){
                    vector<int> cand = dpS[p[i]][c-1];
                    cand.push_back(a[i-1].idx);
                    sort(cand.begin(), cand.end());
                    if(cand < dpS[i][c]){
                        dpS[i][c]=move(cand);
                    }
                }
            }
        }
        long long bestW = -1;
        vector<int> bestSeq;
        for(int c=0;c<=4;c++){
            long long w = dpW[n][c];
            if(w > bestW){
                bestW = w;
                bestSeq = dpS[n][c];
            }else if(w == bestW){
                if(dpS[n][c] < bestSeq) bestSeq = dpS[n][c];
            }
        }
        return bestSeq;
    }
};