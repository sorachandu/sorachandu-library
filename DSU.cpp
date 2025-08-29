#include<bits/stdc++.h>
using namespace std;
// 全然たたき台でしかないけど、一応保存しておく



// 代表元に「最も根に近いもの」という性質を持たせるため、自作
// 経路圧縮はするけどUnion by Sizeはしない、再帰のやつ
class UnionFind{
    vector<int> par,siz;
    
    public:
        UnionFind(int n) : par(n,-1),siz(n,1) {}
        
        inline int root(int v){
            return (~par[v] ? par[v]=root(par[v]) : v);
        }

        // u側に、vを併合
        inline void merge(int u,int v){
            u=root(u);
            v=root(v);
            if(u!=v){
                par[v]=u;
                siz[u]+=siz[v];
            }
        }
        
        inline bool same(int u,int v){ return root(u)==root(v); }
};