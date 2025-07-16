#include<bits/stdc++.h>
using namespace std;

/*--------------------

// 無向根付き木に対して各頂点の行きがけ,帰りがけの時刻を記録
// idxsには辺番号が並ぶ 頂点iの根側の辺を辺iとして、+iなら上から下、-iなら下から上への移動
// 最初はpにrootを渡すこと あとin,out,idxs,time=0は外で作っておく 適宜機能を追加して使うべし
// Refer to: https://maspypy.com/euler-tour-%E3%81%AE%E3%81%8A%E5%8B%89%E5%BC%B7
auto EulerTour=[&](auto self,int p=0) -> void {
    in[p]=time++;
    idxs.emplace_back(p);
    for(auto q:g[p]){
        if(in[q]!=-1) continue;
        self(self,q);
    }
    out[p]=time++;
    idxs.emplace_back(-p);
};

---------------------*/

//-----------How To Use?------------------
int main(){
    int n;
    cin>>n;
    vector g(n,vector<int>());
    for(int i=0;i<n-1;i++){
        int u,v;
        cin>>u>>v;
        u--,v--;
        g[u].emplace_back(v);
        g[v].emplace_back(u);
    }

    vector<int> in(n,-1),out(n,-1),idxs;
    int time=0;
    auto EulerTour=[&](auto self,int p=0) -> void {
        in[p]=time++;
        idxs.emplace_back(p);
        for(auto q:g[p]){
            if(in[q]!=-1) continue;
            self(self,q);
        }
        out[p]=time++;
        idxs.emplace_back(-p);
    };
    EulerTour(EulerTour);
}