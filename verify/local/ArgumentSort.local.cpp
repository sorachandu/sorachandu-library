#include "../../geometry/ArgumentSort.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <random>
#include <vector>

int main() {
    using P = sora::Point<long long>;
    const std::vector<P> expected{{-1, -1}, {0, -1}, {1, -1}, {0, 0}, {0, 0},
                                  {1, 0}, {2, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};
    auto points = expected;
    std::reverse(points.begin(), points.end());
    sora::argument_sort(points);
    assert(points == expected);
    std::vector<P> empty;
    sora::argument_sort(empty);
    std::vector<P> single{{0, 0}};
    sora::argument_sort(single);
    assert(single == std::vector<P>({{0, 0}}));
    constexpr long long big = 1000000000000000000LL;
    std::vector<P> large{{big, big}, {-big, big}, {big, -big}, {-big, -big}};
    sora::argument_sort(large);
    assert(large == std::vector<P>({{-big, -big}, {big, -big}, {big, big}, {-big, big}}));
    const sora::ArgumentLess<long long> less;
    std::vector<P> grid;
    for (int x = -3; x <= 3; ++x)
        for (int y = -3; y <= 3; ++y) grid.emplace_back(x, y);
    for (const auto& a : grid) {
        assert(!less(a, a));
        for (const auto& b : grid) {
            assert(!(less(a, b) && less(b, a)));
            for (const auto& c : grid) {
                if (less(a, b) && less(b, c)) assert(less(a, c));
                if (!less(a, b) && !less(b, a) && !less(b, c) && !less(c, b))
                    assert(!less(a, c) && !less(c, a));
            }
        }
    }
    std::mt19937 rng(123456);
    for (int iteration = 0; iteration < 100; ++iteration) {
        std::vector<P> sample;
        for (int i = 0; i < 100; ++i)
            sample.emplace_back(static_cast<int>(rng() % 2001) - 1000,
                                static_cast<int>(rng() % 2001) - 1000);
        auto reference = sample;
        std::sort(reference.begin(), reference.end(), [](P a, P b) {
            if (a == P{}) return b != P{} && b.y >= 0;
            if (b == P{}) return a.y < 0;
            const auto aa = std::atan2(static_cast<long double>(a.y), static_cast<long double>(a.x));
            const auto ab = std::atan2(static_cast<long double>(b.y), static_cast<long double>(b.x));
            if (aa != ab) return aa < ab;
            return sora::dot(a, a) < sora::dot(b, b);
        });
        sora::argument_sort(sample);
        assert(sample == reference);
    }
    std::vector<sora::Point<double>> floating{{-1, -0.0}, {0, -1}, {1, 0}, {0, 1}};
    sora::argument_sort(floating);
    assert(floating.front() == sora::Point<double>(0, -1));
    assert(floating.back() == sora::Point<double>(-1, 0));
}
