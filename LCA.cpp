#include<bits/stdc++.h>
using namespace std;




/*  (隣接リスト形式)木 G に対する根を root として Lowest Common Ancestor を求める構造体
    Gを直接渡さず辺を逐次追加してからbuildしてもよい
    lca(u,v): u と v の LCA を求める。計算量 O(logn)
    前処理: 時間O(nlogn), 空間O(nlogn)
*/
class LCA{
    int n,logn;
    vector<int> depth,parent_edge_id;
    vector<vector<pair<int,int>>> edges;
    vector<vector<int>> ancestor;

    public:
        LCA(int _n) : n(_n), logn(bit_width(uint(n))), depth(n,0),
            parent_edge_id(n,-1), edges(n), ancestor(logn,vector<int>(n,-1)) {}
        
        LCA(vector<vector<int>> &G, int root=0) : LCA(ssize(G)) {
            int edge_id=0;
            for(int u=0;u<n;u++){
                for(int v:G[u]){
                    if(u<v) add_edge(u,v,edge_id++);
                }
            }
            build(root);
        }
        
        // (u,v) 間に edge_id 番目の辺を張る
        inline void add_edge(int u,int v,int edge_id){
            assert(0<=u and u<n);
            assert(0<=v and v<n);
            assert(0<=edge_id and edge_id<n-1);
            edges[u].emplace_back(v,edge_id);
            edges[v].emplace_back(u,edge_id);
        }

        // 根 root の頂点番号を渡して、構築 (by doubling)
        inline void build(int root=0){
            assert(0<=root and root<n);
            stack<int> st;
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

        // 頂点vと、その親とを接続する辺の番号を返す
        inline int get_parent_edge_id(int v){
            assert(0<=v and v<n);
            return parent_edge_id[v];
        }

        // 頂点vの親(1つ上の祖先)を返す
        inline int get_parent(int v){
            assert(0<=v and v<n);
            return ancestor[0][v];
        }

        // 頂点vの深さを返す
        inline int get_depth(int v){
            assert(0<=v and v<n);
            return depth[v];
        }

        // 頂点u,vのLCAを求めて返す O(logN)
        inline int lca(int u,int v){
            assert(0<=u and u<n);
            assert(0<=v and v<n);
            // vのほうがuより深いように正規化
            if(depth[u]>depth[v]) swap(u,v);
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
        inline int get_distance(int u,int v){
            assert(0<=u and u<n);
            assert(0<=v and v<n);
            return depth[u]+depth[v] - 2*depth[lca(u,v)];
        }

        // (u,v) 間のパスに頂点pが含まれるか
        inline int is_on_path(int u,int v,int p){
            assert(0<=u and u<n);
            assert(0<=v and v<n);
            assert(0<=p and p<n);
            return get_distance(u,p)+get_distance(p,v) == get_distance(u,v);
        }

        // 実装してないけど、(u,v) 間パスに含まれる辺の最大(最小)コストとかもlogNで求められる
        // https://ikatakos.com/pot/programming_algorithm/graph_theory/lowest_common_ancestor
};