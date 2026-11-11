#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <type_traits>
#include <vector>

namespace sora {

// 整数列の静的な区間順位・頻度クエリ。入力は変更しない。
// N <= INT_MAX、区間は [l, r)。σ は値の種類数、H = ceil(log2(max(1, σ)))。
// 構築 O((N + 1) log(N + 2)) 時間、保持領域 O(σ + (N / 64 + 1) H) ワード、作業領域 O(N)。
// クエリは O(log(σ + 2)) 時間。bool は対象外。更新には非対応。
template <std::integral T>
    requires (!std::same_as<std::remove_cv_t<T>, bool> && std::same_as<T, std::remove_cv_t<T>>)
class WaveletMatrix {
    // 64bitブロックごとの累積個数を持つビット列。
    struct BitVector {
        std::vector<std::uint64_t> bits;
        std::vector<int> prefix;

        explicit BitVector(int n) : bits((static_cast<std::size_t>(n) + 63) / 64) {}

        void set(int p) { bits[p / 64] |= std::uint64_t{1} << (p % 64); }

        void build() {
            prefix.resize(bits.size() + 1);
            for (std::size_t i = 0; i < bits.size(); ++i)
                prefix[i + 1] = prefix[i] + std::popcount(bits[i]);
        }

        int rank_one(int r) const {
            const int offset = r % 64;
            // r が64の倍数なら、末尾ブロックへのアクセスもシフトも不要。
            return prefix[r / 64] + (offset == 0 ? 0 :
                std::popcount(bits[r / 64] & ((std::uint64_t{1} << offset) - 1)));
        }

        bool get(int p) const { return (bits[p / 64] >> (p % 64)) & 1; }
    };

    int n_ = 0;
    int height_ = 0;
    std::vector<T> values_;
    std::vector<BitVector> levels_;
    std::vector<int> zeros_;

    void check_range(int l, int r) const { assert(0 <= l && l <= r && r <= n_); }

    int lower_index(T x) const {
        return static_cast<int>(std::lower_bound(values_.begin(), values_.end(), x) - values_.begin());
    }

    int count_less(int l, int r, int upper) const {
        if (upper == 0) return 0;
        if (upper == static_cast<int>(values_.size())) return r - l;
        int result = 0;
        for (int level = 0; level < height_; ++level) {
            const int left = levels_[level].rank_one(l);
            const int right = levels_[level].rank_one(r);
            if ((upper >> (height_ - 1 - level)) & 1) {
                result += (r - l) - (right - left);
                l = zeros_[level] + left;
                r = zeros_[level] + right;
            } else {
                l -= left;
                r -= right;
            }
        }
        return result;
    }

public:
    // 空列を構築する。時間 O(1)。
    WaveletMatrix() = default;

    // 配列から構築する。時間 O((N + 1) log(N + 2))、入力は変更しない。
    explicit WaveletMatrix(const std::vector<T>& a) : values_(a) {
        assert(a.size() <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
        n_ = static_cast<int>(a.size());
        std::sort(values_.begin(), values_.end());
        values_.erase(std::unique(values_.begin(), values_.end()), values_.end());
        // 圧縮辞書の容量も種類数に合わせ、重複の多い列での保持領域を抑える。
        std::vector<T>(values_).swap(values_);
        if (empty()) return;
        height_ = std::bit_width(static_cast<unsigned int>(values_.size() - 1));
        std::vector<int> current(n_), next(n_);
        for (int i = 0; i < n_; ++i) current[i] = lower_index(a[i]);
        levels_.reserve(height_);
        zeros_.reserve(height_);
        for (int bit = height_ - 1; bit >= 0; --bit) {
            levels_.emplace_back(n_);
            auto& bv = levels_.back();
            int zero_count = 0;
            for (int i = 0; i < n_; ++i) {
                if ((current[i] >> bit) & 1) bv.set(i);
                else ++zero_count;
            }
            bv.build();
            zeros_.push_back(zero_count);
            int zero_pos = 0, one_pos = zero_count;
            for (int x : current) {
                if ((x >> bit) & 1) next[one_pos++] = x;
                else next[zero_pos++] = x;
            }
            current.swap(next);
        }
    }

    int size() const { return n_; }
    bool empty() const { return n_ == 0; }

    // 元の a[p] を取得する。0 <= p < N。時間 O(log(σ + 2))。
    T get(int p) const {
        assert(0 <= p && p < n_);
        int index = 0;
        for (int level = 0; level < height_; ++level) {
            const bool one = levels_[level].get(p);
            const int count = levels_[level].rank_one(p);
            index = (index << 1) | static_cast<int>(one);
            p = one ? zeros_[level] + count : p - count;
        }
        return values_[index];
    }

    // [l, r) 内で x に等しい要素数。空区間では0。時間 O(log(σ + 2))。
    int rank(int l, int r, T x) const {
        check_range(l, r);
        const int index = lower_index(x);
        if (index == static_cast<int>(values_.size()) || values_[index] != x) return 0;
        for (int level = 0; level < height_; ++level) {
            const int left = levels_[level].rank_one(l);
            const int right = levels_[level].rank_one(r);
            if ((index >> (height_ - 1 - level)) & 1) {
                l = zeros_[level] + left;
                r = zeros_[level] + right;
            } else {
                l -= left;
                r -= right;
            }
        }
        return r - l;
    }

    // [l, r) 内の小さい方から k 番目（0-indexed）。0 <= k < r-l。
    // 時間 O(log(σ + 2))。
    T kth_smallest(int l, int r, int k) const {
        check_range(l, r);
        assert(0 <= k && k < r - l);
        int index = 0;
        for (int level = 0; level < height_; ++level) {
            const int left = levels_[level].rank_one(l);
            const int right = levels_[level].rank_one(r);
            const int zero_count = (r - l) - (right - left);
            const bool one = k >= zero_count;
            index = (index << 1) | static_cast<int>(one);
            if (one) {
                k -= zero_count;
                l = zeros_[level] + left;
                r = zeros_[level] + right;
            } else {
                l -= left;
                r -= right;
            }
        }
        return values_[index];
    }

    // [l, r) 内の大きい方から k 番目（0-indexed）。0 <= k < r-l。
    // 時間 O(log(σ + 2))。
    T kth_largest(int l, int r, int k) const {
        check_range(l, r);
        assert(0 <= k && k < r - l);
        return kth_smallest(l, r, r - l - 1 - k);
    }

    // [l, r) 内で upper 未満の要素数。空区間では0。時間 O(log(σ + 2))。
    int range_freq(int l, int r, T upper) const {
        check_range(l, r);
        return count_less(l, r, lower_index(upper));
    }

    // [l, r) 内で値域 [lower, upper) に入る要素数。lower <= upper。
    // 空区間・空値域では0。時間 O(log(σ + 2))。
    int range_freq(int l, int r, T lower, T upper) const {
        check_range(l, r);
        assert(lower <= upper);
        return count_less(l, r, lower_index(upper)) - count_less(l, r, lower_index(lower));
    }

    // [l, r) 内で upper 未満の最大値。存在しなければ nullopt。時間 O(log(σ + 2))。
    std::optional<T> prev_value(int l, int r, T upper) const {
        const int count = range_freq(l, r, upper);
        if (count == 0) return std::nullopt;
        return kth_smallest(l, r, count - 1);
    }

    // [l, r) 内で lower 以上の最小値。存在しなければ nullopt。時間 O(log(σ + 2))。
    std::optional<T> next_value(int l, int r, T lower) const {
        const int count = range_freq(l, r, lower);
        if (count == r - l) return std::nullopt;
        return kth_smallest(l, r, count);
    }
};

} // namespace sora
