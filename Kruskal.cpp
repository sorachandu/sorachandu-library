#include <bits/stdc++.h>
using namespace std;
#include <atcoder/dsu>
using namespace atcoder;
typedef long long ll;
#define all(x) (x).begin(),(x).end()

// 最小全域木をクラスカル法で求める from: algo-logic.info/kruskal-mst/
struct Edge{
    ll fr;
    ll to;
    ll cost;
};
bool comp_e(const Edge& ef, const Edge& es) {return ef.cost < es.cost; }

struct Kruskal{
    dsu uf;
    ll sum;
    vector<Edge> edges;
    int V;
    Kruskal(const vector<Edge>& edges_, int V_) : edges(edges_), V(V_) { fix(); }
    void fix(){
        sort(all(edges), comp_e);
        uf = dsu(V);
        sum = 0;
        for(auto e : edges){
            if(!uf.same(e.fr, e.to)){
                uf.merge(e.fr, e.to);
                if(e.cost > 0) sum += e.cost;
            }
        }
    }
};