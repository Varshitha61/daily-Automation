#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minMoves(vector<string>& classroom, int energy) {
        int m = classroom.size();
        int n = classroom[0].size();
        int sx=-1, sy=-1;
        vector<pair<int,int>> litter;
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                if(classroom[i][j]=='S'){
                    sx=i; sy=j;
                }else if(classroom[i][j]=='L'){
                    litter.push_back({i,j});
                }
            }
        }
        int L = litter.size();
        int fullMask = (1<<L)-1;
        // map position to litter index
        vector<vector<int>> litIdx(m, vector<int>(n, -1));
        for(int i=0;i<L;i++){
            litIdx[litter[i].first][litter[i].second]=i;
        }
        static unsigned char visited[20][20][1<<10][51];
        // reset visited for needed dimensions
        for(int i=0;i<m;i++)
            for(int j=0;j<n;j++)
                for(int mask=0; mask<(1<<L); ++mask)
                    memset(visited[i][j][mask], 0, energy+1);
        struct State{
            int x,y,mask,e,d;
        };
        queue<State> q;
        q.push({sx,sy,0,energy,0});
        visited[sx][sy][0][energy]=1;
        const int dx[4]={-1,1,0,0};
        const int dy[4]={0,0,-1,1};
        while(!q.empty()){
            State cur=q.front(); q.pop();
            if(cur.mask==fullMask) return cur.d;
            for(int dir=0;dir<4;dir++){
                int nx=cur.x+dx[dir];
                int ny=cur.y+dy[dir];
                if(nx<0||ny<0||nx>=m||ny>=n) continue;
                char cell=classroom[nx][ny];
                if(cell=='X') continue;
                if(cur.e==0) continue; // cannot move without energy
                int ne=cur.e-1;
                if(cell=='R') ne=energy;
                int nmask=cur.mask;
                if(cell=='L'){
                    int idx=litIdx[nx][ny];
                    if(idx!=-1) nmask|=(1<<idx);
                }
                if(!visited[nx][ny][nmask][ne]){
                    visited[nx][ny][nmask][ne]=1;
                    q.push({nx,ny,nmask,ne,cur.d+1});
                }
            }
        }
        return -1;
    }
};