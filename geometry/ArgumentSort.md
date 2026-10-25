---
title: ArgumentSort
documentation_of: ./ArgumentSort.hpp
---

## 概要

原点を基準として、平面上の点を偏角の昇順に並べ替える。整数座標では三角関数や浮動小数点数を使わず、半平面の分類と外積で比較する。
点は `Geometry2D.hpp` の `sora::Point<T, C>` を使う。回転順に点を処理する場合や、方向ごとに点をまとめる場合に利用できる。

## 公開APIと計算量

| API | 内容 | 時間計算量 | 追加空間 |
| --- | --- | --- | --- |
| `sora::ArgumentLess<T, C>{}(a, b)` | 下記の順序で `a` が `b` より前なら `true` | O(1) | O(1) |
| `sora::argument_sort(points)` | `std::vector<Point<T, C>>` をその場で並べ替える | O(N log(N+1)) | O(log(N+1)) |

`N` は点数。型の算術演算・比較をO(1)とする。`C` の既定値は `DefaultCalculation<T>` で、標準の整数型では `__int128_t`、浮動小数点型では `T` となる。

### 並び順

偏角は `(-π, π]` とする。下半平面から始まり、正のx軸、上半平面、負のx軸の順に反時計回りに並ぶ。
原点の偏角は0とし、正のx軸上の点より前に置く。同じ偏角の点は原点から近い順に並べる。重複点も残すが、安定ソートではない。

比較関数を直接 `std::sort` に渡すこともできる。

## 前提条件・境界ケース

- `T` と `C` は `Point` の条件を満たす符号付き数値型とする。外積、距離の2乗とその中間値が `C` に収まること。
- `T = long long`、`C = __int128_t` なら、各座標の絶対値が `10^18` 以下の範囲で上記の整数演算は収まる。`long long` の全範囲を保証するものではない。
- 空の点列、1点だけの点列、原点、重複点、同一直線上の点を扱える。入力の点列は書き換える。
- 浮動小数点型も指定できるが、丸め誤差の影響を受ける。入力と中間値は有限であり、比較が狭義弱順序を満たす範囲で使うこと。誤差による外積の符号の変化やアンダーフローには対応しない。
- 比較には `eps` を使わない。近い偏角を同一とみなす用途では、ソート後の処理を別途設計する必要がある。
- 別の点を基準にする場合は、その点を原点とする座標に変換してから渡す。座標の差を求める際のオーバーフローにも注意する。

## 使用例

{% raw %}
```cpp
#include "geometry/ArgumentSort.hpp"
#include <cassert>
#include <vector>

int main() {
    using P = sora::Point<long long>;
    std::vector<P> points{{-1, 0}, {2, 0}, {0, 1}, {0, 0}, {0, -1}, {1, 0}};
    sora::argument_sort(points);
    const std::vector<P> expected{{0, -1}, {0, 0}, {1, 0}, {2, 0}, {0, 1}, {-1, 0}};
    assert(points == expected);
}
```
{% endraw %}

## 検証コードと検証範囲

- `verify/SortPointsByArgument.test.cpp`: Library Checkerの [Sort Points by Argument](https://judge.yosupo.jp/problem/sort_points_by_argument) に対する検証コード。整数座標の偏角順を検証する。
- `verify/local/ArgumentSort.local.cpp`: 空列、原点、各軸、重複点、同じ方向・逆方向、大きな整数座標を確認する。小さな整数格子で比較の狭義弱順序を確認し、ランダムな整数点列を `long double` の `atan2` による結果と比較する。浮動小数点型は軸上の基本ケースのみを確認する。

浮動小数点型について、任意の入力で正確な偏角順になることは保証しない。
