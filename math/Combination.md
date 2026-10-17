---
title: "Combination — 素数法の階乗・二項係数"
documentation_of: ./Combination.hpp
---

## 概要

素数法で階乗・逆階乗を前計算し、二項係数と順列数に定数時間で答えます。Nは前計算上限です。modint演算・コピーをO(1)とし、時間・空間計算量はO(N)です。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `Combination<ModInt>(n)` | nまで前計算 | O(N) |
| `comb(n, k)` | nCk | O(1) |
| `perm(n, k)` | nPk | O(1) |
| `fact(n)` | n! | O(1) |

## 仕様・使い方

ModIntはCombinationValue Conceptを満たし、整数からの構築・staticなmod()・積・差を持つ型です。法はintに収まる素数で、0 <= N < modとします。利用中に法を変更しないでください。ACLに依存しない実装です。

comb・permはn < 0、k < 0、k > nなら0、factはn < 0なら0です。それ以外ではnが前計算上限以下である必要があります。n >= modや合成数の法には対応しません。

## 使用例

{% raw %}
```cpp
#include "math/Combination.hpp"

#include <atcoder/modint>
using Mint = atcoder::modint998244353;
sora::Combination<Mint> combination(100);
auto ways = combination.comb(5, 2);  // 10
auto permutations = combination.perm(5, 2);  // 20
```
{% endraw %}

## 検証

公式問題用の`verify/BinomialCoefficientPrimeMod.test.cpp`で検証します。`verify/local/Combination.local.cpp`で独自modintを使って単純解と比較します。使用例のACLは別途必要です。
