---
title: "RangeRandom — 範囲付き整数乱数"
documentation_of: ./RangeRandom.hpp
---

## 概要

std::mt19937とstd::uniform_int_distributionで指定範囲の整数を生成します。空間O(1)（エンジン状態の固定サイズ）。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `RangeRandom()` / `RangeRandom(l, r)` | 既定範囲 / 閉区間[l, r]で初期化 | 乱数源に依存 |
| `get_range()` / `set_range(l, r)` | 範囲取得 / 変更 | O(1) |
| `gen()` / `operator()()` | 整数を1個生成 | 期待O(1) |

## 仕様・使い方

範囲は半開区間ではなく閉区間で、int64_tのl <= rです。既定範囲は[0, INT64_MAX]。std::random_deviceで初期化するため、同じ乱数列を再現するためのシード指定APIはありません。範囲変更後もエンジンの状態は引き継ぎます。

## 使用例

{% raw %}
```cpp
#include "utility/RangeRandom.hpp"

sora::RangeRandom random(1, 6);
auto die = random();  // 1〜6
random.set_range(-10, 10);
auto value = random.gen();
```
{% endraw %}

## 検証

`verify/local/HeaderMigration.local.cpp`で生成範囲を検証します。一様性・乱数の品質を統計的に保証するテストではありません。
