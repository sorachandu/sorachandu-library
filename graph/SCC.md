---
title: "SCC — 強連結成分分解"
documentation_of: ./SCC.hpp
---

## 概要

有向グラフを非再帰Kosaraju法で強連結成分に分解します。縮約DAG、次数、閉路の有無も同時に求めます。Nは頂点数、Mは追加した辺数、Kは成分数、Lは縮約後の辺数です。グラフ・作業領域・結果はそれぞれ空間O(N + M)以内です。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `SCCGraph(n = 0)` | n頂点で初期化 | O(N) |
| `add_edge(from, to)` | 有向辺を追加 | ならしO(1) |
| `vertex_count()` / `edge_count()` | 頂点数 / 辺数 | O(1) |
| `build(keep_multi_edges = false)` | 独立した結果を返す | O(N + M) |
| `result.count()` / `same(u, v)` / `size(c)` | 成分数 / 同じ成分か / 成分の頂点数 | O(1) |
| `result.reverse_dag()` | 縮約DAGの逆グラフを生成 | O(K + L) |

## 仕様・使い方

頂点番号は[0, N)です。自己ループ・多重辺・空グラフを扱えます。N・Mはintの範囲内とします。`build`は入力を変更せず、辺追加後も以前の結果は変化しません。

結果の`id[v]`は頂点vの成分ID、`groups[c]`は成分cの頂点列です。成分間の辺u→vに対し`id[u] < id[v]`となります。`dag`には成分内部の辺を含めず、既定では多重辺を除去します。`indegree`・`outdegree`はそのDAG上の次数です。

`has_cycle[c]`は成分の頂点数が2以上、または自己ループがあるときtrueです。`representative[c]`は最小の頂点番号です。結果の公開配列は読み取り用として扱ってください。頂点列・隣接列の順序は保証しません。

## 使用例

{% raw %}
```cpp
#include "graph/SCC.hpp"

sora::SCCGraph graph(3);
graph.add_edge(0, 1);
graph.add_edge(1, 0);
graph.add_edge(1, 2);
auto result = graph.build();
bool same = result.same(0, 1);  // true
int components = result.count();  // 2
```
{% endraw %}

## 検証

公式問題用の`verify/SCC.test.cpp`で検証します。`verify/local/AddedLibraries.local.cpp`では、追加で到達可能性・縮約DAG・次数・閉路・多重辺を比較し、20万頂点の鎖を検証します。
