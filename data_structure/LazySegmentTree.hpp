#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <cstddef>
#include <ostream>
#include <utility>
#include <vector>

#include "../math/LazyMonoid.hpp"

namespace sora {

// 作用付きモノイドによる点・区間更新、区間積、境界探索を行う遅延セグメント木。
// 0 <= N <= 10^8、区間は0-indexedの [l, r)。入力は変更しない。
// モノイドの各演算と値・作用素のコピー・代入をO(1)として、構築時間・空間O(N)。
// composition(f, g)はgの後にfを適用する。Fの等値比較・デフォルト構築は不要。
// 仕様の参考: https://atcoder.github.io/ac-library/master/document_ja/lazysegtree.html
template <LazyMonoid M>
class LazySegmentTree {
    using T = typename M::T;
    using F = typename M::F;
    int n_;
    int leaf_count_ = 1;
    int height_ = 0;
    std::vector<T> data_;
    std::vector<F> lazy_;

    static int checked_size(std::size_t n) {
        assert(n <= 100000000);
        return static_cast<int>(n);
    }

    void update(int node) {
        data_[node] = M::op(data_[2 * node], data_[2 * node + 1]);
    }

    void apply_node(int node, const F& f) {
        data_[node] = M::mapping(f, data_[node]);
        // data_には反映済みで、子へはまだ伝えていない作用を保持する。
        if (node < leaf_count_) lazy_[node] = M::composition(f, lazy_[node]);
    }

    void push(int node) {
        apply_node(2 * node, lazy_[node]);
        apply_node(2 * node + 1, lazy_[node]);
        lazy_[node] = M::id();
    }

    void push_path(int node) {
        for (int level = height_; level > 0; --level) push(node >> level);
    }

    void push_range(int left, int right) {
        for (int level = height_; level > 0; --level) {
            // 完全に含まれるノードは集約値をそのまま使える。境界の祖先だけを伝播する。
            if (((left >> level) << level) != left) push(left >> level);
            if (((right >> level) << level) != right) push((right - 1) >> level);
        }
    }

public:
    // 空列を構築する。時間・空間O(1)。
    LazySegmentTree() : LazySegmentTree(0) {}

    // 長さnの列を全てM::e()で初期化する。0 <= n <= 10^8。時間・空間O(n)。
    explicit LazySegmentTree(int n) : n_(n) {
        assert(0 <= n && n <= 100000000);
        leaf_count_ = static_cast<int>(std::bit_ceil(static_cast<unsigned int>(n)));
        height_ = static_cast<int>(std::bit_width(static_cast<unsigned int>(leaf_count_))) - 1;
        data_.assign(2 * leaf_count_, M::e());
        lazy_.assign(leaf_count_, M::id());
    }

    // 配列から構築する。入力は変更しない。時間・空間O(N)。
    explicit LazySegmentTree(const std::vector<T>& a) : LazySegmentTree(checked_size(a.size())) {
        std::copy(a.begin(), a.end(), data_.begin() + leaf_count_);
        for (int node = leaf_count_ - 1; node > 0; --node) update(node);
    }

    int size() const { return n_; }
    bool empty() const { return n_ == 0; }

    // a[p]をxに置き換える。0 <= p < N。時間O(log N)。
    void set(int p, const T& x) {
        assert(0 <= p && p < n_);
        const int node = leaf_count_ + p;
        push_path(node);
        data_[node] = x;
        for (int parent = node / 2; parent > 0; parent /= 2) update(parent);
    }

    // a[p]のコピーを返す。0 <= p < N。遅延作用を伝播する。時間O(log N)。
    T get(int p) {
        assert(0 <= p && p < n_);
        const int node = leaf_count_ + p;
        push_path(node);
        return data_[node];
    }

    // [l, r)の積を左から順に求める。0 <= l <= r <= N。
    // 空区間は単位元を返す。遅延作用を伝播する。時間O(log N)。
    T prod(int l, int r) {
        assert(0 <= l && l <= r && r <= n_);
        if (l == r) return M::e();
        l += leaf_count_;
        r += leaf_count_;
        push_range(l, r);
        T left = M::e(), right = M::e();
        while (l < r) {
            if (l & 1) left = M::op(left, data_[l++]);
            if (r & 1) right = M::op(data_[--r], right);
            l /= 2;
            r /= 2;
        }
        return M::op(left, right);
    }

    // 列全体の積。空列は単位元を返す。時間O(1)。
    T all_prod() const { return data_[1]; }

    // a[p]にfを適用する。0 <= p < N。時間O(log N)。
    void apply(int p, const F& f) {
        assert(0 <= p && p < n_);
        const int node = leaf_count_ + p;
        push_path(node);
        apply_node(node, f);
        for (int parent = node / 2; parent > 0; parent /= 2) update(parent);
    }

    // [l, r)の各要素にfを適用する。0 <= l <= r <= N。
    // 空区間では何もしない。時間O(log N)。
    void apply(int l, int r, const F& f) {
        assert(0 <= l && l <= r && r <= n_);
        if (l == r) return;
        const int left = l + leaf_count_, right = r + leaf_count_;
        push_range(left, right);
        for (l = left, r = right; l < r; l /= 2, r /= 2) {
            if (l & 1) apply_node(l++, f);
            if (r & 1) apply_node(--r, f);
        }
        for (int level = 1; level <= height_; ++level) {
            // 作用を直接載せたノードを子の値で上書きしないよう、境界の祖先だけを再計算する。
            if (((left >> level) << level) != left) update(left >> level);
            if (((right >> level) << level) != right) update((right - 1) >> level);
        }
    }

    // g(prod(l, r))がtrueとなる最大のrを返す。0 <= l <= N。
    // g(e()) == true、gは同じ引数に同じ値を返し、区間を右へ伸ばすとtrueからfalseへ単調に変わること。
    // gの評価をO(1)として時間O(log N)。l == NならNを返す。遅延作用を伝播する。
    template <class G>
    int find_last(int l, G g) {
        assert(0 <= l && l <= n_);
        assert(g(M::e()));
        if (l == n_) return n_;
        int node = leaf_count_ + l;
        push_path(node);
        T accumulated = M::e();
        do {
            while ((node & 1) == 0) node /= 2;
            T candidate = M::op(accumulated, data_[node]);
            if (!g(candidate)) {
                while (node < leaf_count_) {
                    push(node);
                    node *= 2;
                    candidate = M::op(accumulated, data_[node]);
                    if (g(candidate)) {
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
    template <bool (*g)(T)>
    int find_last(int l) { return find_last(l, g); }

    // g(prod(l, r))がtrueとなる最小のlを返す。0 <= r <= N。
    // g(e()) == true、gは同じ引数に同じ値を返し、区間を左へ伸ばすとtrueからfalseへ単調に変わること。
    // gの評価をO(1)として時間O(log N)。r == 0なら0を返す。遅延作用を伝播する。
    template <class G>
    int find_first(int r, G g) {
        assert(0 <= r && r <= n_);
        assert(g(M::e()));
        if (r == 0) return 0;
        int node = leaf_count_ + r;
        push_path(node - 1);
        T accumulated = M::e();
        do {
            --node;
            while (node > 1 && (node & 1)) node /= 2;
            T candidate = M::op(data_[node], accumulated);
            if (!g(candidate)) {
                while (node < leaf_count_) {
                    push(node);
                    node = 2 * node + 1;
                    candidate = M::op(data_[node], accumulated);
                    if (g(candidate)) {
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
    template <bool (*g)(T)>
    int find_first(int r) { return find_first(r, g); }

    // 遅延作用を全て葉へ伝え、元の葉列を [a[0], a[1], ...] と出力する。
    // 空列は []、改行は付けない。Tを出力できる場合のみ使用可能。
    // 各要素の出力をO(1)として時間O(N)。論理的な要素列は変更しない。
    friend std::ostream& operator<<(std::ostream& os, LazySegmentTree& tree)
        requires requires(std::ostream& out, const T& x) { out << x; }
    {
        for (int node = 1; node < tree.leaf_count_; ++node) tree.push(node);
        os << '[';
        for (int i = 0; i < tree.n_; ++i) {
            if (i != 0) os << ", ";
            os << tree.data_[tree.leaf_count_ + i];
        }
        return os << ']';
    }
};

} // namespace sora
