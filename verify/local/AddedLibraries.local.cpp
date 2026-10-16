#include <algorithm>
#include <cassert>
#include <limits>
#include <random>
#include <set>
#include <vector>
#include "../../graph/SCC.hpp"
#include "../../data_structure/IntervalSet.hpp"
#include "../../data_structure/DynamicPointAddRectangleSum.hpp"

int main() {
    std::mt19937 rng(12345);
    assert(sora::SCCGraph().build().count() == 0);
    for (int trial = 0; trial < 200; ++trial) {
        const int n = 1 + rng() % 15;
        sora::SCCGraph graph(n);
        std::vector edges(n, std::vector<int>(n));
        for (int i = 0; i < n * n; ++i) {
            int u = rng() % n, v = rng() % n;
            graph.add_edge(u, v);
            ++edges[u][v];
        }
        auto reach = edges;
        for (int i = 0; i < n; ++i) reach[i][i] = 1;
        for (int k = 0; k < n; ++k)
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    if (reach[i][k] && reach[k][j]) reach[i][j] = 1;
        for (bool multi : {false, true}) {
            auto result = graph.build(multi);
            std::vector expected(result.count(), std::vector<int>(result.count()));
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    assert(result.same(i, j) == (reach[i][j] && reach[j][i]));
                    if (edges[i][j] && !result.same(i, j)) {
                        assert(result.id[i] < result.id[j]);
                        auto& value = expected[result.id[i]][result.id[j]];
                        if (multi) value += edges[i][j];
                        else value = 1;
                    }
                }
            }
            auto reverse = result.reverse_dag();
            for (int c = 0; c < result.count(); ++c) {
                std::vector<int> actual(result.count());
                for (int d : result.dag[c]) ++actual[d];
                assert(actual == expected[c]);
                assert(result.outdegree[c] == static_cast<int>(result.dag[c].size()));
                assert(result.indegree[c] == static_cast<int>(reverse[c].size()));
                assert(result.representative[c] == *std::min_element(result.groups[c].begin(), result.groups[c].end()));
                bool cycle = result.size(c) > 1;
                for (int v : result.groups[c]) cycle = cycle || edges[v][v];
                assert(result.has_cycle[c] == cycle);
            }
        }
    }
    sora::SCCGraph chain(200000);
    for (int i = 1; i < chain.vertex_count(); ++i) chain.add_edge(i - 1, i);
    assert(chain.build().count() == chain.vertex_count());

    sora::IntervalSet intervals;
    std::set<int> naive;
    for (int step = 0; step < 5000; ++step) {
        int l = static_cast<int>(rng() % 61) - 30;
        int r = static_cast<int>(rng() % 61) - 30;
        if (rng() % 2) {
            intervals.insert(l, r);
            for (int x = l; x < r; ++x) naive.insert(x);
        } else {
            intervals.erase(l, r);
            for (int x = l; x < r; ++x) naive.erase(x);
        }
        bool all = true, any = false;
        for (int x = l; x < r; ++x) {
            all = all && naive.contains(x);
            any = any || naive.contains(x);
        }
        assert(intervals.contains(l, r) == all);
        assert(intervals.intersects(l, r) == any);
        for (int x = -31; x <= 31; ++x) {
            assert(intervals.contains(x) == naive.contains(x));
            auto found = intervals.find_interval(x);
            assert(found.has_value() == naive.contains(x));
            int mex = x;
            while (naive.contains(mex)) ++mex;
            assert(intervals.mex(x) == mex);
        }
        long long end = -100;
        std::set<int> rebuilt;
        for (auto [a, b] : intervals.intervals()) {
            assert(end < a && a < b);
            for (int x = a; x < b; ++x) rebuilt.insert(x);
            end = b;
        }
        assert(rebuilt == naive);
    }
    intervals.clear();
    assert(intervals.empty());
    const auto lo = std::numeric_limits<long long>::min();
    const auto hi = std::numeric_limits<long long>::max();
    intervals.insert(lo, hi);
    assert(intervals.mex(lo) == hi && !intervals.contains(hi));
    intervals.erase(-1, 1);
    assert(intervals.size() == 2 && intervals.mex(lo) == -1);

    sora::DynamicPointAddRectangleSum<> points;
    using Point = sora::DynamicPointAddRectangleSum<>::Point;
    std::vector<Point> data;
    assert(points.empty() && points.rectangle_sum(-1, -1, 1, 1) == 0);
    for (int step = 0; step < 3000; ++step) {
        if (step % 751 == 750) {
            points.clear();
            data.clear();
            assert(points.empty());
        }
        long long x = static_cast<int>(rng() % 21) - 10;
        long long y = static_cast<int>(rng() % 21) - 10;
        long long w = static_cast<int>(rng() % 31) - 15;
        points.add(x, y, w);
        data.push_back({x, y, w});
        assert(points.size() == data.size());
        for (int query = 0; query < 10; ++query) {
            long long l = static_cast<int>(rng() % 25) - 12;
            long long r = static_cast<int>(rng() % 25) - 12;
            long long d = static_cast<int>(rng() % 25) - 12;
            long long u = static_cast<int>(rng() % 25) - 12;
            long long sum = 0;
            for (auto p : data)
                if (l <= p.x && p.x < r && d <= p.y && p.y < u) sum += p.weight;
            assert(points.rectangle_sum(l, d, r, u) == sum);
        }
    }
}
