---
title: "RerootingDP — 全方位木DP"
documentation_of: ./RerootingDP.hpp
---

## 概要

全頂点を根とした木全体のDP値を前計算し、根を指定した部分木DPにも答えます。Nは頂点数です。演算・コピーをO(1)とし、前処理の時間・空間計算量はO(N)です。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `rerootingdp<V, E>(op, e, put_edge, put_vertex, g, root = 0)` | クエリオブジェクトを生成 | O(N) |
| `dp(v)` | vを根とした木全体のDP値 | O(1) |
| `dp(r, v)` | rを根としたときのvの部分木DP値 | O(log N) |
| `dp.is_in_subtree(r, v)` | 前処理の根でvがrの真の子孫か | O(1) |

## 仕様・使い方

Eは隣接部分木からの寄与、Vは頂点で確定した値です。opとeにはE上の可換モノイドの演算と単位元を渡します。put_vertexとput_edgeには、それぞれ`put_vertex(E, 頂点番号) -> V`、`put_edge(V, 辺ID) -> E`の関数を渡します。V・Eはデフォルト構築・コピー可能である必要があります。

g[v]に(c, eid)を入れる場合、g[c]には(v, ~eid)を入れます。vがc側の寄与を取り込むときはeidをput_edgeへ渡します。方向ごとに意味のある辺情報がある場合は、この呼び出し方向に合わせて扱ってください。

入力は空でない連結な無向木で、変更しません。rootは前処理だけに使う根です。再帰深さは最悪O(N)です。公開配列ans[v]は全体DP値で、from・toの前処理の根に対応する要素は未定義です。通常はoperator()で取得してください。

## 使用例

{% raw %}
```cpp
#include "graph/RerootingDP.hpp"

#include <utility>
#include <vector>
std::vector<std::vector<std::pair<int, int>>> graph(2);
graph[0].emplace_back(1, 0);
graph[1].emplace_back(0, ~0);
auto dp = sora::rerootingdp<int, int>(
    [](int a, int b) { return a + b; }, 0,
    [](int value, int) { return value; },
    [](int sum, int) { return sum + 1; }, graph);
int whole = dp(0);  // 2
int subtree = dp(0, 1);  // 1
```
{% endraw %}

## 検証

`verify/local/TreeAlgorithms.local.cpp`で全ての根・部分木に対するDP値を単純解と比較します。
