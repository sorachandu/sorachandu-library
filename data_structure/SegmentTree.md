---
title: "SegmentTree — モノイドの点更新・区間積"
documentation_of: ./SegmentTree.hpp
---

## 概要・用途

モノイドを載せた完全二分木で、点更新、区間積、区間の境界探索を行います。和・最小値・最大値のほか、行列積や関数合成などの非可換な演算にも使えます。内部の葉数を2の冪に切り上げ、配列で保持します。

`math/Monoid.hpp` の `sora::Monoid` Conceptを満たすクラスMを指定します。必要な要素は値の型 `M::T`、結合的な演算 `M::op(a, b)`、単位元を返す `M::e()` です。Conceptは構文だけを検査し、結合則と左右の単位元の法則は利用者が保証します。可換性・逆元・冪等性は不要です。値の型にはコピー・代入など、各操作で使う処理が可能であることを要求します。

引数の順序、半開区間、境界探索の挙動は [AtCoder Libraryのsegtree](https://atcoder.github.io/ac-library/master/document_ja/segtree.html) に準じます。境界探索の名前は `find_last`（ACLの `max_right`）・`find_first`（ACLの `min_left`）としています。モノイドクラスを指定する点に加え、`size()`・`empty()` と葉列の `operator<<` を用意しています。ACLへの依存はありません。

## 公開API・計算量

Nを要素数とします。時間計算量は、`M::op`・`M::e`・値のコピーと代入をO(1)として評価します。保持領域は O(N) です。文字列連結などでは、演算やコピー自体のコストを別途考慮してください。

| API | 内容 | 時間 |
| --- | --- | --- |
| `SegmentTree<M>()` | 空列を構築 | O(1) |
| `SegmentTree<M>(int n)` | n要素を全て単位元で初期化 | O(n) |
| `SegmentTree<M>(const std::vector<M::T>& a)` | aから構築。入力は変更しない | O(N) |
| `size()` / `empty()` | 要素数 / 空かどうか | O(1) |
| `set(p, x)` | a[p]をxで置き換える | O(log N) |
| `get(p)` | a[p]のコピーを返す | O(1) |
| `prod(l, r)` | [l, r)の要素を左から順に演算 | O(log N) |
| `all_prod()` | 列全体の積 | O(1) |
| `find_last(l, f)` / `find_last<f>(l)` | fを満たす右端を探索 | O(log N) |
| `find_first(r, f)` / `find_first<f>(r)` | fを満たす左端を探索 | O(log N) |
| `os << seg` | 葉列を `[a[0], a[1], ...]` の形式で出力 | O(N) |

取得・区間積・探索・出力はconstな木にも使えます。出力は各要素の出力をO(1)、探索はfの評価をO(1)とした時間です。`operator<<` は `std::ostream << const M::T&` が有効なときだけ使えます。出力できない型も木自体には載せられます。

## 前提条件・境界ケース

- `0 <= N <= 10^8` とし、添字と要素数にはintを使います。入力の配列は変更しません。
- `set`・`get` は `0 <= p < N`、`prod` は `0 <= l <= r <= N` を要求します。
- 空区間の `prod` と空列の `all_prod` は単位元です。空列の `set`・`get` は呼べません。
- `set` は加算ではなく代入です。加算するなら `seg.set(p, seg.get(p) + x)` のように書きます。
- `operator<<` は内部ノードや単位元で埋めた末尾の葉を出力せず、元のN要素を出力します。空列は `[]`、末尾の改行はありません。要素の文字列表現に引用符やエスケープは加えません。

### 境界探索

fは値を受け取ってboolを返す関数オブジェクトです。同じ引数に対して同じ結果を返し、副作用を持たず、`f(M::e()) == true` を満たす必要があります。`find_last<f>`・`find_first<f>` 形式では、`bool f(M::T)` の関数ポインタをテンプレート引数に指定します。

- `find_last(l, f)`: `0 <= l <= N`。`f(prod(l, r))` がtrueになる最大のrを返します。区間を右に伸ばすと、fはtrueからfalseへ単調に変わる必要があります。lがNならNを返し、最後までtrueならNです。
- `find_first(r, f)`: `0 <= r <= N`。`f(prod(l, r))` がtrueになる最小のlを返します。区間を左に伸ばすと、fはtrueからfalseへ単調に変わる必要があります。rが0なら0を返し、先頭までtrueなら0です。

常にtrueな述語も使えます。例えば非負整数の和に対する「和が上限以下」は単調ですが、負数が混在すると単調とは限りません。非可換モノイドでも、積の順序は常に元の配列の左から右です。

## 使用例

{% raw %}
```cpp
#include "data_structure/SegmentTree.hpp"

#include <cassert>
#include <sstream>
#include <vector>

struct Sum {
    using T = long long;
    static T op(T a, T b) { return a + b; }
    static T e() { return 0; }
};

bool at_most_seven(long long x) { return x <= 7; }

int main() {
    sora::SegmentTree<Sum> seg(std::vector<long long>{1, 2, 3, 4});
    seg.set(1, 5); // {1, 5, 3, 4}
    assert(seg.get(1) == 5);
    assert(seg.prod(1, 3) == 8);
    assert(seg.all_prod() == 13);
    assert(seg.find_last(0, [](long long x) { return x <= 6; }) == 2);
    assert(seg.find_first<at_most_seven>(4) == 2);
    std::ostringstream out;
    out << seg;
    assert(out.str() == "[1, 5, 3, 4]");
}
```
{% endraw %}

## 検証コード・検証範囲

- `verify/PointSetRangeComposite.test.cpp`: Library Checkerの [Point Set Range Composite](https://judge.yosupo.jp/problem/point_set_range_composite) 用です。非可換な一次関数の合成で、点更新と区間積を検証します。
- `verify/local/SegmentTree.local.cpp`: 単純な走査と比較するローカル検証です。空列、非2冪長、ランダムな点更新と区間積、非可換演算、境界探索の積の順序、関数ポインタ形式、単位元での初期化、葉列の出力を対象にします。出力できない型の利用も確認します。

境界探索と出力はローカル検証の対象で、上記の公式問題には含まれません。
