---
title: "RangeDistinctCount — 区間・値域内の種類数"
documentation_of: ./RangeDistinctCount.hpp
---

## 概要

静的配列の位置区間[l, r)に現れる値のうち、a <= A[i] < bを満たす異なる値の数を返します。外側の値用ウェーブレット行列と内側の直前出現位置用ウェーブレット行列を組み合わせます。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `RangeDistinctCount(a)` | 前計算 | O(N D_VAL D_PRE + U) |
| `query(l, r, a, b)` | 位置区間・値域を指定して種類数を取得 | O(D_VAL D_PRE) |

## 仕様・使い方

現実装はD_VAL = D_PRE = 19で固定です。N < 2^19とし、各入力値は[0, 2^19)に属する必要があります。Uは最大入力値 + 1（空入力では0）です。空間はO(N D_VAL D_PRE + U)以内で、通常の種類数クエリよりもメモリ使用量が大きくなります。

入力は変更しません。問い合わせの引数は0 <= l <= r <= N、a <= bを満たす必要があります。位置区間または値域が空なら0を返します。内部のBitVector・InnerWMはこの実装用の補助構造です。

## 使用例

{% raw %}
```cpp
#include "data_structure/RangeDistinctCount.hpp"

#include <vector>
sora::RangeDistinctCount counter(std::vector<int>{1, 2, 1, 3});
int result = counter.query(0, 4, 1, 3);  // 1と2の2種類
```
{% endraw %}

## 検証

`examples/RangeDistinctCount.cpp`に手動提出用の使用例があります。対応する公式検証・専用ローカルテストはありません。
