---
title: "Monoid — モノイドのConcept"
documentation_of: ./Monoid.hpp
---

## 概要

Folds・DisjointSparseTableで使うモノイドの構文を検査します。実行時の処理・保持空間はありません。

## API・計算量

| 要素 | 意味 | 条件 |
| --- | --- | --- |
| `M::T` | 値の型 | 利用する構造で必要なコピー等が可能 |
| `M::op(a, b)` | 二項演算 | M::Tに変換可能 |
| `M::e()` | 単位元 | M::Tに変換可能 |

## 仕様・使い方

結合則と左右の単位元の法則は利用者が保証します。可換性・冪等性・逆元は要求しません。各データ構造の計算量は演算・コピーをO(1)として評価しています。たとえば可変長文字列の連結では、そのコストを別途考慮してください。

## 使用例

{% raw %}
```cpp
#include "math/Monoid.hpp"

struct Sum {
    using T = long long;
    static T op(T a, T b) { return a + b; }
    static T e() { return 0; }
};
static_assert(sora::Monoid<Sum>);
```
{% endraw %}

## 検証

Folds・DisjointSparseTableを利用する検証で使用します。Conceptの成立だけでは数学的な法則の検証にはなりません。
