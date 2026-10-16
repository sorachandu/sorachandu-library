---
title: "Geometry — 平面幾何の基本型（旧実装）"
documentation_of: ./Geometry.hpp
---

## 概要

doubleによる点・ベクトル・線分・円と基本演算をまとめた旧実装です。比較規則が統一されていないため、用途に合わせて仕様を確認してください。各基本演算の時間・追加空間はO(1)。

## API・計算量

| 型・操作 | 内容 | 時間 |
| --- | --- | --- |
| `Point(x = 0, y = 0)` / `Vector` | 点・ベクトル、四則演算 | O(1) |
| `Point::abs()` / `norm()` | 長さ / 長さの二乗 | O(1) |
| `Segment` / `Line` | p1・p2の2点による表現 | O(1) |
| `Circle(c, r)` / `Polygon` | 円 / 点のvector | 円はO(1)、多角形の保持空間O(N) |
| `dot(a, b)` / `cross(a, b)` | 内積 / 外積 | O(1) |
| `ccw(p0, p1, p2)` | 方向・直線上の位置関係 | O(1) |
| `intersect(...)` | 線分交差判定 | O(1) |
| `get_distance_sto_p(segment, point)` | 旧仕様の点・線分距離 | O(1) |

## 仕様・使い方

eps = 1e-10。点の順序比較は厳密な辞書順、等値比較は座標ごとの誤差比較です。等値比較は順序比較と同じ同値関係にはなりません。ccwは反時計回り1、時計回り−1、後方2、前方−2、線分上0を返します。

get_distance_sto_pは射影が線分内なら符号付きの外積を長さで割った値を返すため、通常の非負距離とは異なります。長さ0の線分では0除算が生じます。この挙動は本整備では変更していません。

## 使用例

{% raw %}
```cpp
#include "geometry/Geometry.hpp"

sora::Point a(0, 0), b(1, 0), c(0, 1);
auto orientation = sora::ccw(a, b, c);  // counter_clockwise
sora::Vector u(1, 2), v(3, 4);
auto product = sora::dot(u, v);  // 11
```
{% endraw %}

## 検証

対応する公式検証・専用ローカルテストはありません。未完成の旧実装として扱います。
