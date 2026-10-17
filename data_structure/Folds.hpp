#pragma once

#include <cassert>
#include <vector>

#include "../math/Monoid.hpp"

namespace sora {

// モノイドの左右累積積を保持する。可換性・逆元は不要。入力は変更しない。
// M::op、M::e、値のコピーをO(1)として、構築時間・空間O(N)、各クエリO(1)。
template <Monoid M>
class Folds {
    using T = typename M::T;
    int n;
    std::vector<T> prefix, suffix;

public:
    // 空配列に対する構造を作る。時間・空間O(1)。
    Folds() : Folds(std::vector<T>{}) {}

    // 時間・空間O(N)。
    explicit Folds(const std::vector<T>& a)
        : n(static_cast<int>(a.size())), prefix(n + 1, M::e()), suffix(n + 1, M::e()) {
        for (int i = 0; i < n; ++i) prefix[i + 1] = M::op(prefix[i], a[i]);
        for (int i = n - 1; i >= 0; --i) suffix[i] = M::op(a[i], suffix[i + 1]);
    }

    // [0, r)の積。時間O(1)。
    T fold_left(int r) const {
        assert(0 <= r && r <= n);
        return prefix[r];
    }

    // [l, N)の積。時間O(1)。
    T fold_right(int l) const {
        assert(0 <= l && l <= n);
        return suffix[l];
    }

    // 0-indexedの半開区間[l, r)を除いた積。時間O(1)。空区間なら全体の積。
    T fold_without_range(int l, int r) const {
        assert(0 <= l && l <= r && r <= n);
        return M::op(prefix[l], suffix[r]);
    }

    // 0-indexedの要素iを除いた積。時間O(1)。
    T fold_without1(int i) const {
        assert(0 <= i && i < n);
        return fold_without_range(i, i + 1);
    }

    // 1-indexedの閉区間[l, r]を除いた積。旧実装と同じ仕様。時間O(1)。
    T fold_without_range_one_indexed(int l, int r) const {
        assert(1 <= l && l <= r && r <= n);
        return fold_without_range(l - 1, r);
    }

    // 1-indexedの要素iを除いた積。旧実装と同じ仕様。時間O(1)。
    T fold_without1_one_indexed(int i) const {
        assert(1 <= i && i <= n);
        return fold_without1(i - 1);
    }

    // 要素数。時間O(1)。
    int size() const { return n; }
};

} // namespace sora
