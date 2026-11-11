#include "../../data_structure/WaveletMatrix.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <optional>
#include <random>
#include <type_traits>
#include <vector>

template <class T>
std::vector<T> thresholds() {
    std::vector<T> result{
        std::numeric_limits<T>::lowest(), T{0}, std::numeric_limits<T>::max()};
    if constexpr (std::is_signed_v<T>) {
        result.push_back(T{-1});
        result.push_back(T{1});
    } else {
        result.push_back(T{1});
    }
    for (int x = -5; x <= 5; ++x) result.push_back(static_cast<T>(x));
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    return result;
}

template <class T>
void check_range(const sora::WaveletMatrix<T>& wm, const std::vector<T>& values,
                 int left, int right, const std::vector<T>& query_values) {
    std::vector<T> sorted(values.begin() + left, values.begin() + right);
    std::sort(sorted.begin(), sorted.end());

    for (T x : query_values) {
        int count = 0;
        for (int i = left; i < right; ++i) count += values[i] == x;
        assert(wm.rank(left, right, x) == count);

        count = 0;
        for (int i = left; i < right; ++i) count += values[i] < x;
        assert(wm.range_freq(left, right, x) == count);

        std::optional<T> previous;
        std::optional<T> next;
        for (T value : sorted) {
            if (value < x) previous = value;
            if (!next && value >= x) next = value;
        }
        assert(wm.prev_value(left, right, x) == previous);
        assert(wm.next_value(left, right, x) == next);
    }

    for (T lower : query_values) {
        for (T upper : query_values) {
            if (upper < lower) continue;
            int count = 0;
            for (int i = left; i < right; ++i)
                count += lower <= values[i] && values[i] < upper;
            assert(wm.range_freq(left, right, lower, upper) == count);
        }
    }

    for (int k = 0; k < right - left; ++k) {
        assert(wm.kth_smallest(left, right, k) == sorted[k]);
        assert(wm.kth_largest(left, right, k) == sorted[sorted.size() - 1 - k]);
    }
}

template <class T>
void check_case(const std::vector<T>& values, bool exhaustive_ranges) {
    const sora::WaveletMatrix<T> wm(values);
    assert(wm.size() == static_cast<int>(values.size()));
    assert(wm.empty() == values.empty());
    for (int i = 0; i < static_cast<int>(values.size()); ++i)
        assert(wm.get(i) == values[i]);

    const auto query_values = thresholds<T>();
    if (values.empty()) {
        const sora::WaveletMatrix<T> default_empty;
        assert(default_empty.size() == 0 && default_empty.empty());
        check_range(default_empty, values, 0, 0, query_values);
    }
    if (exhaustive_ranges) {
        for (int left = 0; left <= static_cast<int>(values.size()); ++left)
            for (int right = left; right <= static_cast<int>(values.size()); ++right)
                check_range(wm, values, left, right, query_values);
    } else {
        check_range(wm, values, 0, static_cast<int>(values.size()), query_values);
        check_range(wm, values, 0, 0, query_values);
        check_range(wm, values, static_cast<int>(values.size()),
                    static_cast<int>(values.size()), query_values);
        std::mt19937 rng(481516);
        for (int iteration = 0; iteration < 120; ++iteration) {
            int left = static_cast<int>(rng() % (values.size() + 1));
            int right = static_cast<int>(rng() % (values.size() + 1));
            if (left > right) std::swap(left, right);
            check_range(wm, values, left, right, query_values);
        }
    }
}

template <class T>
void run_type() {
    check_case(std::vector<T>{}, true);
    check_case(std::vector<T>{T{7}}, true);
    check_case(std::vector<T>(12, T{3}), true);

    std::mt19937 rng(20261111);
    for (int n = 0; n <= 12; ++n) {
        std::vector<T> values(n);
        for (T& value : values) value = static_cast<T>(static_cast<int>(rng() % 11) - 5);
        check_case(values, true);
    }

    for (int n : {63, 64, 65, 127, 128, 129}) {
        std::vector<T> patterned(n);
        for (int i = 0; i < n; ++i)
            patterned[i] = static_cast<T>((i * 37 + i / 3) % 17 - 8);
        check_case(patterned, false);

        std::vector<T> boundary(n, T{0});
        if (n > 0) boundary[0] = std::numeric_limits<T>::lowest();
        if (n > 1) boundary[1] = std::numeric_limits<T>::max();
        for (int i = 2; i < n; ++i)
            boundary[i] = static_cast<T>(static_cast<int>(rng() % 9) - 4);
        check_case(boundary, false);
    }
}

int main() {
    run_type<std::int64_t>();
    run_type<std::uint64_t>();
}
