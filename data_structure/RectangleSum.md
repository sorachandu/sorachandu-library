---
title: "RectangleSum — 静的な重み付き点集合の長方形和"
documentation_of: ./RectangleSum.hpp
---

## 概要・用途

構築時に与えた重み付き点集合に対して、半開長方形 `[l, r) × [d, u)` 内の重みの和を求めます。点集合は固定で、構築後の点追加・重み更新には対応しません。問い合わせは前の結果に応じて決めてもよく、事前登録は不要です。

`RectangleSum<Coord, T>` のCoordは座標型、Tは重みの型で、どちらも既定はlong longです。既存の `DynamicPointAddRectangleSum` と同じ型指定・問い合わせ引数の順序を使います。点の重複と負の重みを許します。

## 公開API・計算量

Nを入力点数とします。座標の比較・コピー、重みのコピー・加減算をO(1)として評価します。

| API | 内容 | 時間 |
| --- | --- | --- |
| `RectangleSum<Coord, T>()` | 空の点集合を構築 | O(1) |
| `RectangleSum<Coord, T>(std::vector<Point> points)` | 全点から構築 | O(N log N) |
| `rectangle_sum(l, d, r, u)` | `[l, r) × [d, u)` 内の重みの和をTで返す | O(log N) |
| `size()` / `empty()` | 入力点数 / 空かどうか | O(1) |

`Point` は公開された集成体で、`Coord x, y; T weight;` を持ちます。`size()` は重複を含む入力点数をintで返します。問い合わせ・サイズ取得はconstです。

保持空間はO(N log N)、構築時の追加作業空間はO(N)です。空でない点集合では、y座標の種類数をσとすると、重みの累積和を `ceil(log2 σ)` 段で保持します。全てのy座標が同じなら段数は0で、段ごとのデータは保持しません。

## 前提条件・境界ケース

- `N <= INT_MAX` とします。空集合も構築できます。
- Coordはコピー可能な全順序型です。比較に矛盾のない値を使ってください。座標に対する加減算は行いません。
- Tはコピー・デフォルト構築可能で、`T{}`を零元とする加法可換群です。`+=`と減算を使います。Conceptは構文を検査し、結合則・可換性・逆元などの法則は利用者が保証します。
- 整数の重みを使う場合は、各段の累積和や差を含む途中結果も型の範囲に収まる必要があります。
- 同じ座標の点を複数渡すと、その重みを全て加えます。重み0の点も入力点数に含まれます。
- 問い合わせの左端・下端は含み、右端・上端は含みません。登録されていない座標も端点に使えます。
- 空集合、空長方形、逆向きの長方形（`l >= r` または `d >= u`）では `T{}` を返します。
- コンストラクタの引数は値渡しです。左辺値で渡した配列は変更しません。`std::move(points)`で渡すと配列の領域を受け取れます。内部に入力配列への参照は保持しません。

## アルゴリズム・使い分け

点をx座標順に並べ、長方形のx範囲を配列の添字区間へ変換します。y座標は圧縮して重み付きWavelet Matrixを構築します。各段で上位ビットから安定分割し、0側へ進む要素の重みの累積和を保持します。yが上端未満の和から下端未満の和を引くことで、長方形内の和を求めます。

順位を求めるビット列は64bit単位にまとめ、ブロックごとの累積個数と `std::popcount` を使います。重みの累積和は各段でN要素分必要です。既存の `WaveletMatrix` は重みを保持しないため、重み付きの内部構造を独立して実装しています。

構築後にも点を追加する場合は [DynamicPointAddRectangleSum]({{ '/data_structure/DynamicPointAddRectangleSum.hpp.html' | relative_url }}) を使えます。静的な点集合では、RectangleSumの問い合わせはO(log N)、既存の動的版はO(log² N)です。

## 使用例

{% raw %}
```cpp
#include "data_structure/RectangleSum.hpp"

#include <cassert>
#include <utility>
#include <vector>

int main() {
    using RS = sora::RectangleSum<>;
    std::vector<RS::Point> points{
        {2, 3, 5}, {2, 3, -1}, {8, 9, 7}, {2, 5, 10}
    };
    const RS rs(std::move(points));
    assert(rs.size() == 4);
    assert(rs.rectangle_sum(0, 0, 5, 5) == 4); // y=5の点は含まない
    assert(rs.rectangle_sum(2, 3, 3, 6) == 14);
    assert(rs.rectangle_sum(0, 0, 10, 10) == 21);
    assert(rs.rectangle_sum(5, 0, 0, 10) == 0);
    const RS empty;
    assert(empty.rectangle_sum(0, 0, 10, 10) == 0);
}
```
{% endraw %}

## 検証コード・検証範囲

- `verify/RectangleSum.test.cpp`: Library Checkerの [Rectangle Sum](https://judge.yosupo.jp/problem/rectangle_sum) 用です。静的な点集合の長方形和を検証します。
- `verify/local/RectangleSum.local.cpp`: 全点走査との比較によるローカル検証です。空集合、重複、負の重み、同一x・y座標、64bitブロックの境界、符号付き・符号なし64bit座標の端、空・逆向きの長方形、汎用の座標型・重み型、コピー・代入、入力配列を変更しないことを対象にします。

負の重み・空集合・整数型の端点など、公式問題の制約外のケースはローカル検証の対象です。
