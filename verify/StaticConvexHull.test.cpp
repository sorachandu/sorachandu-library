#define PROBLEM "https://judge.yosupo.jp/problem/static_convex_hull"
#include "../geometry/Geometry2D.hpp"
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int t;
    std::cin >> t;
    while (t--) {
        int n;
        std::cin >> n;
        std::vector<sora::Point<long long>> points(n);
        for (auto& p : points) std::cin >> p.x >> p.y;
        auto hull = sora::convex_hull(std::move(points));
        std::cout << hull.size() << '\n';
        for (auto p : hull) std::cout << p.x << ' ' << p.y << '\n';
    }
}
