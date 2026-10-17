---
title: "Mo — オフライン区間クエリ"
documentation_of: ./Mo.hpp
---

## 概要

静的な配列への区間クエリを並べ替え、区間端点の移動回数を抑えます。Nは配列長、Qはクエリ数、αは端点1回の更新時間です。保持空間はO(Q)です。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `Mo(n)` | 配列長を登録 | O(1) |
| `add(l, r)` | 半開区間のクエリを登録 | ならしO(1) |
| `build(add_left, add_right, del_left, del_right, rem)` | 左右別の更新で処理 | O(Q log(Q + 1) + α(N√Q + N)) |
| `build(add, del, rem)` | 左右共通の更新で処理 | 同上 |

## 仕様・使い方

各クエリは0 <= l <= r <= Nを満たす必要があります。区間は[0, 0)から開始し、更新コールバックには追加・削除する配列の添字を渡します。`rem(i)`には登録順のクエリ番号を渡すので、そこで答えを保存します。結果保存1回をO(1)として計算量を評価しています。

配列の更新を伴うクエリには対応しません。buildを再実行する場合は、外部の集計状態を空区間へ戻してください。Q = 0ならコールバックを呼びません。

## 使用例

{% raw %}
```cpp
#include "utility/Mo.hpp"

#include <vector>
std::vector<int> a{1, 2, 3};
std::vector<long long> answers(1);
sora::Mo mo(a.size());
mo.add(0, 2);
long long sum = 0;
mo.build([&](int i) { sum += a[i]; },
         [&](int i) { sum -= a[i]; },
         [&](int q) { answers[q] = sum; });  // 3
```
{% endraw %}

## 検証

`examples/Mo.cpp`に手動提出用の使用例があります。対応する公式検証・専用ローカルテストはありません。
