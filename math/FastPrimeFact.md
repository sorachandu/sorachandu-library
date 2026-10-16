---
title: "FastPrimeFact — 64bit素数判定・素因数分解"
documentation_of: ./FastPrimeFact.hpp
---

## 概要

signed 64bit整数をMiller–Rabin法で素数判定し、Pollardのロー法で素因数分解します。剰余積には128bit整数を使います。

## API・計算量

| 操作 | 内容 | 時間・空間 |
| --- | --- | --- |
| `is_prime(n)` | 素数かを返す | 時間O(log n)、空間O(1)（n < 2はO(1)） |
| `prime_factorize(n)` | 素因数の昇順列 | 探索時間に依存・最悪時間保証なし、空間O(log n) |

## 仕様・使い方

is_primeはn < 2にfalseを返し、固定の判定基底でsigned 64bit範囲を扱います。prime_factorizeはn >= 1が前提で、1の結果は空です。素因数は指数をまとめず、重複して列挙します。

素因数分解の探索は入力によって所要時間が変わります。多数の小さい整数をまとめて処理するなら、篩を使う方法も検討してください。

## 使用例

{% raw %}
```cpp
#include "math/FastPrimeFact.hpp"

bool prime = sora::is_prime(998244353);  // true
auto factors = sora::prime_factorize(60);  // 2, 2, 3, 5
```
{% endraw %}

## 検証

公式問題: `verify/PrimalityTest.test.cpp`（素数判定のみ）。素因数分解・64bit境界付近は`verify/local/FastPrimeFact.local.cpp`。
