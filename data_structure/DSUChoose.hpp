#pragma once

#include <cassert>
#include <numeric>
#include <utility>
#include <vector>

namespace sora {

// Union by Rankと経路圧縮を使い、論理的な代表元だけを自由に選べるUnion-Find。
// 構造上の根はpar_root、論理的な代表元はrpr_rootで取得する。空間O(N)。
class DSUChoose {
    std::vector<int> par, rnk, rpr;

public:
    // 空の構造を作る。時間・空間O(1)。
    DSUChoose() = default;

    // N要素を作る。N >= 0。時間・空間O(N)。
    explicit DSUChoose(int n) : par(n, -1), rnk(n, 0), rpr(n) {
        std::iota(rpr.begin(), rpr.end(), 0);
    }

    // 構造上の根。償却時間O(α(N))。
    int par_root(int v) {
        assert(0 <= v && v < static_cast<int>(par.size()));
        return par[v] == -1 ? v : par[v] = par_root(par[v]);
    }

    // 論理的な代表元。償却時間O(α(N))。
    int rpr_root(int v) { return rpr[par_root(v)]; }

    // choose(代表元u, 代表元v)で新しい論理代表元を選ぶ。同じ成分なら呼ばない。
    // 償却時間O(α(N) + C)。Cはchooseの実行時間。chooseは代表元として成分内の頂点を返すこと。
    void merge(int u, int v, auto choose) {
        int ru = par_root(u), rv = par_root(v);
        if (ru == rv) return;
        const int new_rpr = choose(rpr[ru], rpr[rv]);
        if (rnk[ru] < rnk[rv]) std::swap(ru, rv);
        par[rv] = ru;
        if (rnk[ru] == rnk[rv]) ++rnk[ru];
        rpr[ru] = new_rpr;
    }

    // 同じ成分かを返す。償却時間O(α(N))。
    bool same(int u, int v) { return par_root(u) == par_root(v); }
};

} // namespace sora
