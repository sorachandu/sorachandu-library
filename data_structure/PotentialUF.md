---
title: "PotentialUF — ポテンシャル付きUnion-Find"
documentation_of: ./PotentialUF.hpp
---

## 概要

頂点間のポテンシャル差を管理するUnion-Findです。ランクと経路圧縮を使用します。Nは頂点数、空間O(N)、群演算・コピーはO(1)とします。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `PotentialUF<Abel>(n = 1, sum_unity = 0)` | N頂点を初期化 | O(N) |
| `root(x)` / `same(x, y)` | 根 / 連結判定 | 償却O(α(N)) |
| `potential(x)` | 根からのポテンシャル差 | 償却O(α(N)) |
| `merge(x, y, p)` | potential(y) − potential(x) = pを課す | 償却O(α(N)) |
| `diff(x, y)` | potential(y) − potential(x) | 償却O(α(N)) |
| `groups()` | 連結成分を列挙 | O(N α(N)) |

## 仕様・使い方

Abelは加法可換群として加減算・符号反転ができる型です。sum_unityには零元を渡します。頂点番号は[0, N)。`diff`は同じ成分の頂点にのみ呼べます。

`merge`は異なる成分を併合したときtrue、既に同じ成分なら制約が矛盾するかに関係なくfalseです。矛盾の判定は呼び出し側で、同じ成分なら`diff(x, y) == p`を調べてください。問い合わせでも経路圧縮で内部状態を変更します。

## 使用例

{% raw %}
```cpp
#include "data_structure/PotentialUF.hpp"

sora::PotentialUF<long long> uf(3);
uf.merge(0, 1, 4);
uf.merge(1, 2, -2);
auto difference = uf.diff(0, 2);  // 2
```
{% endraw %}

## 検証

`verify/local/HeaderMigration.local.cpp`で差分制約を検証します。
