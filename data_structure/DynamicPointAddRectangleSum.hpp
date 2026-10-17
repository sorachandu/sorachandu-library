#pragma once

#include <algorithm>
#include <bit>
#include <cstddef>
#include <concepts>
#include <iterator>
#include <utility>
#include <vector>

namespace sora {

// 座標の事前登録なしで点加算・長方形和を処理する。
// Coordは全順序を持つコピー可能な型、TはT{}を零元とする加法可換群。
// 各演算・コピーはO(1)とし、代数的な法則は利用者が保証する。
// M回のaddに対し空間O(M log(M + 2))。各ブロックの点数はintの範囲内。
template <class Coord = long long, class T = long long>
requires std::totally_ordered<Coord> && std::copyable<Coord>
    && std::copyable<T> && std::default_initializable<T>
    && requires(T a, const T b) {
        { a += b } -> std::same_as<T&>;
        { b - b } -> std::convertible_to<T>;
    }
class DynamicPointAddRectangleSum {
public:
    // 座標と加算量を保持する点。
    struct Point { Coord x, y; T weight; };

private:
    // x座標順の点列を保持する静的重み付きウェーブレット行列。
    struct Block {
        std::vector<Point> points;
        std::vector<Coord> ys;
        std::vector<T> total;
        std::vector<std::vector<int>> zero_count;
        std::vector<std::vector<T>> zero_sum;
        std::vector<int> middle;
        int height = 0;

        Block() = default;
        explicit Block(std::vector<Point> p) : points(std::move(p)) {
            std::sort(points.begin(), points.end(),
                      [](const Point& a, const Point& b) { return a.x < b.x; });
            const int n = static_cast<int>(points.size());
            ys.reserve(n);
            total.assign(n + 1, T{});
            for (int i = 0; i < n; ++i) {
                ys.push_back(points[i].y);
                total[i + 1] = total[i];
                total[i + 1] += points[i].weight;
            }
            std::sort(ys.begin(), ys.end());
            ys.erase(std::unique(ys.begin(), ys.end()), ys.end());
            height = static_cast<int>(std::bit_width(
                static_cast<unsigned>(ys.size() - 1)));
            std::vector<int> rank(n), next_rank(n);
            std::vector<T> weight(n), next_weight(n);
            for (int i = 0; i < n; ++i) {
                rank[i] = static_cast<int>(std::lower_bound(
                    ys.begin(), ys.end(), points[i].y) - ys.begin());
                weight[i] = points[i].weight;
            }
            zero_count.assign(height, std::vector<int>(n + 1));
            zero_sum.assign(height, std::vector<T>(n + 1, T{}));
            middle.resize(height);
            for (int level = 0; level < height; ++level) {
                const int bit = height - 1 - level;
                auto& cnt = zero_count[level];
                auto& sum = zero_sum[level];
                for (int i = 0; i < n; ++i) {
                    const bool zero = ((rank[i] >> bit) & 1) == 0;
                    cnt[i + 1] = cnt[i] + zero;
                    sum[i + 1] = sum[i];
                    if (zero) sum[i + 1] += weight[i];
                }
                middle[level] = cnt[n];
                int a = 0, b = cnt[n];
                for (int i = 0; i < n; ++i) {
                    const int j = ((rank[i] >> bit) & 1) ? b++ : a++;
                    next_rank[j] = rank[i];
                    next_weight[j] = weight[i];
                }
                rank.swap(next_rank);
                weight.swap(next_weight);
            }
        }

        int lower_x(const Coord& x) const {
            return static_cast<int>(std::lower_bound(
                points.begin(), points.end(), x,
                [](const Point& p, const Coord& v) { return p.x < v; })
                - points.begin());
        }

        // xの添字が区間[l, r)に属し、圧縮後のy座標がk未満である点の重みの和。
        T less(int l, int r, int k) const {
            if (k == 0 || l == r) return T{};
            if (k == static_cast<int>(ys.size())) return total[r] - total[l];
            T ans{};
            for (int level = 0; level < height; ++level) {
                const auto& cnt = zero_count[level];
                const int a = cnt[l], b = cnt[r];
                if ((k >> (height - 1 - level)) & 1) {
                    ans += zero_sum[level][r] - zero_sum[level][l];
                    l = middle[level] + l - a;
                    r = middle[level] + r - b;
                } else {
                    l = a;
                    r = b;
                }
            }
            return ans;
        }

        T rectangle_sum(const Coord& l, const Coord& d,
                        const Coord& r, const Coord& u) const {
            const int a = lower_x(l), b = lower_x(r);
            const int low = static_cast<int>(std::lower_bound(
                ys.begin(), ys.end(), d) - ys.begin());
            const int high = static_cast<int>(std::lower_bound(
                ys.begin(), ys.end(), u) - ys.begin());
            return less(a, b, high) - less(a, b, low);
        }
    };

    std::vector<Block> blocks;
    std::size_t count = 0;

public:
    DynamicPointAddRectangleSum() = default;

    // (x, y)にdeltaを加算する。座標の重複・負の加算量も許す。
    // ならしO(log²(M + 2))。1回の最悪時間はO((M + 1) log(M + 2))。
    void add(Coord x, Coord y, T delta) {
        std::vector<Point> p{{std::move(x), std::move(y), std::move(delta)}};
        std::size_t level = 0;
        while (level < blocks.size() && !blocks[level].points.empty()) {
            auto& old = blocks[level].points;
            p.insert(p.end(), std::make_move_iterator(old.begin()),
                     std::make_move_iterator(old.end()));
            blocks[level] = Block{};
            ++level;
        }
        if (level == blocks.size()) blocks.emplace_back();
        blocks[level] = Block(std::move(p));
        ++count;
    }

    // [l, r) × [d, u)の重みの和。空・逆向きの長方形は零元を返す。 O(log²(M + 2))。
    [[nodiscard]] T rectangle_sum(const Coord& l, const Coord& d,
                                  const Coord& r, const Coord& u) const {
        if (!(l < r) || !(d < u)) return T{};
        T ans{};
        for (const auto& b : blocks)
            if (!b.points.empty()) ans += b.rectangle_sum(l, d, r, u);
        return ans;
    }

    // 異なる座標数ではなく、最後のclear以降のadd呼び出し回数。O(1)。
    [[nodiscard]] std::size_t size() const { return count; }
    [[nodiscard]] bool empty() const { return count == 0; }
    // 全点を削除する。O(M log(M + 2))。
    void clear() { blocks.clear(); count = 0; }
};

}  // namespace sora
