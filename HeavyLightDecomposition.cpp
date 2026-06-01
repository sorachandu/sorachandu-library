/*//--------------------------------------------------------
木に対して重軽分解をする構造体
refer:
https://info.atcoder.jp/entry/algorithm_lectures/heavy_light_decomposition
https://nachiavivias.github.io/cp-library/cpp/tree/heavy-light-decomposition.html
*///--------------------------------------------------------
struct HeavyLightDecomposition{
private:
    int N;
    std::vector<int> par;
    std::vector<int> dep;
    std::vector<int> siz;
    std::vector<int> vertex;
    std::vector<int> id;
    std::vector<int> head;

public:
    // G is tree that represented by AdjacencyList
    HeavyLightDecomposition(std::vector<std::vector<int>> G,int root=0){
        N=std::ssize(G);
        par.assign(N,-1);
        dep.assign(N,-1);
        siz.assign(N,0);
        vertex.assign(N,-1);
        id.assign(N,-1);
        head.assign(N,-1);
        {
            // calculate subtree size, put the heave child first in G.
            auto dfs=[&](auto &dfs, int v, int p=-1) -> void {
                par[v]=p;
                dep[v]=(p==-1 ? 0 : dep[p]+1);
                siz[v]=1;
                if(!G[v].empty() and G[v][0]==p) std::swap(G[v][0],G[v].back());
                for(int &w:G[v]){
                    if(w==p) continue;
                    dfs(dfs,w,v);
                    siz[v]+=siz[w];
                    if(siz[w]>siz[G[v][0]]) std::swap(G[v][0],w);
                }
            };
            dfs(dfs,root);
        }
        {
            int idx=0;
            auto dfs=[&](auto &dfs,int v,int p=-1) -> void {
                id[v]=idx;
                vertex[idx++]=v;
                for(int &w:G[v]){
                    if(w==p) continue;
                    bool heavy=(w==G[v][0]);
                    head[w]=(heavy?head[v]:w);
                    dfs(dfs,w,v);
                }
            };
            dfs(dfs,root);
        }
    }

    int numVertices() const { return N; }
    // 頂点vの深さ
    int depth(int v) const { return dep[v]; }
    // 頂点vがHLD配列上で何番目か
    int toSeq(int v) const { return id[v]; }
    // HLD配列[seqidx]の頂点番号
    int toVertex(int seqidx) const { return vertex[seqidx]; }
    // 頂点vの親 (根なら-1)
    int parent(int v) const { return par[v]; }
    // 頂点vのheavyRoot　(heavyPathに乗っていなければ自身の番号を返す)
    int heavyR(int v) const { return head[v]; }
    // 頂点vのheaveChild　存在しなければ-1
    int heavyC(int v) const {
        if(toSeq(v)==N-1) return -1;
        int cand=toVertex(toSeq(v)+1);
        if(same(v,cand)) return cand;
        return -1;
    }

    // 頂点uと頂点vが同一のheavyPathに乗っているか
    bool same(int u,int v) const { return head[u]==head[v]; }
    // 頂点vがheavyPath上で何番目の頂点か
    int distToHR(int v) const { return id[v]-id[head[v]]; }
    // 頂点vが含まれるheavyPathを配列で返す stopV=trueでvで止まる
    // O(len(heavyPath))
    std::vector<int> heavyPath(int v,bool stopV=false) const {
        std::vector<int> path;
        int idx=id[head[v]];
        while(idx!=-1){
            path.emplace_back(idx);
            if(stopV and idx==v) break; 
            idx=heavyC(idx);
        }
        return path;
    }
    // Level Ancestor　頂点vの祖先であって深さdにある頂点番号を返す
    // O(logN)
    int LA(int v,int d) const {
        assert(dep[v]>=d);
        while(dep[head[v]]>d){
            v=par[head[v]];
        }
        return vertex[id[v]-(dep[v]-d)];
    }
    // Lowest Common Ancestor　頂点u,vの最近共通祖先なる頂点番号を返す
    // O(logN)
    int LCA(int u,int v) const {
        while(head[u]!=head[v]){
            if(id[u]>id[v]) std::swap(u,v);
            v=par[head[v]];
        }
        return (id[u] < id[v] ? u:v);
    }
    // 任意の2頂点u,vの距離を返す　O(logN)
    int dist(int u,int v) const {
        const int w=LCA(u,v);
        return (dep[u]-dep[w])+(dep[v]-dep[w]);
    }
    // Jump on Tree　2頂点s,t及び非負整数iについて、
    // パスstにおけるi番目の頂点番号を返す　O(logN)
    int jump(int u,int v,int i) const {
        const int w=LCA(u,v);
        const int d=(dep[u]-dep[w])+(dep[v]-dep[w]);
        if(d<i) return -1;
        if(i < dep[u]-dep[w]) return LA(u,dep[u]-i);
        return LA(v,dep[v]-(d-i));
    }
};