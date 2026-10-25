#define PROBLEM "https://judge.yosupo.jp/problem/sort_points_by_argument"

#include "../geometry/ArgumentSort.hpp"
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<sora::Point<long long>> points(n);
    for (auto& p : points) std::cin >> p.x >> p.y;
    sora::argument_sort(points);
    for (const auto& p : points) std::cout << p.x << ' ' << p.y << '\n';
}
