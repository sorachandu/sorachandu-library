#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"

#include <cstdint>
#include <iostream>
#include <vector>

#include "../data_structure/LazySegmentTree.hpp"

// 区間の各要素に一次関数を適用し、区間和を求める作用付きモノイド。
struct RangeAffineSum {
    static constexpr std::int64_t mod = 998244353;
    // 区間和と要素数。
    struct T {
        std::int64_t sum;
        int length;
    };
    // 一次関数 ax+b の係数。
    struct F {
        std::int64_t a, b;
    };
    static T op(T x, T y) { return {(x.sum + y.sum) % mod, x.length + y.length}; }
    static T e() { return {0, 0}; }
    static T mapping(F f, T x) { return {(f.a * x.sum + f.b * x.length) % mod, x.length}; }
    static F composition(F f, F g) { return {f.a * g.a % mod, (f.a * g.b + f.b) % mod}; }
    static F id() { return {1, 0}; }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, q;
    std::cin >> n >> q;
    std::vector<RangeAffineSum::T> values(n);
    for (auto& x : values) {
        std::cin >> x.sum;
        x.length = 1;
    }
    sora::LazySegmentTree<RangeAffineSum> seg(values);
    while (q--) {
        int type, l, r;
        std::cin >> type >> l >> r;
        if (type == 0) {
            RangeAffineSum::F f;
            std::cin >> f.a >> f.b;
            seg.apply(l, r, f);
        } else {
            std::cout << seg.prod(l, r).sum << '\n';
        }
    }
}
