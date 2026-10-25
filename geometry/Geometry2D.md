---
title: "Geometry2D — 整数中心の平面幾何"
documentation_of: ./Geometry2D.hpp
---

## 概要

点・ベクトル、内積・外積、CCW、線分交差、凸包、多角形の面積と内外判定を提供します。

整数で答えられる操作は整数で計算します。距離・射影・直線交点だけ、計算結果の実数型をテンプレート引数で選びます。
円・円の交点、接線、回転角、半平面交差はこのヘッダでは扱いません。

## 型と数値の扱い

`Point<T, C = DefaultCalculation<T>>`は座標をTで保持し、内積・外積・距離の二乗をCで計算します。
標準整数型TではCの既定値は`__int128_t`、それ以外ではTです。
`Vector<T, C>`はPointの別名です。`Segment<T, C>{a, b}`と`Line<T, C>{a, b}`で線分と直線を表します。

T/Cには符号付きの数値型を使い、全中間値が計算する型に収まる必要があります。
内積は積の前、3点の外積と距離の二乗は差の前にCへ変換します。
ただし、`p + q`・`p - q`・単項`-`・`p * k`・`p / k`はTで計算します。整数の除算は整数除算で、除数は非零とします。
大きな座標では`cross(a, b, c)`を使ってください。`cross(b - a, c - a)`では座標型Tの減算が先に実行されます。

long longの全範囲を無条件に扱えるわけではありません。絶対値B以下の整数座標では、外積・距離の二乗の中間値の絶対値は8B²以下です。
例えばB <= 10^18ならこれらはsigned 128bitに収まります。面積の累積では、さらに頂点数Nに応じた余裕が必要です。
Cは明示できるため、必要なら利用環境で使える多倍長整数型に変更できます。ただし、その場合の時間計算量は各演算のコストにも依存します。

### 実数型と誤差

距離・射影・交点の実数型Rの既定値は`long double`です。`distance<double>(...)`のように変更できます。
`double`は速度を重視する場合、`long double`は追加の精度が必要な場合に使います。long doubleの精度は環境に依存します。
大きな座標の近接点や、ほぼ平行な直線の交点では丸め誤差が増えます。Rを高精度にしても、入力時に失った精度は戻りません。

GCCの`__float128`もRや座標型に指定できます。平方根を使うdistanceでは、`sqrtq`などを明示的に渡します。
`std::float128_t`は環境が提供している場合に使えますが、本環境では検証していません。
平方根の関数は戻り値もRとしてください。標準の平方根を利用できない型を、暗黙にdoubleへ落として計算する設計にはしていません。

CCW・交差・凸包・内外判定のepsはC型の絶対許容誤差で、既定値は0です。負のepsは禁止します。
整数ではeps=0を使います。実数では問題の座標スケールに応じて明示してください。
外積・内積・距離の二乗に同じepsを使う簡潔な規則であり、座標の相対誤差や厳密な幾何述語を保証するものではありません。
locateのy座標による半開区間判定と、点の等値・辞書順比較はepsに依存しません。NaN・無限大は入力しないでください。
ソートには誤差比較を使わず、厳密な辞書順を使います。凸包で重複を除くときも座標の厳密一致を使います。

## API・計算量

以下は数値の基本演算と平方根をO(1)とした計算量です。`is >> p`はpを書き換えます。それ以外は入力を変更しません。

| 操作 | 意味・戻り値 | 時間・追加空間 |
| --- | --- | --- |
| `Point<T, C>(x, y)` / 既定構築 | 点・ベクトル / 原点 | O(1) |
| `p + q`, `p - q`, `-p`, `p * k`, `p / k` | 座標型Tでの演算 | O(1) |
| `p == q`, `p < q` | 厳密一致 / x、yの辞書順 | O(1) |
| `p.cast<R>()` | R型の点へ変換 | O(1) |
| `is >> p` | x、yの順に読み込み、入力ストリームを返す | 時間はTの入力2回分、追加空間O(1) |
| `os << p` | `x y`を出力し、出力ストリームを返す。改行なし | 時間はTの出力2回分、追加空間O(1) |
| `sign(value, eps = 0)` | 正1・負-1・零0 | O(1) |
| `dot(a, b)`, `cross(a, b)` | ベクトルの内積・外積、C型 | O(1) |
| `cross(a, b, c)` | (b-a)×(c-a)、C型 | O(1) |
| `squared_distance(a, b)` | 距離の二乗、C型 | O(1) |
| `ccw(a, b, c, eps = 0)` | 有向線分abに対するcの位置 | O(1) |
| `contains(segment, p, eps = 0)` | 点が線分上にあるか | O(1) |
| `intersect(s, t, eps = 0)` | 線分が交差するか | O(1) |
| `twice_signed_area(polygon)` | 符号付き面積の2倍、C型 | 時間O(N)、空間O(1) |
| `convex_hull(points, eps = 0)` | 凸包の頂点列 | 時間O(N log(N+1))、空間O(N) |
| `locate(polygon, p, eps = 0)` | Locationのoutside・boundary・inside | 時間O(N)、空間O(1) |
| `distance<R>(a, b, sqrt_fn = StandardSqrt{})` | 点間距離、R型 | O(1) |
| `distance<R>(segment, p, sqrt_fn = StandardSqrt{})` | 非負の点・線分間距離、R型 | O(1) |
| `projection<R>(line, p)` | 直線への射影、Point<R> | O(1) |
| `line_intersection<R>(l, m, eps = 0)` | 一意な交点のoptional<Point<R>> | O(1) |

Point・Segment・Lineの保持空間はO(1)で、多角形のvectorはO(N)です。
convex_hullは引数を値で受け、内部でソートします。元配列を不要にする場合は`std::move(points)`でコピーを避けられます。

入出力には`std::istream`と`std::ostream`を使います。座標型Tが対応するストリーム演算を持つ場合に利用できます。
出力の区切りは空白1文字です。浮動小数点数の精度などの書式設定はストリームに従います。
`std::cin >> p >> q`、`std::cout << p << '\n'`のように連結できます。
入力失敗時の状態・例外・座標の更新は、`is >> p.x >> p.y`と同じです。途中で失敗した場合、点の一部が更新されることがあります。

## 前提条件・境界ケース

CCWは反時計回り1、時計回り-1、始点より後方2、終点より前方-2、線分上0です。
a == bのときはc == aを線分上、それ以外を前方とします。epsを指定すると、この分類も許容誤差に従います。
線分交差は端点の接触・重なりを含みます。長さ0の線分も扱います。

凸包は重複と辺上の中間点を除き、辞書順最小の頂点から反時計回りに返します。始点を末尾に再掲しません。
空入力は空、同一点だけなら1点、全点が同一直線上なら両端の2点を返します。

面積とlocateの頂点列は周回順とし、始点の再掲は不要です。locateは単純多角形を前提とし、周回方向は問いません。
空多角形はoutside、1点・2点では一致する点・線分上だけboundaryです。面積は空・1点・2点で0、反時計回りで正です。

直線は異なる2点で指定します。射影はRへ変換した後も方向ベクトルの二乗が非零である必要があります。
line_intersectionは平行・一致をnulloptとします。平行判定はCで行い、交点座標はRで計算します。
点・線分間距離は退化線分では端点との距離を返します。
実数演算ではオーバーフロー・アンダーフローや、Rへの変換で異なる座標が同一になる場合を呼び出し側で考慮してください。

## 使用例

{% raw %}
```cpp
#include "geometry/Geometry2D.hpp"
#include <vector>

int main() {
    using P = sora::Point<long long>;
    P a{0, 0}, b{4, 0}, p{2, 3};
    int direction = sora::ccw(a, b, p);  // 1
    __int128_t d2 = sora::squared_distance(a, p);  // 13
    bool hit = sora::intersect(sora::Segment<long long>{a, b},
                              sora::Segment<long long>{{2, -1}, {2, 1}});  // true
    long double d = sora::distance(sora::Segment<long long>{a, b}, p);  // 3
    auto hull = sora::convex_hull(std::vector<P>{{0,0}, {1,0}, {1,1}, {0,1}, {0,0}});
    auto location = sora::locate(hull, P{0,0});  // Location::boundary
    (void)direction; (void)d2; (void)hit; (void)d; (void)location;
}
```
{% endraw %}

GCCのlibquadmathを使う場合は`#include <quadmath.h>`とリンクオプション`-lquadmath`を追加し、
`geo::distance<__float128>(a, b, [](const __float128 x) { return sqrtq(x); })`のように呼びます。
平方根のAPIは[GCCの公式資料](https://gcc.gnu.org/onlinedocs/gcc-13.2.0/libquadmath/Math-Library-Routines.html)を参照してください。

## 検証コードと検証範囲

- Library CheckerのStatic Convex Hull用: `verify/StaticConvexHull.test.cpp`。`oj-verify run`で公式データ全25ケースの成功を確認しました。
- ローカル: `verify/local/Geometry2D.local.cpp`。CCWの5分類、点・線分の退化、交差の単純解比較、凸包の支持直線・回転方向、重複・共線・空入力、面積、凸・凹多角形の内外、射影・交点・距離を検証します。
- Pointの入出力はローカルで、整数・実数の座標順、空白区切り、改行なし、演算子の連結、実数の出力精度、入力失敗時の状態を確認します。
- 差がlong longの範囲を超えるケース、double・long double、独自の平方根関数を渡した__float128をローカルで確認します。libquadmathのsqrtqを渡す使用例も別途コンパイル・実行して確認しました。
- std::float128_tや多倍長整数型、全ての浮動小数点の誤差条件を網羅する検証ではありません。
