#define PROBLEM "https://judge.yosupo.jp/problem/point_add_rectangle_sum"

#include <iostream>
#include "../data_structure/DynamicPointAddRectangleSum.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, q;
    std::cin >> n >> q;
    sora::DynamicPointAddRectangleSum<> points;
    auto add = [&] {
        long long x, y, w;
        std::cin >> x >> y >> w;
        points.add(x, y, w);
    };
    while (n--) add();
    while (q--) {
        int type;
        std::cin >> type;
        if (type == 0) add();
        else {
            long long l, d, r, u;
            std::cin >> l >> d >> r >> u;
            std::cout << points.rectangle_sum(l, d, r, u) << '\n';
        }
    }
}
