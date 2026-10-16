---
title: "DSU — 併合方向を固定するUnion-Find"
documentation_of: ./DSU.hpp
---

## 概要

経路圧縮を行い、常にu側の根へv側の根をつなぎます。Union by Size/Rankを使わず、代表元の併合方向を指定したい場合の構造です。空間O(N)。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `DSU()` / `DSU(n)` | 空 / N頂点で初期化 | O(1) / O(N) |
| `root(v)` | 代表元 | 最悪O(N) |
| `merge(u, v)` | u側へv側を併合 | 最悪O(N) |
| `same(u, v)` | 同じ成分か | 最悪O(N) |

## 仕様・使い方

頂点番号は[0, N)。`merge(u, v)`後の代表元は併合前の`root(u)`です。`root`・`same`も経路圧縮により内部状態を変更します。一般的なランク付きUnion-Findと同じ償却O(α(N))を前提にしないでください。論理代表元だけ選びたい場合はDSUChooseが利用できます。

## 使用例

{% raw %}
```cpp
#include "data_structure/DSU.hpp"

sora::DSU dsu(3);
dsu.merge(1, 2);
int root = dsu.root(2);  // 1
```
{% endraw %}

## 検証

`verify/local/DSUAndFolds.local.cpp`で単純解比較・20万頂点の鎖を検証します。
