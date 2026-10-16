#pragma once

#include <cassert>
#include <cstddef>
#include <vector>

namespace sora {

// 無向木の行きがけ・帰りがけの時刻と、頂点の往復列を求める。
// 時間 O(N)、出力を含む追加空間 O(N)。入力の隣接リストは変更しない。
// in/out/idxs は上書きする。時刻は 0 から始まり、全体で 2N 個。
// idxs は入場時に v、退場時に -v を格納する（頂点 0 は符号で区別できない）。
// 前提: Gは root を含む連結な無向木、または∅。頂点番号は [0, N)。
inline void euler_tour(const std::vector<std::vector<int>>& G,
                       std::vector<int>& in, std::vector<int>& out,
                       std::vector<int>& idxs, int root = 0) {
    assert(&in != &out && &in != &idxs && &out != &idxs);
    const int N = static_cast<int>(G.size());
    in.assign(N, -1);
    out.assign(N, -1);
    idxs.clear();
    if (N == 0) return;
    assert(0 <= root && root < N);
    idxs.reserve(static_cast<std::size_t>(N) * 2);

    // 再帰を使わず、長いパスでも呼び出しスタックを消費しない。
    struct Frame {
        int vertex;
        std::size_t next_edge;
    };
    std::vector<Frame> stack;
    stack.reserve(N);
    int time = 0;
    in[root] = time++;
    idxs.push_back(root);
    stack.push_back({root, 0});
    while (!stack.empty()) {
        auto& frame = stack.back();
        const int v = frame.vertex;
        if (frame.next_edge == G[v].size()) {
            out[v] = time++;
            idxs.push_back(-v);
            stack.pop_back();
            continue;
        }
        const int w = G[v][frame.next_edge++];
        assert(0 <= w && w < N);
        if (in[w] != -1) continue;
        in[w] = time++;
        idxs.push_back(w);
        stack.push_back({w, 0});
    }
    assert(idxs.size() == static_cast<std::size_t>(N) * 2);
}

} // namespace sora
