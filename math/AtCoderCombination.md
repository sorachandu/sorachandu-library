---
title: "AtCoderCombination — ACL modint用の別名"
documentation_of: ./AtCoderCombination.hpp
---

## 概要

CombinationをACLのstatic_modint・dynamic_modintに限定した別名です。計算の実装・仕様はCombinationと共通です。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `AtCoderCombination<ModInt>(n)` | nまで前計算 | O(N) |
| `comb(n, k)` / `perm(n, k)` / `fact(n)` | 二項係数 / 順列数 / 階乗 | O(1) |

## 仕様・使い方

外部のAtCoder Libraryへのincludeパスが必要です。法はintに収まる素数で、0 <= N < modとします。動的modintは構築前に法を設定し、利用中に変更しないでください。保持空間はO(N)です。範囲外引数の扱いはCombinationと同じです。

## 使用例

{% raw %}
```cpp
#include "math/AtCoderCombination.hpp"

using Mint = atcoder::modint998244353;
sora::AtCoderCombination<Mint> combination(100);
auto ways = combination.comb(5, 2);  // 10
```
{% endraw %}

## 検証

`verify/local/AtCoderCombination.local.cpp`でACLの静的・動的modintを検証します。実行には`python3 verify/RunLocalTests.py --acl /path/to/ac-library`が必要で、通常のローカル検証ではスキップします。
