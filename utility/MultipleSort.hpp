#pragma once

#include <algorithm>
#include <cstddef>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace sora {

// 先頭配列をcompで比較して、全配列を同じ順序に並べ替える。入力配列を変更する。
// 戻り値のindices[i]は並べ替え後のi番目に対応する元の添字を表す。
// 同じキーを持つ要素同士の順序は未規定。配列の長さは同じで、配列同士は別オブジェクトであること。
// 長さNの配列K個について、比較・コピーをO(1)として時間O(K + N log(N+1) + KN)、追加空間O(N)。
template <typename Compare, typename First, typename... Rest>
std::vector<std::size_t> multiple_sort(Compare comp, First& first, Rest&... rest) {
    const std::size_t n = first.size();
    if (!((rest.size() == n) && ...)) {
        throw std::invalid_argument("Vectors must have the same size");
    }
    std::vector<std::size_t> indices(n);
    std::iota(indices.begin(), indices.end(), std::size_t{0});
    std::sort(indices.begin(), indices.end(), [&](std::size_t i, std::size_t j) {
        return comp(first[i], first[j]);
    });
    auto reorder = [&](auto& values) {
        const auto original = values;
        for (std::size_t i = 0; i < n; ++i) values[i] = original[indices[i]];
    };
    reorder(first);
    (reorder(rest), ...);
    return indices;
}

} // namespace sora
