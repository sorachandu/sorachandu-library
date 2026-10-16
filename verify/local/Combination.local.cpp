#include <cassert>
#include <vector>

#include "../../math/Combination.hpp"

// ACLを使わず、必要最小限の演算だけを持つmodintで汎用版を検証する。
template <int Mod>
struct TestModInt {
    int value;
    explicit TestModInt(int x) : value((x % Mod + Mod) % Mod) {}
    static constexpr int mod() { return Mod; }
    friend TestModInt operator*(TestModInt a, TestModInt b) { return TestModInt(a.value * b.value); }
    friend TestModInt operator-(TestModInt a, TestModInt b) { return TestModInt(a.value - b.value); }
};

// Pascalの三角形・積による単純解と比較するローカル検証。
int main() {
    static_assert(sora::CombinationValue<TestModInt<101>>);
    static_assert(!sora::CombinationValue<int>);
    using Mint = TestModInt<101>;
    std::vector<std::vector<int>> pascal(101, std::vector<int>(101));
    pascal[0][0] = 1;
    for (int n = 1; n <= 100; ++n) {
        pascal[n][0] = 1;
        for (int k = 1; k <= n; ++k) pascal[n][k] = (pascal[n - 1][k - 1] + pascal[n - 1][k]) % 101;
    }
    for (int limit : {0, 1, 2, 100}) {
        const sora::Combination<Mint> comb(limit);
        int factorial = 1;
        for (int n = 0; n <= limit; ++n) {
            if (n > 0) factorial = factorial * n % 101;
            assert(comb.fact(n).value == factorial);
            int permutation = 1;
            for (int k = 0; k <= n; ++k) {
                if (k > 0) permutation = permutation * (n - k + 1) % 101;
                assert(comb.comb(n, k).value == pascal[n][k]);
                assert(comb.perm(n, k).value == permutation);
            }
            assert(comb.comb(n, n + 1).value == 0 && comb.perm(n, n + 1).value == 0);
            assert(comb.comb(n, -1).value == 0 && comb.perm(n, -1).value == 0);
        }
        assert(comb.comb(-1, 0).value == 0 && comb.perm(-1, 0).value == 0 && comb.fact(-1).value == 0);
    }
    const sora::Combination<TestModInt<2>> binary(1);
    assert(binary.comb(1, 0).value == 1 && binary.comb(1, 1).value == 1);
}
