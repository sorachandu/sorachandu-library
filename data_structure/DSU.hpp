#pragma once

#include <cassert>
#include <vector>

namespace sora {

// 常にu側の根へv側の根を併合するUnion-Find。経路圧縮のみを行う。
// Union by Size/Rankは行わないため、基本操作の最悪時間はO(N)。空間O(N)。
class DSU {
    std::vector<int> par;

public:
    // 空の構造を作る。時間・空間O(1)。
    DSU() = default;

    // N要素を作る。N >= 0。時間・空間O(N)。
    explicit DSU(int n) : par(n, -1) {}

    // 根を返す。最悪時間O(N)。経路圧縮で内部状態を変更する。
    int root(int v) {
        assert(0 <= v && v < static_cast<int>(par.size()));
        int r = v;
        while (par[r] != -1) r = par[r];
        while (v != r) {
            const int next = par[v];
            par[v] = r;
            v = next;
        }
        return r;
    }

    // u側へv側を併合する。最悪時間O(N)。
    void merge(int u, int v) {
        u = root(u);
        v = root(v);
        if (u != v) par[v] = u;
    }

    // 同じ成分かを返す。最悪時間O(N)。
    bool same(int u, int v) { return root(u) == root(v); }
};

} // namespace sora
