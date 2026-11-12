#include "../../data_structure/RectangleSum.hpp"

#include <cassert>
#include <cstdint>
#include <limits>
#include <random>
#include <string>
#include <utility>
#include <vector>

// 加法と減法を持つ独自の重み型。
struct CustomWeight {
    long long value = 0;
    CustomWeight() = default;
    constexpr CustomWeight(long long value_) : value(value_) {}
    constexpr CustomWeight& operator+=(const CustomWeight& rhs) {
        value += rhs.value;
        return *this;
    }
    friend constexpr CustomWeight operator-(CustomWeight lhs, CustomWeight rhs) {
        return CustomWeight(lhs.value - rhs.value);
    }
    friend bool operator==(const CustomWeight&, const CustomWeight&) = default;
};

template <class Coord, class T>
void check_rectangles(const std::vector<typename sora::RectangleSum<Coord, T>::Point>& points,
                      const std::vector<Coord>& bounds, int random_queries = 100) {
    using Rectangle = sora::RectangleSum<Coord, T>;
    const auto original = points;
    const Rectangle rectangles(points);
    assert(rectangles.size() == static_cast<int>(points.size()));
    assert(rectangles.empty() == points.empty());
    assert(points.size() == original.size());
    for (std::size_t i = 0; i < points.size(); ++i) {
        assert(points[i].x == original[i].x);
        assert(points[i].y == original[i].y);
        assert(points[i].weight == original[i].weight);
    }

    auto oracle = [&](const Coord& left, const Coord& down,
                      const Coord& right, const Coord& up) {
        T sum{};
        if (!(left < right) || !(down < up)) return sum;
        for (const auto& point : points)
            if (!(point.x < left) && point.x < right &&
                !(point.y < down) && point.y < up)
                sum += point.weight;
        return sum;
    };

    for (const Coord& left : bounds) {
        for (const Coord& right : bounds) {
            for (const Coord& down : bounds) {
                for (const Coord& up : bounds) {
                    assert(rectangles.rectangle_sum(left, down, right, up) ==
                           oracle(left, down, right, up));
                }
            }
        }
    }

    if (!bounds.empty()) {
        std::mt19937 rng(778899);
        for (int iteration = 0; iteration < random_queries; ++iteration) {
            const Coord& left = bounds[rng() % bounds.size()];
            const Coord& right = bounds[rng() % bounds.size()];
            const Coord& down = bounds[rng() % bounds.size()];
            const Coord& up = bounds[rng() % bounds.size()];
            assert(rectangles.rectangle_sum(left, down, right, up) ==
                   oracle(left, down, right, up));
        }
    }

    Rectangle copy = rectangles;
    Rectangle assigned;
    assigned = copy;
    assert(assigned.size() == rectangles.size());
    for (const Coord& left : bounds)
        for (const Coord& right : bounds)
            assert(assigned.rectangle_sum(left, bounds.front(), right, bounds.back()) ==
                   oracle(left, bounds.front(), right, bounds.back()));
}

int main() {
    using Rectangle = sora::RectangleSum<>;
    using Point = Rectangle::Point;
    const long long low = std::numeric_limits<long long>::lowest();
    const long long high = std::numeric_limits<long long>::max();
    const std::vector<long long> signed_bounds{low, -5, -1, 0, 1, 5, high};

    Rectangle empty;
    assert(empty.empty() && empty.size() == 0);
    assert(empty.rectangle_sum(-1, -1, 1, 1) == 0);
    std::vector<Point> no_points;
    check_rectangles<long long, long long>(no_points, signed_bounds, 5);

    const std::vector<Point> edge_points{
        {low, low, 7}, {low, high, -4}, {high, low, 9}, {high, high, -3},
        {0, 0, 0}, {0, 0, -8}, {-1, 0, 5}, {1, -1, -6}};
    check_rectangles<long long, long long>(edge_points, signed_bounds);
    Rectangle moved{std::vector<Point>(edge_points)};
    assert(moved.size() == static_cast<int>(edge_points.size()));
    assert(moved.rectangle_sum(low, low, high, high) ==
           -2);

    std::mt19937 rng(20261112);
    for (int n : {1, 2, 3, 63, 64, 65, 127, 128, 129}) {
        std::vector<Point> points;
        points.reserve(n);
        for (int i = 0; i < n; ++i) {
            const long long x = static_cast<int>(rng() % 19) - 9;
            const long long y = static_cast<int>(rng() % 17) - 8;
            const long long weight = static_cast<int>(rng() % 41) - 20;
            points.push_back({x, y, weight});
        }
        check_rectangles<long long, long long>(
            points, {-10, -9, -1, 0, 1, 8, 9, 10}, 250);

        std::vector<Point> same_x, same_y;
        for (const auto& point : points) {
            same_x.push_back({4, point.y, point.weight});
            same_y.push_back({point.x, -3, point.weight});
        }
        check_rectangles<long long, long long>(same_x, {-5, 3, 4, 5}, 60);
        check_rectangles<long long, long long>(same_y, {-5, -3, -2, 5}, 60);
    }

    using UnsignedPoint = sora::RectangleSum<std::uint64_t>::Point;
    const std::uint64_t umax = std::numeric_limits<std::uint64_t>::max();
    const std::vector<UnsignedPoint> unsigned_points{
        {0, 0, 3}, {0, umax, -5}, {1, 1, 7}, {umax, 0, -9}, {umax, umax, 11}};
    check_rectangles<std::uint64_t, long long>(
        unsigned_points, {0, 1, 2, umax}, 80);

    using StringPoint = sora::RectangleSum<std::string, CustomWeight>::Point;
    const std::vector<StringPoint> string_points{
        {"a", "x", 4}, {"b", "y", -3}, {"b", "y", 8},
        {"c", "x", -2}, {"z", "z", 6}};
    check_rectangles<std::string, CustomWeight>(
        string_points, {"", "a", "b", "c", "d", "z", "zz"}, 100);

    using FloatPoint = sora::RectangleSum<int, double>::Point;
    const std::vector<FloatPoint> float_points{
        {-2, 1, 1.5}, {-2, 1, -0.5}, {0, 0, 3.0}, {4, 8, -2.0}};
    check_rectangles<int, double>(float_points, {-3, -2, -1, 0, 1, 4, 5, 9}, 80);
}
