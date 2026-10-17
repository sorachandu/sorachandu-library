---
title: "Folds — 左右累積積"
documentation_of: ./Folds.hpp
---

## 概要

モノイドの左右累積積を保存し、接頭辞・接尾辞・区間を除いた積を取得します。Nは入力長です。演算・コピーをO(1)とし、空間計算量はO(N)です。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `Folds<M>()` / `Folds<M>(a)` | 空 / 配列から構築 | O(1) / O(N) |
| `fold_left(r)` | [0, r)の積 | O(1) |
| `fold_right(l)` | [l, N)の積 | O(1) |
| `fold_without_range(l, r)` | [l, r)を除いた積 | O(1) |
| `fold_without1(i)` | 要素iを除いた積 | O(1) |
| `fold_without_range_one_indexed(l, r)` | 1-indexed閉区間[l, r]を除いた積 | O(1) |
| `fold_without1_one_indexed(i)` | 1-indexedの要素iを除いた積 | O(1) |
| `size()` | 入力長 | O(1) |

## 仕様・使い方

MはMonoid Conceptを満たす型です。入力は変更せず、更新には対応しません。可換性・逆元は不要で、除外後も元の順序で演算します。空の積は単位元です。通常の区間は0 <= l <= r <= Nを満たし、要素の添字は[0, N)に属する必要があります。`one_indexed`の付く互換用APIだけ添字・区間形式が異なります。

## 使用例

{% raw %}
```cpp
#include "data_structure/Folds.hpp"

#include <vector>
struct Sum {
    using T = long long;
    static T op(T a, T b) { return a + b; }
    static T e() { return 0; }
};
sora::Folds<Sum> folds(std::vector<long long>{1, 2, 3, 4});
auto result = folds.fold_without_range(1, 3);  // 1 + 4 = 5
```
{% endraw %}

## 検証

`verify/local/DSUAndFolds.local.cpp`で非可換モノイド・空入力を含めて単純解と比較します。
