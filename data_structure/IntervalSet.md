---
title: "IntervalSet — 整数区間の集合"
documentation_of: ./IntervalSet.hpp
---

## 概要

整数の半開区間をstd::setで管理します。重なる区間・隣接する区間を併合し、互いに離れた非空区間として保持します。Mは保持する区間の数で、空間計算量はO(M)です。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `insert(l, r)` | 区間を追加 | O(log(M + 1) + k)、kは併合区間数 |
| `erase(l, r)` | 区間を削除 | O(log(M + 1) + k)、kは交差区間数 |
| `find_interval(x)` | xを含む区間、なければstd::nullopt | O(log(M + 1)) |
| `contains(x)` / `contains(l, r)` | 点 / 区間全体を含むか | O(log(M + 1)) |
| `intersects(l, r)` | 区間と交差するか | O(log(M + 1)) |
| `mex(x = 0)` | x以上の最小の未登録整数 | O(log(M + 1)) |
| `intervals()` / `size()` / `empty()` | 区間集合のconst参照 / 区間数 / 空か | O(1) |
| `clear()` | 全削除 | O(M) |

## 仕様・使い方

端点はlong longです。`Coordinate`はlong long、`Interval`は端点のpair、`ll`は互換用の別名です。`size()`は含まれる整数の個数ではありません。

l >= rの場合、追加・削除は何もせず、`contains(l, r)`はtrue、`intersects(l, r)`はfalseです。閉区間から変換するときのr + 1のオーバーフローは呼び出し側で避けてください。long longの最大値は右端に使えますが、その値自体を含む区間は表現できません。

`intervals()`が返す区間集合は左端の昇順に並んでいます。追加・削除によって、削除された区間への参照・反復子は無効になります。

## 使用例

{% raw %}
```cpp
#include "data_structure/IntervalSet.hpp"

sora::IntervalSet intervals;
intervals.insert(1, 4);
intervals.insert(4, 7);  // [1, 7)に併合
intervals.erase(3, 5);  // [1, 3), [5, 7)
auto interval = intervals.find_interval(2);
long long next = intervals.mex(1);  // 3
```
{% endraw %}

## 検証

`verify/local/AddedLibraries.local.cpp`でstd::setによる単純解、空・逆向き区間、分割・併合、64bit端点を確認します。公式問題用の検証コードはありません。
