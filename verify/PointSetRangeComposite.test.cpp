#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"

#include <cstdint>
#include <iostream>
#include <vector>

#include "../data_structure/SegmentTree.hpp"

// 一次関数を左から順に適用する非可換モノイド。
struct AffineComposition {
    // 一次関数 ax+b の係数。
    struct T {
        std::int64_t a, b;
    };
    static constexpr std::int64_t mod = 998244353;
    static T op(const T& f, const T& g) {
        return {g.a * f.a % mod, (g.a * f.b + g.b) % mod};
    }
    static T e() { return {1, 0}; }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, q;
    std::cin >> n >> q;
    std::vector<AffineComposition::T> a(n);
    for (auto& f : a) std::cin >> f.a >> f.b;
    sora::SegmentTree<AffineComposition> seg(a);
    while (q--) {
        int type;
        std::cin >> type;
        if (type == 0) {
            int p;
            AffineComposition::T f;
            std::cin >> p >> f.a >> f.b;
            seg.set(p, f);
        } else {
            int l, r;
            std::int64_t x;
            std::cin >> l >> r >> x;
            const auto f = seg.prod(l, r);
            std::cout << (f.a * x + f.b) % AffineComposition::mod << '\n';
        }
    }
}
