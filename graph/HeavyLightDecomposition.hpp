#pragma once

#include <cassert>
#include <iterator>
#include <utility>
#include <vector>

namespace sora {

// 木の重軽分解。祖先・距離・パス上の移動を処理する。
// 参考: https://info.atcoder.jp/entry/algorithm_lectures/heavy_light_decomposition
// https://nachiavivias.github.io/cp-library/cpp/tree/heavy-light-decomposition.html
struct HeavyLightDecomposition{
private:
    int n;
    std::vector<int> par;
    std::vector<int> dep;
    std::vector<int> siz;
    std::vector<int> vertex;
    std::vector<int> id;
    std::vector<int> head;

public:
    // 連結な無向木を重軽分解する。時間・空間O(N)。入力をコピーし、元の木は変更しない。
    // N >= 1、rootと全ての頂点番号は[0, N)。再帰深さは最悪O(N)。
    HeavyLightDecomposition(std::vector<std::vector<int>> g,int root=0){
        n=std::ssize(g);
        assert(0 <= root && root < n);
        par.assign(n,-1);
        dep.assign(n,-1);
        siz.assign(n,0);
        vertex.assign(n,-1);
        id.assign(n,-1);
        head.assign(n,-1);
        {
            // 部分木サイズを求め、部分木サイズが最大の子を隣接リストの先頭へ移す。
            auto dfs=[&](auto &dfs, int v, int p=-1) -> void {
                par[v]=p;
                dep[v]=(p==-1 ? 0 : dep[p]+1);
                siz[v]=1;
                if(!g[v].empty() and g[v][0]==p) std::swap(g[v][0],g[v].back());
                for(int &w:g[v]){
                    if(w==p) continue;
                    dfs(dfs,w,v);
                    siz[v]+=siz[w];
                    if(siz[w]>siz[g[v][0]]) std::swap(g[v][0],w);
                }
            };
            dfs(dfs,root);
        }
        {
            int idx=0;
            auto dfs=[&](auto &dfs,int v,int p=-1) -> void {
                id[v]=idx;
                vertex[idx++]=v;
                for(int &w:g[v]){
                    if(w==p) continue;
                    bool heavy=(w==g[v][0]);
                    head[w]=(heavy?head[v]:w);
                    dfs(dfs,w,v);
                }
            };
            head[root]=root;
            dfs(dfs,root);
        }
    }

    // 時間O(1)。
    int num_vertices() const { return n; }
    // 頂点vの深さ (dep) O(1)。
    int depth(int v) const { return dep[v]; }
    // 頂点vがHLD配列上で何番目か (id) O(1)。
    int to_seq(int v) const { return id[v]; }
    // HLD配列[seqidx]の頂点番号 (vertex) O(1)。
    int to_vertex(int seqidx) const { return vertex[seqidx]; }
    // 頂点vの親 (根なら-1) O(1)。
    int parent(int v) const { return par[v]; }
    // vを含む重いパスの先頭頂点。 O(1)。
    int heavy_root(int v) const { return head[v]; }
    // vの重い子。存在しなければ-1。 O(1)。
    int heavy_child(int v) const {
        if(to_seq(v)==n-1) return -1;
        int cand=to_vertex(to_seq(v)+1);
        if(same(v,cand)) return cand;
        return -1;
    }

    // uとvが同じ重いパスに属するか。 O(1)。
    bool same(int u,int v) const { return head[u]==head[v]; }
    // 頂点uと頂点vについてHLD配列上でu<vを成立させる (外部クエリ処理用) O(1)。
    void normalize_pair(int &u,int &v){ if(id[u]>id[v]) std::swap(u,v); }
    // 重いパスの先頭からvまでの距離。 O(1)。
    int dist_to_heavy_root(int v) const { return id[v]-id[head[v]]; }
    // vを含む重いパスの先頭から、頂点番号列を返す。
    // stop_at_vertex=trueならvで止まり、falseならパス末尾まで返す。
    // 返す頂点数をKとして、時間・出力空間O(K)。
    std::vector<int> heavy_path(int v,bool stop_at_vertex=false) const {
        std::vector<int> path;
        int idx=head[v];
        while(idx!=-1){
            path.emplace_back(idx);
            if(stop_at_vertex and idx==v) break; 
            idx=heavy_child(idx);
        }
        return path;
    }
    // Level Ancestor　頂点vの祖先のうち、深さdにある頂点の番号を返す O(logN)
    int la(int v,int d) const {
        assert(0 <= d && d <= dep[v]);
        while(dep[head[v]]>d){
            v=par[head[v]];
        }
        return vertex[id[v]-(dep[v]-d)];
    }
    // Lowest Common Ancestor　頂点u,vの最近共通祖先の頂点番号を返す O(logN)
    int lca(int u,int v) const {
        while(!same(u,v)){
            if(id[u]>id[v]) std::swap(u,v);
            v=par[head[v]];
        }
        return (id[u] < id[v] ? u:v);
    }
    // 任意の2頂点u,vの距離を返す　O(logN)
    int dist(int u,int v) const {
        const int w=lca(u,v);
        return (dep[u]-dep[w])+(dep[v]-dep[w]);
    }
    // Jump on Tree　2頂点s,t及び非負整数iについて、
    // パスstにおけるi番目の頂点番号を返す　O(logN)
    int jump(int u,int v,int i) const {
        assert(i >= 0);
        const int w=lca(u,v);
        const int d=(dep[u]-dep[w])+(dep[v]-dep[w]);
        if(d<i) return -1;
        if(i < dep[u]-dep[w]) return la(u,dep[u]-i);
        return la(v,dep[v]-(d-i));
    }
};

} // namespace sora
