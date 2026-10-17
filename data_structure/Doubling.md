---
title: "Doubling — 遷移関数の繰り返し"
documentation_of: ./Doubling.hpp
---

## 概要

[0, N)上の遷移関数を二分累乗で適用します。単射・全単射である必要はありません。最大遷移回数をK、L = max(1, bit_width(K))とすると、空間計算量はO(NL)です。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `Doubling<T>()` | 空で初期化 | O(1) |
| `Doubling<T>(next, k = 1ULL << 60)` | k回までの遷移を前計算 | O(NL) |
| `query(p, k)` | pからk回遷移した先 | O(L) |

## 仕様・使い方

Tは整数の添字型です。nextの全要素は[0, N)に属する必要があります。入力は変更しません。queryに指定する遷移回数は0 <= k <= Kで、0回ならpを返します。空の構造にはqueryできません。遷移途中の値の集計は提供しません。

## 使用例

{% raw %}
```cpp
#include "data_structure/Doubling.hpp"

#include <vector>
sora::Doubling<int> doubling(std::vector<int>{1, 2, 0}, 100);
int destination = doubling.query(0, 5);  // 2
```
{% endraw %}

## 検証

`verify/local/HeaderMigration.local.cpp`で単純な繰り返しと比較します。
