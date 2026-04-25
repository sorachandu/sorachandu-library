/*//--------------------------------------------------------
DSU 経路圧縮+Union by Rank O(α(N))
merge時に代表元を自由に決められる (根付き木において深さが小さい方、とか)
例: dsu.merge(u,v,[&](int a,int b){ return depth[a]<depth[b]?a:b; });
論理的な代表元と実際の木の根は異なることに注意
意味を持たせた代表元という点ではpar_rootではなくrpr_rootを使うことに注意
u,vでu側に問答無用で併合したい場合、[](int a,int b){ return a; } でよい
*///--------------------------------------------------------
class DSU {
    vector<int> par, rnk, rpr; // rpr: 論理的な代表元

public:
    DSU() : par(),rnk(),rpr() {}
    DSU(int n) : par(n, -1), rnk(n, 0), rpr(n) {
        iota(rpr.begin(), rpr.end(), 0);
    }

    // Union-Find木における根を返す
    int par_root(int v) {
        return (~par[v] ? par[v] = par_root(par[v]) : v);
    }

    // 論理的な代表元を返す
    int rpr_root(int v) { return rpr[par_root(v)]; }

    // 代表元の選び方をラムダで注入
    // choose(rpr_u, rpr_v) -> 新しい代表元
    void merge(int u, int v, auto choose) {
        int ru = par_root(u), rv = par_root(v);
        if (ru == rv) return;

        int new_rpr = choose(rpr[ru], rpr[rv]); // 好きな規則で選ぶ

        // Union by Rank で構造を決める（代表元とは独立）
        if (rnk[ru] < rnk[rv]) swap(ru, rv);
        par[rv] = ru;
        if (rnk[ru] == rnk[rv]) rnk[ru]++;

        rpr[ru] = new_rpr; // 構造上の根に代表元を記録
    }

    bool same(int u, int v) { return par_root(u) == par_root(v); }
};