---
title: "LazyMonoid — 作用付きモノイドのConcept"
documentation_of: ./LazyMonoid.hpp
---

## 概要・用途

`LazySegmentTree`で使う作用付きモノイドの構文を検査します。既存の `Monoid` Conceptに、作用素の型、値への作用、作用素の合成、恒等作用素を加えたものです。実行時の処理や保持領域はありません。

## 公開API・計算量

| 要素 | 意味 | 戻り値の条件 |
| --- | --- | --- |
| `M::T` | 集約値の型 | `Monoid<M>`の要件を満たす |
| `M::op(x, y)` | 左から右への区間積 | Tに変換可能 |
| `M::e()` | 区間積の単位元 | Tに変換可能 |
| `M::F` | 作用素の型 | 利用する構造で必要なコピー・代入などが可能 |
| `M::mapping(f, x)` | 集約値xに作用素fを適用 | Tに変換可能 |
| `M::composition(f, g)` | gの後にfを適用する合成 | Fに変換可能 |
| `M::id()` | 恒等作用素 | Fに変換可能 |

Concept自体に実行時の計算量はありません。`LazySegmentTree`の計算量は上記の各演算と値・作用素のコピー・代入をO(1)として評価します。

## 前提条件・法則

結合則、単位元、以下の作用の法則は利用者が保証します。Conceptは数学的な法則を検査しません。

- `op`は結合的で、`e()`は左右の単位元です。
- `composition`は結合的で、`id()`は左右の単位元です。集約値の演算・作用素の合成はどちらも非可換で構いません。
- `mapping(id(), x) == x`。
- `mapping(composition(f, g), x) == mapping(f, mapping(g, x))`。
- `mapping(f, op(x, y)) == op(mapping(f, x), mapping(f, y))`。

ここでの `==` は値の意味が一致することを表します。型T・Fの等値比較演算子は要求しません。関数は同じ引数に同じ結果を返し、渡された値を変更しないものとします。

合成順序と引数の並びは [AtCoder Libraryのlazysegtree](https://atcoder.github.io/ac-library/master/document_ja/lazysegtree.html) に合わせています。例えば `composition(加算3, 代入5)` は代入5の後で3を加える作用です。

## 使用例

{% raw %}
```cpp
#include "math/LazyMonoid.hpp"

// 区間加算・区間和の作用付きモノイド。
struct RangeAddSum {
    // 区間和と要素数。
    struct T { long long sum, length; };
    using F = long long;
    static T op(T x, T y) { return {x.sum + y.sum, x.length + y.length}; }
    static T e() { return {0, 0}; }
    static T mapping(F f, T x) { return {x.sum + f * x.length, x.length}; }
    static F composition(F f, F g) { return f + g; }
    static F id() { return 0; }
};
static_assert(sora::LazyMonoid<RangeAddSum>);
```
{% endraw %}

区間長が必要な演算では、例のようにTへ区間長を持たせます。木から `mapping` へ区間長を別引数で渡すAPIにはしていません。整数の演算結果が型の範囲に収まることも利用者が保証してください。

## 検証コード・検証範囲

`verify/RangeAffineRangeSum.test.cpp`で、非可換な一次関数の合成を `LazySegmentTree`に載せて検証します。`verify/local/LazySegmentTree.local.cpp`では、有効・無効な型のConcept判定と、デフォルト構築・等値比較を要求しない型の利用を確認します。一般の型に対する数学的な法則の成立を保証する検証ではありません。
