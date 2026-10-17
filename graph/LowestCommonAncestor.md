---
title: "LowestCommonAncestor — 二分累乗によるLCA"
documentation_of: ./LowestCommonAncestor.hpp
---

## 概要

無向木の祖先を二分累乗で前計算し、LCA・距離・パス上の位置関係を処理します。Nは頂点数で、空間計算量はO(N log(N + 1))です。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `LowestCommonAncestor(n)` | 頂点・表を確保 | O(N log(N + 1)) |
| `LowestCommonAncestor(g, root = 0)` | 木から構築 | O(N log N) |
| `add_edge(u, v, edge_id)` | 無向辺を追加 | ならしO(1) |
| `build(root = 0)` | 前計算・再構築 | O(N log N) |
| `get_parent(v)` / `get_depth(v)` / `get_parent_edge_id(v)` | 親 / 深さ / 親への辺ID | O(1) |
| `lca(u, v)` / `get_distance(u, v)` | LCA / 辺数で測る距離 | O(log N) |
| `is_on_path(u, v, p)` | pがu→v上にあるか | O(log N) |
| `closest_on_path(u, v, w)` | u→v上でwに最も近い頂点 | O(log N) |

## 仕様・使い方

buildの実行時にはN >= 1の連結な無向木が必要です。頂点番号は[0, N)、辺IDは[0, N − 1)です。辺を追加する方式では問い合わせの前にbuildしてください。根を変えて再構築できます。隣接リストからの構築は入力を変更しません。

根の親・親辺IDは−1、深さは0です。隣接リストから構築した場合、辺IDは内部で割り当てます。自前のIDが必要ならadd_edgeを使います。closest_on_pathは端点も候補に含みます。距離の重み付けは提供しません。

## 使用例

{% raw %}
```cpp
#include "graph/LowestCommonAncestor.hpp"

sora::LowestCommonAncestor tree(3);
tree.add_edge(0, 1, 0);
tree.add_edge(0, 2, 1);
tree.build();
int ancestor = tree.lca(1, 2);  // 0
int distance = tree.get_distance(1, 2);  // 2
```
{% endraw %}

## 検証

`verify/local/HeaderMigration.local.cpp`と`verify/local/TreeAlgorithms.local.cpp`でLCA・距離・パス関係を単純解と比較します。
