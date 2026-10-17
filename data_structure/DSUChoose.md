---
title: "DSUChoose — 論理代表元を選べるUnion-Find"
documentation_of: ./DSUChoose.hpp
---

## 概要

ランクと経路圧縮による構造上の根と、利用者が選ぶ論理代表元を分けて管理します。Nは頂点数で、空間計算量はO(N)です。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `DSUChoose()` / `DSUChoose(n)` | 空 / N頂点で初期化 | O(1) / O(N) |
| `par_root(v)` | 構造上の根 | 償却O(α(N)) |
| `rpr_root(v)` | 論理代表元 | 償却O(α(N)) |
| `merge(u, v, choose)` | 代表元をchooseで選んで併合 | 償却O(α(N) + C) |
| `same(u, v)` | 同じ成分か | 償却O(α(N)) |

## 仕様・使い方

chooseには併合前のu側・v側の論理代表元をこの順で渡します。chooseが返す頂点は併合後の成分に属する必要があります。同じ成分ならchooseは呼びません。Cはchooseの実行時間です。構造上の根はランクに依存するため、選んだ代表元の取得には`rpr_root`を使います。

## 使用例

{% raw %}
```cpp
#include "data_structure/DSUChoose.hpp"

#include <algorithm>
sora::DSUChoose dsu(4);
dsu.merge(3, 1, [](int u, int v) { return std::min(u, v); });
int representative = dsu.rpr_root(3);  // 1
```
{% endraw %}

## 検証

`verify/local/DSUAndFolds.local.cpp`で単純解との比較と代表元選択の検証を行います。
