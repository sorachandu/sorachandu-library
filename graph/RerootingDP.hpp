#pragma once

#include <cassert>
#include <ranges>
#include <utility>
#include <vector>

namespace sora {

/*//--------------------------------------------------------
Copyright (c) 2022 Noya2
Released under the MIT license
refer to: https://noya2ruler.github.io/noya2_Library/tree/rerootingdp.hpp
----------------------------------------------------------*/
// 全方位木DP。入力は空でない連結な無向木で、変更しない。
// E: 隣接する部分木からの寄与。op(E,E)->E と単位元eは可換モノイドを成すこと。
// V: 頂点で確定したDP値。put_vertex(E, 頂点番号)->V、put_edge(V, 辺ID)->E。
// g[v]の(c,eid)に対し、子cの値をvへ取り込むときにput_edge(child_value,eid)を呼ぶ。
// 逆向きには~eidを使うため、g[c]には(v,~eid)を格納すること。
// V/Eはデフォルト構築・コピーが可能であること。
// 各演算・コピーをO(1)として、前処理時間・空間O(N)。再帰深さは最悪O(N)。
// rootは内部の前処理用の根。戻り値では全ての根に対するクエリを処理できる。
template<class V, class E>
auto rerootingdp(auto op, E e, auto put_edge, auto put_vertex, const std::vector<std::vector<std::pair<int, int>>> &g, int root = 0){
    // 各根の木全体・部分木のDP値を保持するクエリオブジェクト。
    struct Dp {
        // operator()(r,v): rを根としたときのvの部分木のDP値。
        // ans[v]: vを根とする木全体のDP値。
        // from[v]: vを根としたときの、前処理上の親の部分木のDP値。
        // to[v]: 前処理上の親を根としたときのvの部分木のDP値。
        // 前処理上の根のfrom/toは未定義。クエリ経由では参照しない。
        std::vector<V> ans, from, to;
        std::vector<int> down, up;
        std::vector<std::vector<int>> childs;
        // vがrの真の子孫か。時間O(1)。
        bool is_in_subtree(int r, int v) const {
            return down[r] < down[v] && up[v] <= up[r];
        }
        // vを根とする木全体のDP値。時間O(1)。
        V operator()(int v) const {
            return ans[v];
        }
        // rを根としたときのvの部分木のDP値。最悪時間O(log N)。
        V operator()(int r, int v) const {
            if (r == v) return ans[v];
            if (!is_in_subtree(v, r)) return to[v];
            int le = 0, ri = childs[v].size();
            while (ri - le > 1){
                int md = (le + ri) / 2;
                if (down[childs[v][md]] <= down[r]){
                    le = md;
                }
                else {
                    ri = md;
                }
            }
            return from[childs[v][le]];
        }
    };
    int n = static_cast<int>(g.size());
    assert(0 <= root && root < n);
    std::vector<E> from(n, e), to(n, e);
    std::vector<V> dp_to(n);
    std::vector<std::vector<int>> childs(n);
    std::vector<int> down(n), up(n);
    int t = 0;
    auto dfs = [&](auto sfs, int v, int f) -> void {
        down[v] = t++;
        childs[v].reserve(g[v].size());
        for (auto [c, eid] : g[v]){
            if (c == f) continue;
            childs[v].emplace_back(c);
            sfs(sfs, c, v);
            dp_to[c] = put_vertex(to[c], c);
            to[c] = put_edge(dp_to[c], eid);
            to[v] = op(to[v], to[c]);
        }
        up[v] = t;
    };
    dfs(dfs, root, -1);
    std::vector<V> dp_ans(n), dp_from(n);
    std::vector<E> inner(n);
    auto bfs = [&](auto sfs, int v, int f) -> void {
        int sz = g[v].size();
        inner[sz] = e;
        int i = sz-1;
        for (auto [c, eid] : g[v] | std::views::reverse){
            if (c == f) continue;
            inner[i] = op(inner[i+1], to[c]);
            i--;
        }
        dp_ans[v] = put_vertex(op(inner[++i], from[v]), v);
        E rui = e;
        for (auto [c, eid] : g[v]){
            if (c == f) continue;
            dp_from[c] = put_vertex(op(op(rui, inner[++i]), from[v]), v);
            from[c] = put_edge(dp_from[c], ~eid);
            rui = op(rui, to[c]);
        }
        for (auto [c, eid] : g[v]){
            if (c == f) continue;
            sfs(sfs, c, v);
        }
    };
    bfs(bfs, root, -1);
    return Dp{std::move(dp_ans), std::move(dp_from), std::move(dp_to),
              std::move(down), std::move(up), std::move(childs)};
}

} // namespace sora
