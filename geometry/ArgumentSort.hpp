#pragma once

#include "Geometry2D.hpp"
#include <algorithm>
#include <vector>

namespace sora {

// 偏角(-π, π]、同じ偏角では原点からの距離の昇順で比較する。原点の偏角は0。
// T/Cの条件はPointと同じ。外積と距離の2乗の全中間値がCに収まること。
template <class T, class C = DefaultCalculation<T>>
struct ArgumentLess {
    // 時間・追加空間O(1)。浮動小数点型でもepsは使わない。
    constexpr bool operator()(const Point<T, C>& a, const Point<T, C>& b) const {
        const bool a_zero = a.x == T{} && a.y == T{};
        const bool b_zero = b.x == T{} && b.y == T{};
        if (a_zero) return !b_zero && !(b.y < T{});
        if (b_zero) return a.y < T{};
        const bool a_first = a.y < T{} || (a.y == T{} && a.x > T{});
        const bool b_first = b.y < T{} || (b.y == T{} && b.x > T{});
        if (a_first != b_first) return a_first;
        const C determinant = cross(a, b);
        if (determinant != C{}) return determinant > C{};
        return dot(a, a) < dot(b, b);
    }
};

// pointsを破壊的に偏角順へ並べ替える。順序と前提条件はArgumentLessを参照。
// N = points.size()として時間O(N log(N+1))、追加空間O(log(N+1))。安定ソートではない。
template <class T, class C>
void argument_sort(std::vector<Point<T, C>>& points) {
    std::sort(points.begin(), points.end(), ArgumentLess<T, C>{});
}

} // namespace sora
