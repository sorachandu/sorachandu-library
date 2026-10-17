---
title: "DynamicPointAddRectangleSum — オンライン点加算・長方形和"
documentation_of: ./DynamicPointAddRectangleSum.hpp
---

## 概要

座標の事前登録なしで点への加算と長方形和を処理します。サイズが2の冪である静的重み付きウェーブレット行列を保持し、二進カウンタの繰り上がりと同様にマージします。Mは最後のclear以降のadd回数で、空間計算量はO(M log(M + 2))です。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `DynamicPointAddRectangleSum<Coord, T>()` | 空で初期化 | O(1) |
| `add(x, y, delta)` | 点への加算 | ならしO(log²(M + 2)) |
| `rectangle_sum(l, d, r, u)` | [l, r) × [d, u)の和 | O(log²(M + 2)) |
| `size()` / `empty()` | add回数 / addがないか | O(1) |
| `clear()` | 全削除 | O(M log(M + 2)) |

## 仕様・使い方

Coord・Tの既定はlong longです。Coordはコピー可能な全順序型、Tはコピー・デフォルト構築可能で、`T{}`を零元とする加法可換群であることが必要です。`+=`と減算を使います。Conceptは構文を検査し、代数的な法則は利用者が保証します。コピー・比較・群演算をO(1)として計算量を評価します。

座標の重複や負の加算量を許します。空・逆向きの長方形は零元を返します。問い合わせは点集合を変更しません。整数の重みは途中の累積値も型の範囲に収まる必要があります。

追加1回の最悪時間はO((M + 1) log(M + 2))です。小さなブロックをまとめて再構築するため、更新時間が毎回一定ではありません。各ブロックの点数はintに収まる必要があります。

## 使用例

{% raw %}
```cpp
#include "data_structure/DynamicPointAddRectangleSum.hpp"

sora::DynamicPointAddRectangleSum<> points;
points.add(2, 3, 5);
points.add(2, 3, -1);
points.add(8, 9, 7);
auto sum = points.rectangle_sum(0, 0, 5, 5);  // 4
```
{% endraw %}

## 検証

公式問題用の検証コードは`verify/DynamicPointAddRectangleSum.test.cpp`です。`verify/local/AddedLibraries.local.cpp`では、負の加算・重複座標・空領域・clearを含め、全点走査とのランダム比較を行います。
