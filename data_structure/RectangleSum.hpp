#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

namespace sora {

// 静的な重み付き点集合に対する半開長方形の和。構築後の点追加・重み更新は非対応。
// Coordはコピー可能な全順序型、TはT{}を零元とする加法可換群。各演算・コピーをO(1)とする。
// N <= INT_MAX。構築時間・保持空間O(N log N)、問い合わせO(log N)。
// 座標の重複・負の重みを許す。代数的な法則と演算結果が型の範囲に収まることは利用者が保証する。
template <class Coord = long long, class T = long long>
requires std::totally_ordered<Coord> && std::copyable<Coord>
    && std::copyable<T> && std::default_initializable<T>
    && requires(T a, const T b) {
        { a += b } -> std::same_as<T&>;
        { b - b } -> std::convertible_to<T>;
    }
class RectangleSum {
public:
    // 点の座標と重み。同じ座標の点も個別に数える。
    struct Point { Coord x, y; T weight; };

private:
    // 各段のビット列と、0側の要素だけを加えた累積和。
    struct Level {
        std::vector<std::uint64_t> bits;
        std::vector<int> block_prefix;
        std::vector<T> zero_sum;
        int zero_count = 0;

        explicit Level(int n)
            : bits((static_cast<std::size_t>(n) + 63) / 64),
              zero_sum(static_cast<std::size_t>(n) + 1, T{}) {}

        void set_one(int p) { bits[p / 64] |= std::uint64_t{1} << (p % 64); }

        void build_rank() {
            block_prefix.resize(bits.size() + 1);
            for (std::size_t i = 0; i < bits.size(); ++i)
                block_prefix[i + 1] = block_prefix[i] + std::popcount(bits[i]);
        }

        int rank_zero(int r) const {
            const int offset = r % 64;
            // rが64の倍数なら末尾ブロックへのアクセスと64bitシフトを避ける。
            const int ones = block_prefix[r / 64] + (offset == 0 ? 0 :
                std::popcount(bits[r / 64] & ((std::uint64_t{1} << offset) - 1)));
            return r - ones;
        }
    };

    int n_ = 0;
    int height_ = 0;
    std::vector<Coord> xs_, ys_;
    std::vector<T> total_;
    std::vector<Level> levels_;

    int lower_y(const Coord& y) const {
        return static_cast<int>(std::lower_bound(ys_.begin(), ys_.end(), y) - ys_.begin());
    }

    // x順の添字が[l, r)にあり、圧縮後のyがupper未満の点の和。
    T sum_less(int l, int r, int upper) const {
        if (upper == 0 || l == r) return T{};
        if (upper == static_cast<int>(ys_.size())) return total_[r] - total_[l];
        T result{};
        for (int level = 0; level < height_; ++level) {
            const auto& row = levels_[level];
            const int left = row.rank_zero(l), right = row.rank_zero(r);
            if ((upper >> (height_ - 1 - level)) & 1) {
                result += row.zero_sum[r] - row.zero_sum[l];
                l = row.zero_count + (l - left);
                r = row.zero_count + (r - right);
            } else {
                l = left;
                r = right;
            }
        }
        return result;
    }

public:
    // 空の点集合を構築する。時間・空間O(1)。
    RectangleSum() = default;

    // 全点から構築する。時間・保持空間O(N log N)、作業空間O(N)。
    // 引数は値渡し。左辺値の入力は変更せず、右辺値なら入力領域を受け取れる。
    explicit RectangleSum(std::vector<Point> points) {
        assert(points.size() <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
        n_ = static_cast<int>(points.size());
        if (empty()) return;
        std::sort(points.begin(), points.end(),
                  [](const Point& a, const Point& b) { return a.x < b.x; });
        xs_.reserve(n_);
        ys_.reserve(n_);
        total_.assign(static_cast<std::size_t>(n_) + 1, T{});
        for (int i = 0; i < n_; ++i) {
            xs_.push_back(points[i].x);
            ys_.push_back(points[i].y);
            total_[i + 1] = total_[i];
            total_[i + 1] += points[i].weight;
        }
        std::sort(ys_.begin(), ys_.end());
        ys_.erase(std::unique(ys_.begin(), ys_.end()), ys_.end());
        height_ = static_cast<int>(std::bit_width(static_cast<unsigned int>(ys_.size() - 1)));
        std::vector<int> rank(n_), next_rank(n_);
        std::vector<T> weight(n_, T{}), next_weight(n_, T{});
        for (int i = 0; i < n_; ++i) {
            rank[i] = lower_y(points[i].y);
            weight[i] = points[i].weight;
        }
        levels_.reserve(height_);
        for (int bit = height_ - 1; bit >= 0; --bit) {
            levels_.emplace_back(n_);
            auto& row = levels_.back();
            for (int i = 0; i < n_; ++i) {
                const bool one = (rank[i] >> bit) & 1;
                row.zero_sum[i + 1] = row.zero_sum[i];
                if (one) row.set_one(i);
                else row.zero_sum[i + 1] += weight[i];
            }
            row.build_rank();
            row.zero_count = row.rank_zero(n_);
            int zero_pos = 0, one_pos = row.zero_count;
            for (int i = 0; i < n_; ++i) {
                const int dest = ((rank[i] >> bit) & 1) ? one_pos++ : zero_pos++;
                next_rank[dest] = rank[i];
                next_weight[dest] = weight[i];
            }
            rank.swap(next_rank);
            weight.swap(next_weight);
        }
    }

    // 重複を含む入力点数を返す。
    [[nodiscard]] int size() const { return n_; }
    [[nodiscard]] bool empty() const { return n_ == 0; }

    // [l, r) × [d, u)の重みの和。時間O(log N)。点集合は変更しない。
    // 空集合・空長方形・逆向きの長方形ではT{}を返す。
    [[nodiscard]] T rectangle_sum(const Coord& l, const Coord& d,
                                  const Coord& r, const Coord& u) const {
        if (empty() || !(l < r) || !(d < u)) return T{};
        const int left = static_cast<int>(std::lower_bound(xs_.begin(), xs_.end(), l) - xs_.begin());
        const int right = static_cast<int>(std::lower_bound(xs_.begin(), xs_.end(), r) - xs_.begin());
        return sum_less(left, right, lower_y(u)) - sum_less(left, right, lower_y(d));
    }
};

} // namespace sora
