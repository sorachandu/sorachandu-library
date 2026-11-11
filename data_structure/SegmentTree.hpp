#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <cstddef>
#include <ostream>
#include <utility>
#include <vector>

#include "../math/Monoid.hpp"

namespace sora {

// モノイドの点更新・区間積と区間の境界探索。可換性・逆元は不要。
// 0 <= N <= 10^8、区間は0-indexedの [l, r)。入力は変更しない。
// M::op、M::e、Tのコピー・代入をO(1)として、構築時間・空間O(N)。
// 仕様の参考: https://atcoder.github.io/ac-library/master/document_ja/segtree.html
template <Monoid M>
class SegmentTree {
    using T = typename M::T;
    int n_;
    int leaf_count_ = 1;
    std::vector<T> data_;

    static int checked_size(std::size_t n) {
        assert(n <= 100000000);
        return static_cast<int>(n);
    }

    void update(int node) {
        data_[node] = M::op(data_[2 * node], data_[2 * node + 1]);
    }

public:
    // 空列を構築する。時間・空間O(1)。
    SegmentTree() : SegmentTree(0) {}

    // 長さnの列を単位元で初期化する。0 <= n <= 10^8。時間・空間O(n)。
    explicit SegmentTree(int n) : n_(n) {
        assert(0 <= n && n <= 100000000);
        leaf_count_ = static_cast<int>(std::bit_ceil(static_cast<unsigned int>(n)));
        data_.assign(2 * leaf_count_, M::e());
    }

    // 配列から構築する。入力は変更しない。時間・空間O(N)。
    explicit SegmentTree(const std::vector<T>& a) : SegmentTree(checked_size(a.size())) {
        std::copy(a.begin(), a.end(), data_.begin() + leaf_count_);
        for (int node = leaf_count_ - 1; node > 0; --node) update(node);
    }

    int size() const { return n_; }
    bool empty() const { return n_ == 0; }

    // a[p]をxに置き換える。0 <= p < N。時間O(log N)。
    void set(int p, const T& x) {
        assert(0 <= p && p < n_);
        int node = leaf_count_ + p;
        data_[node] = x;
        while (node > 1) {
            node /= 2;
            update(node);
        }
    }

    // a[p]を返す。0 <= p < N。時間O(1)。
    T get(int p) const {
        assert(0 <= p && p < n_);
        return data_[leaf_count_ + p];
    }

    // [l, r)の積を左から順に求める。0 <= l <= r <= N。
    // 空区間は単位元を返す。時間O(log N)。
    T prod(int l, int r) const {
        assert(0 <= l && l <= r && r <= n_);
        T left = M::e(), right = M::e();
        for (l += leaf_count_, r += leaf_count_; l < r; l /= 2, r /= 2) {
            if (l & 1) left = M::op(left, data_[l++]);
            if (r & 1) right = M::op(data_[--r], right);
        }
        return M::op(left, right);
    }

    // 列全体の積。空列は単位元を返す。時間O(1)。
    T all_prod() const { return data_[1]; }

    // f(prod(l, r))がtrueとなる最大のrを返す。0 <= l <= N。
    // f(e()) == true、fは同じ引数に同じ値を返し、区間を右へ伸ばすとtrueからfalseへ単調に変わること。
    // fの評価をO(1)として時間O(log N)。l == NならNを返す。
    template <class F>
    int find_last(int l, F f) const {
        assert(0 <= l && l <= n_);
        assert(f(M::e()));
        if (l == n_) return n_;
        int node = leaf_count_ + l;
        T accumulated = M::e();
        do {
            while ((node & 1) == 0) node /= 2;
            T candidate = M::op(accumulated, data_[node]);
            if (!f(candidate)) {
                while (node < leaf_count_) {
                    node *= 2;
                    candidate = M::op(accumulated, data_[node]);
                    if (f(candidate)) {
                        accumulated = std::move(candidate);
                        ++node;
                    }
                }
                return node - leaf_count_;
            }
            accumulated = std::move(candidate);
            ++node;
        } while (!std::has_single_bit(static_cast<unsigned int>(node)));
        return n_;
    }

    // 関数ポインタをテンプレート引数で指定するfind_last。条件・計算量は上と同じ。
    template <bool (*f)(T)>
    int find_last(int l) const { return find_last(l, f); }

    // f(prod(l, r))がtrueとなる最小のlを返す。0 <= r <= N。
    // f(e()) == true、fは同じ引数に同じ値を返し、区間を左へ伸ばすとtrueからfalseへ単調に変わること。
    // fの評価をO(1)として時間O(log N)。r == 0なら0を返す。
    template <class F>
    int find_first(int r, F f) const {
        assert(0 <= r && r <= n_);
        assert(f(M::e()));
        if (r == 0) return 0;
        int node = leaf_count_ + r;
        T accumulated = M::e();
        do {
            --node;
            while (node > 1 && (node & 1)) node /= 2;
            T candidate = M::op(data_[node], accumulated);
            if (!f(candidate)) {
                while (node < leaf_count_) {
                    node = 2 * node + 1;
                    candidate = M::op(data_[node], accumulated);
                    if (f(candidate)) {
                        accumulated = std::move(candidate);
                        --node;
                    }
                }
                return node + 1 - leaf_count_;
            }
            accumulated = std::move(candidate);
        } while (!std::has_single_bit(static_cast<unsigned int>(node)));
        return 0;
    }

    // 関数ポインタをテンプレート引数で指定するfind_first。条件・計算量は上と同じ。
    template <bool (*f)(T)>
    int find_first(int r) const { return find_first(r, f); }

    // 元の葉列を [a[0], a[1], ...] の形式で出力する。空列は []、改行は付けない。
    // Tを出力できる場合のみ使用可能。各要素の出力をO(1)として時間O(N)。
    friend std::ostream& operator<<(std::ostream& os, const SegmentTree& tree)
        requires requires(std::ostream& out, const T& x) { out << x; }
    {
        os << '[';
        for (int i = 0; i < tree.n_; ++i) {
            if (i != 0) os << ", ";
            os << tree.data_[tree.leaf_count_ + i];
        }
        return os << ']';
    }
};

} // namespace sora
