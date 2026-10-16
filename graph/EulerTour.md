---
title: "EulerTour — 木の入退場時刻"
documentation_of: ./EulerTour.hpp
---

## 概要

無向木を非再帰DFSでたどり、入退場時刻と往復の頂点列を求めます。Nは頂点数、時間・出力を含む追加空間O(N)。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `euler_tour(g, in, out, idxs, root = 0)` | 入退場時刻・往復列を出力 | O(N) |

## 仕様・使い方

入力は連結な無向木または空グラフ。入力gは変更せず、in・out・idxsを上書きします。出力先3配列は別オブジェクトである必要があります。非空ならrootは[0, N)。

時刻は0から2N − 1で、入場時にin[v]、退場時にout[v]を記録します。idxsには入場時v、退場時−vが入ります。頂点0の入退場は符号では区別できないので、必要なら時刻を使ってください。部分木を頂点ごとに1回数えるHLDの連続区間とは異なる表現です。

## 使用例

{% raw %}
```cpp
#include "graph/EulerTour.hpp"

#include <vector>
std::vector<std::vector<int>> graph{{1}, {0, 2}, {1}};
std::vector<int> in, out, order;
sora::euler_tour(graph, in, out, order);
// order: 0, 1, 2, -2, -1, 0
```
{% endraw %}

## 検証

`verify/local/EulerTour.local.cpp`で空・単頂点・鎖と入退場時刻を検証します。
