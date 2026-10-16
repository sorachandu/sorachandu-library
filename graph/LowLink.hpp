#pragma once

#include <algorithm>
#include <utility>
#include <vector>

namespace sora {

/***
  LowLink　無向重みなしグラフについて関節点と橋を列挙
  Constructor: O(V+E)
  関節点の集合: std::vector<int> aps
  橋(の端点)の集合: std::vector<std::pair<int,int>> bridges
  reference: https://algo-logic.info/bridge-lowlink/
***/
// 無向単純グラフ用。入力は変更しない。入力グラフは本オブジェクトより長く生存すること。
// 多重辺には非対応。再帰深さは最大 O(V)、入力を除く空間 O(V)。
struct LowLink{
    using Graph=std::vector<std::vector<int>>;
    const Graph &g;
    std::vector<int> used,ord,low;
    std::vector<int> aps;
    std::vector<std::pair<int,int>> bridges;
    
    LowLink(const Graph& graph) : g(graph){
        const int n=g.size();
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

private:
    int dfs(int p,int k,int par){
        used[p]=true;
        ord[p]=k++;
        low[p]=ord[p];
        bool is_aps=false;
        int cnt=0;
        for(auto &q:g[p]){
            if(!used[q]){
                cnt++;
                k=dfs(q,k,p);
                low[p]=std::min(low[p],low[q]);
                if(par!=-1 and ord[p]<=low[q]) is_aps=true;
                if(ord[p]<low[q]) bridges.emplace_back(std::minmax(p,q));
            }else if(q!=par){ // {p,q}の辺が後退辺
                low[p]=std::min(low[p],ord[q]);
            }
        }
        if(par==-1 and cnt>=2) is_aps=true;
        if(is_aps) aps.emplace_back(p);
        return k;
    }
};

} // namespace sora
