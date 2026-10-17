---
title: "DisjointSparseTable — 静的区間積"
documentation_of: ./DisjointSparseTable.hpp
---

## 概要

結合則を持つ演算の区間積を定数時間で求めます。Nは入力長です。演算・コピーをO(1)とし、前処理の時間・空間計算量はO((N + 1) log(N + 2))です。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `DisjointSparseTable<M>(a)` | 配列から前計算 | O((N + 1) log(N + 2)) |
| `get(p)` | 元の要素 | O(1) |
| `prod(l, r)` | [l, r)の積 | O(1) |

## 仕様・使い方

MはMonoid Conceptを満たす型です。入力は変更しません。更新には非対応で、可換性・冪等性は不要です。区間は0 <= l <= r <= Nを満たす必要があります。空区間では単位元を返します。累積和で扱える群ならより省メモリな構造も検討できます。

## 使用例

{% raw %}
```cpp
#include "data_structure/DisjointSparseTable.hpp"

#include <vector>
struct Sum {
    using T = long long;
    static T op(T a, T b) { return a + b; }
    static T e() { return 0; }
};
sora::DisjointSparseTable<Sum> table(std::vector<long long>{1, 2, 3});
auto result = table.prod(1, 3);  // 5
```
{% endraw %}

## 検証

最小値は公式問題用の`verify/DisjointSparseTable.test.cpp`で、非可換演算はローカルテストの`verify/local/DSUAndFolds.local.cpp`で検証します。
