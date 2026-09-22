#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    struct Node {
        int prod;
        int cnt[5];
        Node(int K=5){ prod=1%K; for(int i=0;i<5;i++) cnt[i]=0; }
    };
    int K;
    vector<Node> seg;
    int n;
    
    Node mergeNode(const Node& L, const Node& R){
        Node res(K);
        res.prod = (int)((1LL*L.prod*R.prod)%K);
        for(int i=0;i<K;i++) res.cnt[i]=L.cnt[i];
        for(int i=0;i<K;i++){
            int newmod = (int)((1LL*L.prod*i)%K);
            res.cnt[newmod] += R.cnt[i];
        }
        return res;
    }
    
    void build(const vector<int>& arr, int idx, int l, int r){
        if(l==r){
            seg[idx].prod = arr[l]%K;
            for(int i=0;i<K;i++) seg[idx].cnt[i]=0;
            seg[idx].cnt[seg[idx].prod]=1;
            return;
        }
        int mid=(l+r)/2;
        build(arr, idx*2, l, mid);
        build(arr, idx*2+1, mid+1, r);
        seg[idx]=mergeNode(seg[idx*2], seg[idx*2+1]);
    }
    
    void pointUpdate(int pos, int val, int idx, int l, int r){
        if(l==r){
            seg[idx].prod = val%K;
            for(int i=0;i<K;i++) seg[idx].cnt[i]=0;
            seg[idx].cnt[seg[idx].prod]=1;
            return;
        }
        int mid=(l+r)/2;
        if(pos<=mid) pointUpdate(pos,val,idx*2,l,mid);
        else pointUpdate(pos,val,idx*2+1,mid+1,r);
        seg[idx]=mergeNode(seg[idx*2], seg[idx*2+1]);
    }
    
    Node rangeQuery(int ql, int qr, int idx, int l, int r){
        if(ql<=l && r<=qr) return seg[idx];
        int mid=(l+r)/2;
        if(qr<=mid) return rangeQuery(ql,qr,idx*2,l,mid);
        if(ql>mid) return rangeQuery(ql,qr,idx*2+1,mid+1,r);
        Node left = rangeQuery(ql,qr,idx*2,l,mid);
        Node right = rangeQuery(ql,qr,idx*2+1,mid+1,r);
        return mergeNode(left,right);
    }
    
    vector<int> findXValue(vector<int>& nums, int k, vector<vector<int>>& queries) {
        K = k;
        n = nums.size();
        seg.assign(4*n, Node(K));
        build(nums,1,0,n-1);
        vector<int> ans;
        for(auto &q: queries){
            int idx = q[0];
            int val = q[1];
            int start = q[2];
            int x = q[3];
            pointUpdate(idx,val,1,0,n-1);
            Node res = rangeQuery(start,n-1,1,0,n-1);
            ans.push_back(res.cnt[x]);
        }
        return ans;
    }
};