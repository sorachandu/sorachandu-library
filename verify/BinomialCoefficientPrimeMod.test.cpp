#define PROBLEM "https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod"

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include "../math/Combination.hpp"

// ACLに依存せず、検証に必要な演算だけを提供する動的modint。
struct TestModInt {
    static inline int modulus = 2;
    int value;
    explicit TestModInt(int x) : value((x % modulus + modulus) % modulus) {}
    // 時間O(1)。
    static int mod() { return modulus; }
    // 時間O(1)。
    friend TestModInt operator*(TestModInt a, TestModInt b) {
        return TestModInt(static_cast<int>(1LL * a.value * b.value % modulus));
    }
    // 時間O(1)。
    friend TestModInt operator-(TestModInt a, TestModInt b) {
        return TestModInt(a.value - b.value);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int t;
    std::cin >> t >> TestModInt::modulus;
    std::vector<std::pair<int, int>> queries(t);
    int limit = 0;
    for (auto& [n, k] : queries) {
        std::cin >> n >> k;
        limit = std::max(limit, n);
    }
    const sora::Combination<TestModInt> comb(limit);
    for (auto [n, k] : queries) std::cout << comb.comb(n, k).value << '\n';
}
