---
title: "HeavyLightDecomposition — 重軽分解"
documentation_of: ./HeavyLightDecomposition.hpp
---

## 概要

木を重いパスに分解し、頂点を配列の添字に対応付けます。パスを区間へ分け、Fenwick Tree等と組み合わせて使えます。Nは頂点数、保持空間O(N)。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `HeavyLightDecomposition(g, root = 0)` | 木をコピーして構築 | O(N) |
| `num_vertices()` | 頂点数 | O(1) |
| `depth(v)` / `parent(v)` | 深さ / 親（根は−1） | O(1) |
| `to_seq(v)` / `to_vertex(i)` | 頂点と配列添字の相互変換 | O(1) |
| `heavy_root(v)` / `heavy_child(v)` | 重いパスの先頭 / 重い子（なければ−1） | O(1) |
| `same(u, v)` / `dist_to_heavy_root(v)` | 同じ重いパスか / 先頭までの距離 | O(1) |
| `normalize_pair(u, v)` | 配列添字の昇順になるよう引数を交換 | O(1) |
| `heavy_path(v, stop_at_vertex = false)` | パス先頭からの頂点列 | O(K)、Kは出力頂点数 |
| `la(v, d)` / `lca(u, v)` | 深さdの祖先 / 最近共通祖先 | O(log N) |
| `dist(u, v)` / `jump(u, v, i)` | 距離 / u→vのi番目の頂点 | O(log N) |

## 仕様・使い方

空でない連結な無向木が必要です。頂点は[0, N)。入力は値渡しでコピーし、呼び出し元の木を変更しません。構築は再帰を使い、最悪深さO(N)です。

部分木は`[to_seq(v), to_seq(v) + 部分木サイズ)`の連続区間になります。`heavy_path`は配列添字ではなく頂点番号を返します。stop_at_vertex=falseならvより先のパス末尾まで含めます。

laのdは「上る辺数」ではなく目標の深さで、0 <= d <= depth(v)です。jumpはi >= 0、i = 0ならu、パス長を超えれば−1です。normalize_pairは引数を変更します。

## 使用例

{% raw %}
```cpp
#include "graph/HeavyLightDecomposition.hpp"

#include <vector>
std::vector<std::vector<int>> graph{{1, 2}, {0}, {0}};
sora::HeavyLightDecomposition hld(graph);
int ancestor = hld.lca(1, 2);  // 0
int vertex = hld.jump(1, 2, 1);  // 0
```
{% endraw %}

## 検証

公式問題: `verify/VertexAddPathSum.test.cpp`（Fenwick Treeとの組み合わせ）。各種移動・パス取得は`verify/local/TreeAlgorithms.local.cpp`。
