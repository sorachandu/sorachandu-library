---
title: "Matrix — 行列演算"
documentation_of: ./Matrix.hpp
---

## 概要

二次元配列を保持し、加減乗算・非負整数乗・行列式を計算します。Tの演算・コピーをO(1)として評価します。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `Matrix<T>()` / `Matrix<T>(n, m)` / `Matrix<T>(n)` | 空 / n×m / n×nの零行列 | O(1) / O(NM) / O(N²) |
| `Matrix<T>(v)` | 長方形配列をコピー | O(NM) |
| `height()` / `width()` / `operator[](i)` | 寸法 / 行参照 | O(1) |
| `identity(n)` | 単位行列 | O(N²) |
| `+`, `-`, `+=`, `-=` | 同じ寸法の加減算 | O(NM) |
| `*`, `*=` | N×PとP×Mの積 | O(NPM) |
| `^`, `^=` | N次正方行列のk乗 | O(N³ log(k + 1)) |
| `determinant()` | 行列式 | O(N³) |
| `operator<<` | 行列出力 | O(NM) |

## 仕様・使い方

空行列は0×0で、高さ0・正の幅の行列は表現できません。配列からの構築は入力を変更しません。公開メンバaと行参照で要素を更新できますが、行の長さを不揃いにしないでください。

複合代入は左辺を変更し、通常の演算子は新しい行列を返します。積は内側の寸法が一致する必要があります。累乗は正方行列に対してk >= 0で使います。行列式は体上の除算を要求し、整数除算では正しく計算できません。

保持空間はO(NM)です。加減算の複合代入は追加空間O(1)、通常の加減算は追加空間O(NM)です。乗算は結果用O(NM)に加え、通常の*では左辺のコピーO(NP)も必要です。累乗・行列式は追加空間O(N²)です。

## 使用例

{% raw %}
```cpp
#include "math/Matrix.hpp"

sora::Matrix<long long> matrix(2);
matrix[0][0] = 1;
matrix[0][1] = 1;
matrix[1][0] = 1;
auto powered = matrix ^ 5;
// powered[0][1] == 5
```
{% endraw %}

## 検証

`verify/local/HeaderMigration.local.cpp`で空・非正方行列の積、累乗等を検証します。
