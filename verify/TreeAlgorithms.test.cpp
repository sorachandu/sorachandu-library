// 重軽分解と全方位木DPを、全ての根・頂点について単純解と比較するローカル検証。
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <random>
#include <ranges>
#include <utility>
#include <vector>
#include "../graph/HeavyLightDecomposition.hpp"
#include "../graph/RerootingDP.hpp"
// 頂点値と辺寄与を異なる型にして、コールバックの型契約も確認する。
struct VertexValue { long long count = 0, sum = 0; };
struct EdgeValue { long long count = 0, sum = 0; };
int main() {
    {
        std::vector<std::vector<int>> g{{1,2},{0},{0,3},{2,4},{3}};
        sora::HeavyLightDecomposition h(g);
        assert((h.heavy_path(0) == std::vector<int>{0,2,3,4}));
        assert((h.heavy_path(1) == std::vector<int>{1}));
        assert((h.heavy_path(3,true) == std::vector<int>{0,2,3}));
    }
    std::mt19937 rng(98125);
    auto op = [](EdgeValue a, EdgeValue b) { return EdgeValue{a.count+b.count, a.sum+b.sum}; };
    auto put_vertex = [](EdgeValue a, int v) { return VertexValue{a.count+1,a.sum+v+1}; };
    auto put_edge = [](VertexValue a, int eid) {
        const int weight = eid >= 0 ? eid % 7 + 1 : (~eid) % 7 + 10;
        return EdgeValue{a.count, a.sum + a.count * weight};
    };
    for (int n = 1; n <= 30; ++n) {
        std::vector<std::vector<std::pair<int,int>>> g(n);
        for (int v = 1; v < n; ++v) {
            int p = rng() % v, eid = v-1;
            g[p].emplace_back(v,eid); g[v].emplace_back(p,~eid);
        }
        for (auto& neighbors : g) std::shuffle(neighbors.begin(), neighbors.end(), rng);
        for (int base : {0,n-1}) {
            const auto result = sora::rerootingdp<VertexValue,EdgeValue>(op, EdgeValue{}, put_edge, put_vertex, g, base);
            for (int root = 0; root < n; ++root) {
                std::vector<VertexValue> expected(n);
                auto dfs = [&](auto&& self, int v, int parent) -> VertexValue {
                    EdgeValue aggregate{};
                    for (auto [w,eid] : g[v]) if (w != parent) aggregate=op(aggregate,put_edge(self(self,w,v),eid));
                    return expected[v]=put_vertex(aggregate,v);
                };
                dfs(dfs,root,-1);
                for (int v=0;v<n;++v) {
                    auto value=result(root,v);
                    assert(value.count==expected[v].count && value.sum==expected[v].sum);
                }
                auto value=result(root);
                assert(value.count==expected[root].count && value.sum==expected[root].sum);
            }
        }
    }

    for (int n=1;n<=30;++n) {
        std::vector<std::vector<int>> g(n);
        for (int v=1;v<n;++v) { int p=rng()%v; g[p].push_back(v); g[v].push_back(p); }
        for (int root : {0,n-1}) {
            sora::HeavyLightDecomposition h(g,root);
            std::vector<int> parent(n,-1),depth(n);
            auto dfs=[&](auto&& self,int v,int p)->void {
                parent[v]=p;
                for (int w:g[v]) if(w!=p) {depth[w]=depth[v]+1;self(self,w,v);}
            };
            dfs(dfs,root,-1);
            for (int v=0;v<n;++v) {
                const auto path=h.heavy_path(v);
                assert(!path.empty() && path.front()==h.heavy_root(v));
                int count=0;
                for (int w=0;w<n;++w) if (h.same(v,w)) ++count;
                assert(static_cast<int>(path.size())==count);
                for (std::size_t i=0;i<path.size();++i) {
                    assert(h.same(v,path[i]));
                    if (i) assert(parent[path[i]]==path[i-1]);
                }
                const auto position=std::find(path.begin(),path.end(),v);
                assert(position!=path.end());
                assert((h.heavy_path(v,true)==std::vector<int>(path.begin(),position+1)));
            }
            for(int u=0;u<n;++u) for(int v=0;v<n;++v) {
                int a=u,b=v;
                while(depth[a]>depth[b])a=parent[a];
                while(depth[b]>depth[a])b=parent[b];
                while(a!=b){a=parent[a];b=parent[b];}
                assert(h.lca(u,v)==a);
                std::vector<int> path,tail;
                for(int x=u;x!=a;x=parent[x])path.push_back(x);
                path.push_back(a);
                for(int x=v;x!=a;x=parent[x])tail.push_back(x);
                path.insert(path.end(),tail.rbegin(),tail.rend());
                assert(h.dist(u,v)==static_cast<int>(path.size())-1);
                for(int i=0;i<static_cast<int>(path.size());++i)assert(h.jump(u,v,i)==path[i]);
                assert(h.jump(u,v,path.size())==-1);
            }
        }
    }


}
