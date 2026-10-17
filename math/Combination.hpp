#pragma once

#include <cassert>
#include <concepts>
#include <vector>

namespace sora {

// 整数からの構築、mod()、積・差を持つmodint型。数学的な性質は利用者が保証する。
template <typename T>
concept CombinationValue = std::copyable<T> && requires(T a, T b) {
    T{0};
    T{1};
    { T::mod() } -> std::convertible_to<int>;
    { a * b } -> std::convertible_to<T>;
    { a - b } -> std::convertible_to<T>;
};

// 素数modで階乗・二項係数・順列数を前計算する。
// 前提: 0 <= N < ModInt::mod()、modは素数でintに収まり、利用中に変化しない。
// modintの演算をO(1)として、構築時間・空間O(N)、各クエリO(1)。
template <CombinationValue ModInt>
class Combination {
    int modulus;
    std::vector<ModInt> factorial, inverse_factorial;

public:
    // Nまでの値を前計算する。時間・空間O(N)。N=0にも対応する。
    explicit Combination(int n) : modulus(ModInt::mod()) {
        assert(modulus >= 2 && 0 <= n && n < modulus);
        factorial.assign(n + 1, ModInt{1});
        inverse_factorial.assign(n + 1, ModInt{1});
        std::vector<ModInt> inverse(n + 1, ModInt{0});
        if (n >= 1) inverse[1] = ModInt{1};
        for (int i = 2; i <= n; ++i) {
            factorial[i] = factorial[i - 1] * ModInt{i};
            inverse[i] = ModInt{0} - inverse[modulus % i] * ModInt{modulus / i};
            inverse_factorial[i] = inverse_factorial[i - 1] * inverse[i];
        }
    }

    // nCk。時間O(1)。n<0、k<0、k>nでは0。その他はnが前計算範囲内であること。
    ModInt comb(int n, int k) const {
        assert(ModInt::mod() == modulus);
        if (n < 0 || k < 0 || n < k) return ModInt{0};
        assert(n < static_cast<int>(factorial.size()));
        return factorial[n] * inverse_factorial[k] * inverse_factorial[n - k];
    }

    // nPk。時間O(1)。n<0、k<0、k>nでは0。その他はnが前計算範囲内であること。
    ModInt perm(int n, int k) const {
        assert(ModInt::mod() == modulus);
        if (n < 0 || k < 0 || n < k) return ModInt{0};
        assert(n < static_cast<int>(factorial.size()));
        return factorial[n] * inverse_factorial[n - k];
    }

    // n!。時間O(1)。n<0では0。その他はnが前計算範囲内であること。
    ModInt fact(int n) const {
        assert(ModInt::mod() == modulus);
        if (n < 0) return ModInt{0};
        assert(n < static_cast<int>(factorial.size()));
        return factorial[n];
    }
};

} // namespace sora
