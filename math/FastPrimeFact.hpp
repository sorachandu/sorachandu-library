#pragma once

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>

namespace sora {

namespace detail {

// 二分累乗による剰余べき乗。
template <class T>
T pow_mod(T a, T n, T mod) {
    T res = 1 % mod;
    a %= mod;
    while (n) {
        if (n & 1) res = (res * a) % mod;
        a = (a * a) % mod;
        n >>= 1;
    }
    return res;
}

} // namespace detail

// signed 64-bit整数の素数判定。時間O(log n)、空間O(1)。n<2ではfalse。
inline bool is_prime(long long n) {
    if (n <= 1) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;
    constexpr long long bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    long long s = 0, d = n - 1;
    while (d % 2 == 0) {
        ++s;
        d >>= 1;
    }
    for (auto a : bases) {
        if (a % n == 0) continue;
        long long t, x = detail::pow_mod<__int128_t>(a, d, n);
        if (x != 1) {
            for (t = 0; t < s; ++t) {
                if (x == n - 1) break;
                x = __int128_t(x) * x % n;
            }
            if (t == s) return false;
        }
    }
    return true;
}

namespace detail {

// Pollardのロー法で使用する最大公約数。
inline long long prime_gcd(long long a, long long b) {
    a = std::abs(a), b = std::abs(b);
    if (b == 0)
        return a;
    else
        return prime_gcd(b, a % b);
}

inline long long pollard(long long n) {
    assert(n >= 2);
    if (n % 2 == 0) return 2;
    if (is_prime(n)) return n;

    auto f = [&](long long x) -> long long {
        return (__int128_t(x) * x + 1) % n;
    };
    long long step = 0;
    while (true) {
        step = (step == n - 1 ? 0 : step + 1);
        long long x = step, y = f(x);
        while (true) {
            long long p = prime_gcd(y - x, n);
            if (p == 0 || p == n) break;
            if (p != 1) return p;
            x = f(x);
            y = f(f(y));
        }
    }
}

} // namespace detail

// 正のsigned 64-bit整数を素因数の昇順列へ分解する。1に対しては空を返す。
// Pollardのロー法の探索時間に依存し、最悪時間は保証しない。
// 再帰・出力を含む空間O(log n)。
inline std::vector<long long> prime_factorize(long long n) {
    assert(n >= 1);
    if (n == 1) return {};
    long long p = detail::pollard(n);
    if (p == n) return {p};
    std::vector<long long> left = prime_factorize(p);
    std::vector<long long> right = prime_factorize(n / p);
    left.insert(left.end(), right.begin(), right.end());
    std::sort(left.begin(), left.end());
    return left;
}

} // namespace sora
