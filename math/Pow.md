---
title: "Pow — 整数の二分累乗"
documentation_of: ./Pow.hpp
---

## 概要

long longの整数累乗を二分累乗法で計算します。剰余を取る関数ではありません。時間計算量はO(log(b + 1))、追加空間はO(1)です。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `sora::pow(a, b)` | aのb乗 | O(log(b + 1)) |

## 仕様・使い方

b >= 0とします。計算途中を含めて積がlong longに収まる必要があります。0の0乗は1です。std::powと区別して名前空間を明記すると、整数累乗であることが分かりやすくなります。

## 使用例

{% raw %}
```cpp
#include "math/Pow.hpp"

long long value = sora::pow(3, 5);  // 243
long long identity = sora::pow(0, 0);  // 1
```
{% endraw %}

## 検証

`verify/local/HeaderMigration.local.cpp`で指数0・符号・境界を検証します。
