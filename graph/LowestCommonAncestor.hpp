#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <iterator>
#include <stack>
#include <utility>
#include <vector>

namespace sora {

/***
  (隣接リスト形式)木 G に対する根を root として Lowest Common Ancestor を求める構造体
  Gを直接渡さず辺を逐次追加してからbuildしてもよい
  lca(u,v): u と v の LCA を求める。計算量 O(logn)
  前処理: 時間O(nlogn), 空間O(nlogn)
***/
class LowestCommonAncestor{
    int n,logn;
    std::vector<int> depth,parent_edge_id;
    std::vector<std::vector<std::pair<int,int>>> edges;
    std::vector<std::vector<int>> ancestor;

    public:
        // 頂点だけを確保する。時間・空間 O(N log(N+1))。
        LowestCommonAncestor(int vertex_count) : n(vertex_count), logn(std::bit_width((unsigned int)n)), depth(n,0),
            parent_edge_id(n,-1), edges(n), ancestor(logn,std::vector<int>(n,-1)) {}
        
        // 連結な無向木をコピーして構築。入力は変更しない。時間・空間 O(N log N)。
        LowestCommonAncestor(const std::vector<std::vector<int>>& g, int root=0) : LowestCommonAncestor(std::ssize(g)) {
            int edge_id=0;
            for(int u=0;u<n;u++){
                for(int v:g[u]){
                    if(u<v) add_edge(u,v,edge_id++);
                }
            }
            build(root);
        }
        
        // (u,v) 間に edge_id 番目の辺を張る。償却 O(1)。
        void add_edge(int u,int v,int edge_id){
            assert(0<=u and u<n);
            assert(0<=v and v<n);
            assert(0<=edge_id and edge_id<n-1);
            edges[u].emplace_back(v,edge_id);
            edges[v].emplace_back(u,edge_id);
        }

        // 連結な木を根 root で構築・再構築。時間 O(N log N)。N >= 1。
        void build(int root=0){
            assert(0<=root and root<n);
            for (auto& row : ancestor) std::fill(row.begin(), row.end(), -1);
            std::fill(depth.begin(), depth.end(), 0);
            std::fill(parent_edge_id.begin(), parent_edge_id.end(), -1);
            std::stack<int> st;
            st.push(root);
            while(st.size()){
                int cur=st.top();
                st.pop();
                for(auto [nxt,edge_id]:edges[cur]){
                    if(ancestor[0][nxt]!=cur and ancestor[0][cur]!=nxt){
                        ancestor[0][nxt]=cur;
                        parent_edge_id[nxt]=edge_id;
                        depth[nxt]=depth[cur]+1;
                        st.push(nxt);
                    }
                }
            }
            
            for(int k=1;k<logn;k++){
                for(int i=0;i<n;i++){
                    if(ancestor[k-1][i]==-1){
                        ancestor[k][i]=-1;
                    }else{
                        ancestor[k][i]=ancestor[k-1][ancestor[k-1][i]];
                    }
                }
            }
        }

        // 頂点vと、その親とを接続する辺の番号を返す。時間 O(1)。
        int get_parent_edge_id(int v) const {
            assert(0<=v and v<n);
            return parent_edge_id[v];
        }

        // 頂点vの親(1つ上の祖先)を返す。時間 O(1)。
        int get_parent(int v) const {
            assert(0<=v and v<n);
            return ancestor[0][v];
        }

        // 頂点vの深さを返す。時間 O(1)。
        int get_depth(int v) const {
            assert(0<=v and v<n);
            return depth[v];
        }

        // 頂点u,vのLCAを求めて返す O(logN)
        int lca(int u,int v) const {
            assert(0<=u and u<n);
            assert(0<=v and v<n);
            // vのほうがuより深いように正規化
            if(depth[u]>depth[v]) std::swap(u,v);
            // vの深さをuの深さと等しくなるよう、vの親を辿らせる
            for(int k=0;k<logn;k++){
                if(((depth[v]-depth[u])>>k) & 1){
                    v=ancestor[k][v];
                }
            }
            if(u==v) return u;
            // 二分探索でLCAを求める
            for(int k=logn;k--;){
                if(ancestor[k][u]!=ancestor[k][v]){
                    u=ancestor[k][u];
                    v=ancestor[k][v];
                }
            }
            return ancestor[0][u];
        }

        // (u,v) 間の距離を返す O(logN)
        int get_distance(int u,int v) const {
            assert(0<=u and u<n);
            assert(0<=v and v<n);
            return depth[u]+depth[v] - 2*depth[lca(u,v)];
        }

        // (u,v) 間のパスに頂点pが含まれるか。時間 O(log N)。
        int is_on_path(int u,int v,int p) const {
            assert(0<=u and u<n);
            assert(0<=v and v<n);
            assert(0<=p and p<n);
            return get_distance(u,p)+get_distance(p,v) == get_distance(u,v);
        }

        // 時間 O(log N)。頂点wから (u,v) 間のパスに含まれる頂点(端点含む) のうち最も近い頂点を返す
        // 辺の重みについての最短が欲しい場合もこれでok
        int closest_on_path(int u,int v,int w) const {
            const int a=lca(u,v);
            const int b=lca(u,w);
            const int c=lca(v,w);
            int x=a;
            if(depth[b]>depth[x]) x=b;
            if(depth[c]>depth[x]) x=c;
            return x;
        }

        // 実装してないけど、(u,v) 間パスに含まれる辺の最大(最小)コストとかもlogNで求められる
        // https://ikatakos.com/pot/programming_algorithm/graph_theory/lowest_common_ancestor
};

} // namespace sora
