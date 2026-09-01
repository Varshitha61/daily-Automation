#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minMoves(vector<string> classroom, int energy) {
        int m = classroom.size();
        int n = classroom[0].size();
        int sx=-1, sy=-1;
        vector<pair<int,int>> litterPos;
        vector<vector<int>> litterIdx(m, vector<int>(n, -1));
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                char c = classroom[i][j];
                if(c=='S'){
                    sx=i; sy=j;
                }else if(c=='L'){
                    litterIdx[i][j] = litterPos.size();
                    litterPos.push_back({i,j});
                }
            }
        }
        int k = litterPos.size();
        int allMask = (1<<k)-1;
        int totalCells = m*n;
        int maskCount = 1<<k;
        int eMax = energy;
        long long totalStates = 1LL*totalCells*maskCount*(eMax+1);
        vector<char> visited(totalStates, 0);
        auto idx = [&](int x,int y,int mask,int e)->size_t{
            int cellId = x*n + y;
            return ((size_t)cellId*maskCount + mask)*(eMax+1) + e;
        };
        struct Node{
            int x,y,mask,e,dist;
        };
        queue<Node> q;
        visited[idx(sx,sy,0,energy)] = 1;
        q.push({sx,sy,0,energy,0});
        const int dx[4]={-1,1,0,0};
        const int dy[4]={0,0,-1,1};
        while(!q.empty()){
            Node cur = q.front(); q.pop();
            if(cur.mask==allMask) return cur.dist;
            for(int dir=0;dir<4;dir++){
                int nx = cur.x + dx[dir];
                int ny = cur.y + dy[dir];
                if(nx<0||nx>=m||ny<0||ny>=n) continue;
                char cell = classroom[nx][ny];
                if(cell=='X') continue;
                int ne = cur.e - 1;
                if(ne<0) continue;
                if(cell=='R') ne = energy;
                int nmask = cur.mask;
                if(cell=='L'){
                    int id = litterIdx[nx][ny];
                    if(id!=-1) nmask |= (1<<id);
                }
                size_t id = idx(nx,ny,nmask,ne);
                if(!visited[id]){
                    visited[id]=1;
                    q.push({nx,ny,nmask,ne,cur.dist+1});
                }
            }
        }
        return -1;
    }
};