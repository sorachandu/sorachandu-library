// verification-helper: IGNORE
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <numeric>
#include <random>
#include <vector>

#include "../data_structure/DSU.hpp"
#include "../data_structure/DSUChoose.hpp"
#include "../data_structure/DisjointSparseTable.hpp"
#include "../data_structure/Folds.hpp"

// 非可換な行列積で左右累積と除外時の順序を検証する。
struct MatrixProduct {
    using T = std::array<std::uint64_t, 4>;
    static T op(const T& a, const T& b) {
        return {(a[0] * b[0] + a[1] * b[2]) % 101,
                (a[0] * b[1] + a[1] * b[3]) % 101,
                (a[2] * b[0] + a[3] * b[2]) % 101,
                (a[2] * b[1] + a[3] * b[3]) % 101};
    }
    static T e() { return {1, 0, 0, 1}; }
};

// DSUの単純解比較、およびFolds/DSTで共通モノイドを使用するローカル検証。
int main() {
    std::mt19937 rng(54321);
    {
        sora::DSU empty;
        sora::DSUChoose empty_choose;
        constexpr int n = 100;
        sora::DSU uf(n);
        sora::DSUChoose choose(n);
        std::vector<int> component(n), representative(n);
        std::iota(component.begin(), component.end(), 0);
        std::iota(representative.begin(), representative.end(), 0);
        for (int t = 0; t < 500; ++t) {
            int u = rng() % n, v = rng() % n;
            const int cu = component[u], cv = component[v];
            const int old_root = uf.root(u);
            uf.merge(u, v);
            assert(uf.root(v) == old_root);
            int calls = 0;
            choose.merge(u, v, [&](int a, int b) { ++calls; return std::max(a, b); });
            assert(calls == (cu != cv));
            const int new_representative = std::max(representative[u], representative[v]);
            for (int i = 0; i < n; ++i) if (component[i] == cu || component[i] == cv) {
                component[i] = cu;
                representative[i] = new_representative;
            }
            for (int i = 0; i < n; ++i) {
                assert(choose.rpr_root(i) == representative[i]);
                for (int j = 0; j < n; ++j) {
                    assert(uf.same(i, j) == (component[i] == component[j]));
                    assert(choose.same(i, j) == uf.same(i, j));
                    assert((choose.par_root(i) == choose.par_root(j)) == choose.same(i, j));
                }
            }
        }
        sora::DSU path(200000);
        for (int v = 1; v < 200000; ++v) path.merge(v, v - 1);
        assert(path.root(0) == 199999);
    }
    {
        sora::Folds<MatrixProduct> empty;
        assert(empty.size() == 0 && empty.fold_left(0) == MatrixProduct::e());
        assert(empty.fold_right(0) == MatrixProduct::e());
        assert(empty.fold_without_range(0, 0) == MatrixProduct::e());
        for (int n = 0; n <= 40; ++n) {
            std::vector<MatrixProduct::T> values(n);
            for (auto& a : values) for (auto& x : a) x = rng() % 101;
            const sora::Folds<MatrixProduct> folds(values);
            const sora::DisjointSparseTable<MatrixProduct> dst(values);
            auto prod = [&](int l, int r) {
                auto result = MatrixProduct::e();
                for (int i = l; i < r; ++i) result = MatrixProduct::op(result, values[i]);
                return result;
            };
            for (int l = 0; l <= n; ++l) {
                assert(folds.fold_left(l) == prod(0, l));
                assert(folds.fold_right(l) == prod(l, n));
                for (int r = l; r <= n; ++r) {
                    auto result = MatrixProduct::op(prod(0, l), prod(r, n));
                    assert(folds.fold_without_range(l, r) == result);
                    assert(dst.prod(l, r) == prod(l, r));
                    if (l < r) assert(folds.fold_without_range_one_indexed(l + 1, r) == result);
                }
                if (l < n) {
                    assert(folds.fold_without1(l) == folds.fold_without_range(l, l + 1));
                    assert(folds.fold_without1_one_indexed(l + 1) == folds.fold_without1(l));
                }
            }
        }
    }
}
