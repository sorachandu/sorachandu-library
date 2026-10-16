---
title: "Eratosthenes — 素数の篩"
documentation_of: ./Eratosthenes.hpp
---

## 概要

エラトステネスの篩で0以上n以下の素数を列挙します。時間O(n log log(n + 2))、空間O(n)。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `eratosthenes(n)` | 長さn + 1の素数判定配列 | O(n log log(n + 2)) |
| `get_primes(n)` | n以下の素数の昇順列 | O(n log log(n + 2)) |

## 仕様・使い方

n >= 0。返す判定配列の0・1はfalseです。n = 0・1でも利用できます。get_primesは内部で篩を構築するため、既存の篩を再利用する関数ではありません。nに比例する配列を確保できる範囲で使ってください。

## 使用例

{% raw %}
```cpp
#include "math/Eratosthenes.hpp"

auto sieve = sora::eratosthenes(10);
bool seven_is_prime = sieve[7];  // true
auto primes = sora::get_primes(10);  // 2, 3, 5, 7
```
{% endraw %}

## 検証

`verify/local/HeaderMigration.local.cpp`で小さい値と境界を検証します。
