#include<bits/stdc++.h>
using namespace std;

/*
LowLink　無向重みなしグラフについて関節点と橋を列挙
Constructor: O(V+E)
関節点の集合: vector<int> aps
橋(の端点)の集合: vector<pair<int,int>> bridges
reference: https://algo-logic.info/bridge-lowlink/
*/
struct LowLink{
    using Graph=vector<vector<int>>;
    const Graph &G;
    vector<int> used,ord,low;
    vector<int> aps;
    vector<pair<int,int>> bridges;
    
    LowLink(const Graph &G_) : G(G_){
        const int n=G.size();
        used.assign(n,0);
        ord.assign(n,0);
        low.assign(n,0);
        int k=0;
        for(int i=0;i<n;i++){
            if(!used[i]) k=dfs(i,k,-1);
        }
        // ranges::sort(aps);
        // ranges::sort(bridges);
    }

    int dfs(int p,int k,int par){
        used[p]=true;
        ord[p]=k++;
        low[p]=ord[p];
        bool is_aps=false;
        int cnt=0;
        for(auto &q:G[p]){
            if(!used[q]){
                cnt++;
                k=dfs(q,k,p);
                low[p]=min(low[p],low[q]);
                if(par!=-1 and ord[p]<=low[q]) is_aps=true;
                if(ord[p]<low[q]) bridges.emplace_back(minmax(p,q));
            }else if(q!=par){ // {p,q}の辺が後退辺
                low[p]=min(low[p],ord[q]);
            }
        }
        if(par==-1 and cnt>=2) is_aps=true;
        if(is_aps) aps.emplace_back(p);
        return k;
    }
};