// verification-helper: IGNORE
#include <cassert>
#include <vector>

#include "../graph/EulerTour.hpp"

// 対応するジャッジ問題を指定しないローカル検証。
int main() {
    std::vector<int> in, out, idxs;
    sora::euler_tour({}, in, out, idxs);
    assert(in.empty() && out.empty() && idxs.empty());
    sora::euler_tour({{}}, in, out, idxs);
    assert((in == std::vector<int>{0}));
    assert((out == std::vector<int>{1}));
    assert((idxs == std::vector<int>{0, 0}));
    const std::vector<std::vector<int>> g{{1, 2}, {0, 3}, {0}, {1}};
    sora::euler_tour(g, in, out, idxs);
    assert((in == std::vector<int>{0, 1, 5, 2}));
    assert((out == std::vector<int>{7, 4, 6, 3}));
    assert((idxs == std::vector<int>{0, 1, 3, -3, -1, 2, -2, 0}));
    sora::euler_tour(g, in, out, idxs, 3);
    assert(in[3] == 0 && out[3] == 7);
    const int n = 200000;
    std::vector<std::vector<int>> path(n);
    for (int v = 1; v < n; ++v) {
        path[v - 1].push_back(v);
        path[v].push_back(v - 1);
    }
    sora::euler_tour(path, in, out, idxs);
    for (int v = 0; v < n; ++v) {
        assert(in[v] == v && out[v] == 2 * n - 1 - v);
    }
}
