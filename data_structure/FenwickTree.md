---
title: "FenwickTree — 点更新・区間和・境界探索"
documentation_of: ./FenwickTree.hpp
---

## 概要

Fenwick Treeで配列の点加算・点代入・区間和を管理します。内部ブロックを使う木上の二分探索にも対応します。Nは配列長で、空間計算量はO(N)です。Tのコピー・加減算をO(1)とします。元の要素列や探索用の配列は追加で保持しません。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `FenwickTree<T>(n)` | 長さnの零配列 | O(N) |
| `add(p, x)` | a[p] += x | O(log N) |
| `get(p)` | a[p]のコピー。`sum(p, p + 1)`と等価 | O(log N) |
| `set(p, x)` | a[p]をxで置き換える | O(log N) |
| `sum(r)` | [0, r)の和 | O(log N) |
| `sum(l, r)` | [l, r)の和 | O(log N) |
| `find_last(l, f)` / `find_last<f>(l)` | fを満たす最大の右端 | O(log N) |
| `find_first(r, f)` / `find_first<f>(r)` | fを満たす最小の左端 | O(log N) |

探索は述語fの評価をO(1)とした時間です。`get`・`sum`・境界探索はconstで、内部状態を変更しません。

## 仕様・使い方

Tは `T{}` を零元とし、`+=`と減算を持つ加法可換群です。`+`・`-=`・大小比較は要求しません。n >= 0とし、点操作は `0 <= p < N`、区間は `0 <= l <= r <= N` を満たす必要があります。空区間の和は零元です。N = 0では空区間の和と境界0からの探索を利用できます。探索結果も0です。

加算・代入には負の値も使えます。整数型では、ブロックの和・累積和・差・`set`で使う `x - get(p)` など、途中の演算結果も型の範囲に収まる必要があります。

`get`は該当するブロックから手前の部分を減算して1点の値を取り出します。`set`は現在値との差分を `add` で加算します。既存の `add`・`sum` の使い方は変わりません。

### 木上の二分探索

探索の名前と戻り値は [SegmentTree]({{ '/data_structure/SegmentTree.hpp.html' | relative_url }}) に合わせています。結果は要素の値ではなく半開区間の境界です。関数オブジェクトを渡す形式と、`bool f(T)` の関数ポインタをテンプレート引数で指定する形式が使えます。

- `find_last(l, f)`: `0 <= l <= N`。`f(sum(l, r))` がtrueとなる最大のrを返します。右に区間を伸ばすとtrueからfalseへ単調に変わることが必要です。lがNならN、最後までtrueならNを返します。
- `find_first(r, f)`: `0 <= r <= N`。`f(sum(l, r))` がtrueとなる最小のlを返します。左に区間を伸ばすとtrueからfalseへ単調に変わることが必要です。rが0なら0、先頭までtrueなら0を返します。

どちらも `f(T{}) == true` が必要です。fは同じ引数に対して同じ結果を返し、副作用を持たないものとします。常にtrueな述語も使えます。

非負の要素列なら「区間和が上限以下」は単調です。負の要素が混在する場合、和に対する上限判定は一般には単調になりません。必要な単調性は探索対象の区間で成り立つことが条件で、単にTが比較可能であるだけでは不十分です。unsigned型の加算で周回する場合やmodintなどでも、和の大小が単調かを確認してください。

累積和を1回求めた後、Fenwick Treeの内部ブロックを大きいものから調べて境界を決めます。各候補で `sum` を呼ぶ方法のO(log² N)ではなく、O(log N)で探索します。

### 頻度列からの順位検索

各要素を非負の頻度として、k > 0に対する
`find_last(0, [k](T s) { return s < k; })` は、`sum(p + 1) >= k` となる最小の要素添字pを返します。該当する要素がなければNです。k <= 0では述語が零元でtrueにならないため、この形は使えません。

## 使用例

{% raw %}
```cpp
#include "data_structure/FenwickTree.hpp"

#include <cassert>

bool at_most_seven(long long x) { return x <= 7; }

int main() {
    sora::FenwickTree<long long> tree(5);
    tree.add(2, 7);
    tree.set(0, 2);
    tree.set(4, 3); // {2, 0, 7, 0, 3}
    assert(tree.get(2) == 7);
    assert(tree.sum(1, 4) == 7);
    assert(tree.find_last(0, [](long long x) { return x <= 2; }) == 2);
    assert(tree.find_first<at_most_seven>(4) == 1);
    const int p = tree.find_last(0, [](long long x) { return x < 8; });
    assert(p == 2); // 8番目の要素は添字2にある
}
```
{% endraw %}

## 検証

- `verify/FenwickTree.test.cpp`: Library Checkerの点加算・区間和を検証します。
- `verify/VertexAddPathSum.test.cpp`: HLDと組み合わせた点加算・パス和を検証します。
- `verify/FenwickTreePredecessor.test.cpp`: Library Checkerの [Predecessor Problem](https://judge.yosupo.jp/problem/predecessor_problem) で、0/1の頻度列に対する `get`・`set`・両方向の境界探索を検証します。
- `verify/local/FenwickTree.local.cpp`: 単純な配列と比較するローカル検証です。点加算・点代入・1点取得・区間和、任意の端点からの探索、空列、0の連続、非2冪長、関数ポインタ形式、独自の加法群を対象にします。負の値の更新と、非負の頻度列に対する探索を区別して確認します。
