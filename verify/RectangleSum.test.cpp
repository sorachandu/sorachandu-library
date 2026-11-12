#define PROBLEM "https://judge.yosupo.jp/problem/rectangle_sum"

#include <iostream>
#include <utility>
#include <vector>

#include "../data_structure/RectangleSum.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, q;
    std::cin >> n >> q;
    std::vector<sora::RectangleSum<>::Point> points(n);
    for (auto& p : points) std::cin >> p.x >> p.y >> p.weight;
    const sora::RectangleSum<> rs(std::move(points));
    while (q--) {
        long long l, d, r, u;
        std::cin >> l >> d >> r >> u;
        std::cout << rs.rectangle_sum(l, d, r, u) << '\n';
    }
}
