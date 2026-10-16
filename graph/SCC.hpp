#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <vector>

namespace sora {

// 頂点番号・成分IDは0-indexed。結果の配列は読み取り用として扱う。
struct SCCResult {
    std::vector<int> id;                     // id[v]: 頂点vの成分ID
    std::vector<std::vector<int>> groups;    // groups[c]: 成分cの頂点一覧
    std::vector<std::vector<int>> dag;       // 縮約グラフ（隣接順は未規定）
    std::vector<int> indegree, outdegree;    // dagに含まれる辺に基づく次数
    std::vector<bool> has_cycle;             // 自己ループも閉路として扱う
    std::vector<int> representative;         // 各成分の最小頂点番号

    // 成分数を返す。O(1)。
    [[nodiscard]] int count() const {
        return static_cast<int>(groups.size());
    }

    // 2頂点が同じ成分に属するかを返す。O(1)。
    [[nodiscard]] bool same(int u, int v) const {
        assert(0 <= u && static_cast<std::size_t>(u) < id.size());
        assert(0 <= v && static_cast<std::size_t>(v) < id.size());
        return id[u] == id[v];
    }

    // 成分の頂点数を返す。引数は元の頂点番号ではなく成分ID。O(1)。
    [[nodiscard]] int size(int c) const {
        assert(0 <= c && c < count());
        return static_cast<int>(groups[c].size());
    }

    // 必要な場合のみ生成。O(K + L)、K: 成分数、L: 縮約後の辺数。
    [[nodiscard]] std::vector<std::vector<int>> reverse_dag() const {
        std::vector<std::vector<int>> reversed(count());
        for (int c = 0; c < count(); ++c) {
            reversed[c].reserve(indegree[c]);
        }
        for (int c = 0; c < count(); ++c) {
            for (int d : dag[c]) reversed[d].push_back(c);
        }
        return reversed;
    }
};

// ACL非依存、非再帰Kosaraju法。
// コンストラクタ O(N)、add_edge ならしO(1)、build O(N+M)。
// グラフ保持・buildの作業領域・結果はそれぞれO(N+M)以内。
// NとMはintの範囲内とする。無効な引数のチェックはassert。
class SCCGraph {
public:
    // N頂点の空グラフを構築する。O(N)。
    explicit SCCGraph(int n = 0) : n_(n) {
        assert(n >= 0);
        graph_.resize(n);
        reversed_.resize(n);
    }

    // 頂点数を返す。O(1)。
    [[nodiscard]] int vertex_count() const { return n_; }
    // 多重辺・自己ループを含む辺数を返す。O(1)。
    [[nodiscard]] std::size_t edge_count() const { return m_; }

    // 有向辺を追加する。ならしO(1)。
    void add_edge(int from, int to) {
        assert(0 <= from && from < n_);
        assert(0 <= to && to < n_);
        graph_[from].push_back(to);
        reversed_[to].push_back(from);
        ++m_;
    }

    // 元のグラフを変更せず分解する。時間・追加空間O(N + M)。
    // 成分IDはトポロジカル順：成分間の辺u->vについてid[u]<id[v]。
    // デフォルトは多重辺を除去。trueなら成分間の多重辺を残す。
    // どちらの場合も成分内部の辺はdagに含めない。
    // 呼び出すたび再計算する。辺追加後も以前の結果は変更されない。
    [[nodiscard]] SCCResult build(bool keep_multi_edges = false) const {
        std::vector<unsigned char> visited(n_, 0);
        std::vector<std::size_t> next(n_, 0);
        std::vector<int> order, stack;
        order.reserve(n_);
        stack.reserve(n_);

        // 第1パス：元のグラフのDFS帰りがけ順。
        for (int root = 0; root < n_; ++root) {
            if (visited[root]) continue;
            visited[root] = 1;
            stack.push_back(root);
            while (!stack.empty()) {
                const int v = stack.back();
                if (next[v] == graph_[v].size()) {
                    order.push_back(v);
                    stack.pop_back();
                } else {
                    const int to = graph_[v][next[v]++];
                    if (!visited[to]) {
                        visited[to] = 1;
                        stack.push_back(to);
                    }
                }
            }
        }

        SCCResult result;
        result.id.assign(n_, -1);

        // 第2パス：帰りがけ順の逆順で逆グラフを探索。
        for (auto it = order.rbegin(); it != order.rend(); ++it) {
            const int root = *it;
            if (result.id[root] != -1) continue;
            const int c = result.count();
            result.groups.emplace_back();
            result.representative.push_back(root);
            result.id[root] = c;
            stack.push_back(root);
            while (!stack.empty()) {
                const int v = stack.back();
                stack.pop_back();
                result.groups[c].push_back(v);
                result.representative[c] =
                    std::min(result.representative[c], v);
                for (int to : reversed_[v]) {
                    if (result.id[to] == -1) {
                        result.id[to] = c;
                        stack.push_back(to);
                    }
                }
            }
        }

        const int k = result.count();
        result.dag.resize(k);
        result.indegree.assign(k, 0);
        result.outdegree.assign(k, 0);
        result.has_cycle.assign(k, false);

        // seen[d] == cなら、成分cからdへの辺は追加済み。
        // 同じ成分の全頂点をまとめて処理し、seenをリセットしない。
        std::vector<int> seen(k, -1);
        for (int c = 0; c < k; ++c) {
            result.has_cycle[c] = result.size(c) >= 2;
            for (int v : result.groups[c]) {
                for (int to : graph_[v]) {
                    const int d = result.id[to];
                    if (c == d) {
                        if (v == to) result.has_cycle[c] = true;
                        continue;
                    }
                    if (!keep_multi_edges && seen[d] == c) continue;
                    seen[d] = c;
                    result.dag[c].push_back(d);
                    ++result.outdegree[c];
                    ++result.indegree[d];
                }
            }
        }
        return result;
    }

private:
    int n_;
    std::size_t m_ = 0;
    std::vector<std::vector<int>> graph_, reversed_;
};

}  // namespace sora
