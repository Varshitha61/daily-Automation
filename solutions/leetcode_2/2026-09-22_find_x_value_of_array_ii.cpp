#include <bits/stdc++.h>
using namespace std;

struct Node {
    int prod; // product modulo K of the whole segment
    long long cnt[5][5]; // cnt[s][e]: number of non‑empty prefixes inside segment, starting from remainder s, ending at e
    Node(int K=1){ prod = 1%K; for(int i=0;i<K;i++) for(int j=0;j<K;j++) cnt[i][j]=0; }
};

int K;

Node mergeNode(const Node& A, const Node& B){
    Node res(K);
    res.prod = (A.prod * B.prod) % K;
    for(int s=0;s<K;s++){
        for(int e=0;e<K;e++){
            long long val = A.cnt[s][e];
            int mid = (s * A.prod) % K;
            val += B.cnt[mid][e];
            res.cnt[s][e] = val;
        }
    }
    return res;
}

struct SegTree{
    int n;
    vector<Node> seg;
    SegTree(const vector<int>& arr){
        n = arr.size();
        seg.assign(4*n, Node(K));
        build(1,0,n-1,arr);
    }
    void build(int idx,int l,int r,const vector<int>& arr){
        if(l==r){
            seg[idx]=Node(K);
            seg[idx].prod = arr[l]%K;
            for(int s=0;s<K;s++){
                int e = (s * seg[idx].prod) % K;
                seg[idx].cnt[s][e] = 1;
            }
            return;
        }
        int mid=(l+r)/2;
        build(idx*2,l,mid,arr);
        build(idx*2+1,mid+1,r,arr);
        seg[idx]=mergeNode(seg[idx*2],seg[idx*2+1]);
    }
    void pointUpdate(int pos,int val){ update(1,0,n-1,pos,val%K); }
    void update(int idx,int l,int r,int pos,int val){
        if(l==r){
            seg[idx]=Node(K);
            seg[idx].prod = val%K;
            for(int s=0;s<K;s++){
                int e = (s * seg[idx].prod) % K;
                seg[idx].cnt[s][e] = 1;
            }
            return;
        }
        int mid=(l+r)/2;
        if(pos<=mid) update(idx*2,l,mid,pos,val);
        else update(idx*2+1,mid+1,r,pos,val);
        seg[idx]=mergeNode(seg[idx*2],seg[idx*2+1]);
    }
    Node queryRange(int L,int R){ return query(1,0,n-1,L,R); }
    Node query(int idx,int l,int r,int L,int R){
        if(L<=l && r<=R) return seg[idx];
        int mid=(l+r)/2;
        if(R<=mid) return query(idx*2,l,mid,L,R);
        if(L>mid) return query(idx*2+1,mid+1,r,L,R);
        Node left = query(idx*2,l,mid,L,R);
        Node right = query(idx*2+1,mid+1,r,L,R);
        return mergeNode(left,right);
    }
};

vector<int> parseInts(const string& s){
    vector<int> res;
    int num=0; bool in=false,neg=false;
    for(char c: s){
        if(c=='-'){neg=true;}
        if(isdigit(c)){
            num = num*10 + (c-'0');
            in=true;
        }else{
            if(in){
                res.push_back(neg?-num:num);
                num=0; in=false; neg=false;
            }
        }
    }
    if(in) res.push_back(neg?-num:num);
    return res;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string line;
    if(!getline(cin,line)) return 0;
    vector<int> nums = parseInts(line);
    getline(cin,line);
    K = stoi(line);
    getline(cin,line);
    vector<int> all = parseInts(line);
    int q = all.size()/4;
    vector<array<int,4>> queries(q);
    for(int i=0;i<q;i++){
        queries[i] = {all[4*i], all[4*i+1], all[4*i+2], all[4*i+3]};
    }
    vector<int> arrMod(nums.size());
    for(size_t i=0;i<nums.size();i++) arrMod[i]=nums[i]%K;
    SegTree st(arrMod);
    vector<long long> ans;
    for(auto &qr: queries){
        int idx=qr[0], val=qr[1], start=qr[2], x=qr[3];
        st.pointUpdate(idx,val);
        Node res = st.queryRange(start, (int)nums.size()-1);
        ans.push_back(res.cnt[1%K][x]);
    }
    for(size_t i=0;i<ans.size();i++){
        if(i) cout<<' ';
        cout<<ans[i];
    }
    cout<<"\n";
    return 0;
}