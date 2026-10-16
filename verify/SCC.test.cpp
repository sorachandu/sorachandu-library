#define PROBLEM "https://judge.yosupo.jp/problem/scc"

#include <iostream>
#include "../graph/SCC.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m;
    std::cin >> n >> m;
    sora::SCCGraph graph(n);
    while (m--) {
        int u, v;
        std::cin >> u >> v;
        graph.add_edge(u, v);
    }
    const auto result = graph.build();
    std::cout << result.count() << '\n';
    for (const auto& group : result.groups) {
        std::cout << group.size();
        for (int v : group) std::cout << ' ' << v;
        std::cout << '\n';
    }
}
