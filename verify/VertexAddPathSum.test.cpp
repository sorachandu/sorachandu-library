#define PROBLEM "https://judge.yosupo.jp/problem/vertex_add_path_sum"

#include <iostream>
#include <utility>
#include <vector>
#include "../data_structure/FenwickTree.hpp"
#include "../graph/HeavyLightDecomposition.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, q;
    std::cin >> n >> q;
    std::vector<long long> values(n);
    for (auto& a : values) std::cin >> a;
    std::vector<std::vector<int>> graph(n);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        std::cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    const sora::HeavyLightDecomposition hld(std::move(graph));
    sora::FenwickTree<long long> bit(n);
    for (int v = 0; v < n; ++v) bit.add(hld.to_seq(v), values[v]);
    // 点加算O(log N)、パス和O(log^2 N)、前処理O(N log N)、空間O(N)。
    while (q--) {
        int type;
        std::cin >> type;
        if (type == 0) {
            int p;
            long long x;
            std::cin >> p >> x;
            bit.add(hld.to_seq(p), x);
        } else {
            int u, v;
            std::cin >> u >> v;
            long long answer = 0;
            while (!hld.same(u, v)) {
                if (hld.depth(hld.heavy_root(u)) < hld.depth(hld.heavy_root(v)))
                    std::swap(u, v);
                const int head = hld.heavy_root(u);
                answer += bit.sum(hld.to_seq(head), hld.to_seq(u) + 1);
                u = hld.parent(head);
            }
            if (hld.to_seq(u) > hld.to_seq(v)) std::swap(u, v);
            answer += bit.sum(hld.to_seq(u), hld.to_seq(v) + 1);
            std::cout << answer << '\n';
        }
    }
}
