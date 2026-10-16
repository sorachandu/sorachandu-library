---
title: "LowLink — 関節点・橋の列挙"
documentation_of: ./LowLink.hpp
---

## 概要

無向単純グラフをDFSして、関節点と橋を列挙します。非連結グラフも扱います。Vは頂点数、Eは辺数、時間O(V + E)、入力を除く空間O(V)。

## API・計算量

| 操作・結果 | 内容 | 時間 |
| --- | --- | --- |
| `LowLink(g)` | 全成分を探索 | O(V + E) |
| `aps` | 関節点の頂点番号列 | 参照O(1) |
| `bridges` | 橋の端点pair列 | 参照O(1) |
| `ord` / `low` | DFS順 / 到達できる最小のDFS順 | 参照O(1) |

## 仕様・使い方

入力は両方向を格納した無向単純グラフです。多重辺には対応しません。各橋の端点は小さい番号が先ですが、結果の列全体はソートしません。

入力を参照として保持するため、グラフはLowLinkオブジェクトより長く生存する必要があります。一時オブジェクトを渡さず、構築後に元グラフを変更しないでください。DFSは再帰で、深さは最悪O(V)です。

## 使用例

{% raw %}
```cpp
#include "graph/LowLink.hpp"

#include <vector>
std::vector<std::vector<int>> graph{{1}, {0, 2}, {1}};
sora::LowLink low_link(graph);
// aps: 1、bridges: (0, 1)と(1, 2)（列の順序は未規定）
```
{% endraw %}

## 検証

`verify/local/HeaderMigration.local.cpp`で橋・関節点を検証します。
