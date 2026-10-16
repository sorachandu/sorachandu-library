---
title: "Fraction — 既約分数"
documentation_of: ./Fraction.hpp
---

## 概要

long longの分子・分母で既約分数を表し、四則演算・順序比較・ハッシュを提供します。中間計算には128bit整数を使います。保持空間O(1)。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `Fraction()` / `Fraction(a)` / `Fraction(a, b)` | 未初期化NaN / 整数 / 分数 | O(1) / O(1) / O(log M) |
| `normalize()` | 公開メンバの分子・分母を正規化 | O(log M) |
| `get_pair()` / `get_num()` / `get_den()` / `is_nan()` | 値・状態取得 | O(1) |
| `+`, `-`, `*`, `/`, 単項`-`, `abs()`, `inv()` | 四則演算・符号・絶対値・逆数 | O(log M) |
| 比較演算子 / `FractionHash` | 順序・同値・ハッシュ | O(1) |
| `to_double()` / `convert_1d(value)` | 浮動小数 / num × value + den | O(1) |
| `operator<<` | 分数出力 | O(1)（64bitの桁数を定数とする） |

## 仕様・使い方

Mは正規化の128bit中間値の絶対値の最大です。有限値は分母を正にして約分します。LLONG_MIN、および約分後の分子・分母がlong longに収まらない場合は対象外です。

分母0・未初期化はNaNです。NaNとの演算・0除算はNaNになり、比較では全てのNaNを同値として有限値より後へ置きます。IEEE浮動小数のNaNとは比較規則が異なります。

有限値の公開num・denを直接変更した場合はnormalizeしてから比較・ハッシュを使ってください。convert_1dは計算式を評価するだけで、任意の分数に対する衝突のない符号化を保証しません。結果はlong longに収まる必要があります。

## 使用例

{% raw %}
```cpp
#include "math/Fraction.hpp"

sora::Fraction a(1, 2), b(1, 3);
auto sum = a + b;  // 5/6
bool equal = sora::Fraction(2, 4) == a;  // true
auto invalid = a / sora::Fraction(0);  // NaN
```
{% endraw %}

## 検証

`verify/local/Fraction.local.cpp`で四則演算・NaNの順序・ハッシュ・境界を検証します。
