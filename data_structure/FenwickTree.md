---
title: "FenwickTree — 点加算・区間和"
documentation_of: ./FenwickTree.hpp
---

## 概要

Fenwick Treeで配列の点加算と区間和を管理します。Nは配列長、空間O(N)。Tのコピー・加減算をO(1)とします。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `FenwickTree<T>(n)` | 長さnの零配列 | O(N) |
| `add(p, x)` | a[p] += x | O(log N) |
| `sum(r)` | [0, r)の和 | O(log N) |
| `sum(l, r)` | [l, r)の和 | O(log N) |

## 仕様・使い方

Tは`T{}`を零元とし、`+=`と減算を持つ加法可換群です。n >= 0、更新位置は[0, N)、区間は0 <= l <= r <= N。N = 0では空区間の和のみ利用できます。問い合わせは内部状態を変更しません。

## 使用例

{% raw %}
```cpp
#include "data_structure/FenwickTree.hpp"

sora::FenwickTree<long long> tree(5);
tree.add(2, 7);
auto sum = tree.sum(1, 4);  // 7
```
{% endraw %}

## 検証

公式問題: `verify/FenwickTree.test.cpp`。HLDとの組み合わせは`verify/VertexAddPathSum.test.cpp`。
