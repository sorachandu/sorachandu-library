---
title: "LazySegmentTree — 作用付きモノイドの区間更新・区間積"
documentation_of: ./LazySegmentTree.hpp
---

## 概要・用途

作用付きモノイドを載せた遅延セグメント木です。点の代入、点・区間への作用、区間積、境界探索を行います。区間加算・区間和や区間一次関数適用・区間和などに使えます。値と作用素は配列で保持し、必要な経路だけ遅延作用を伝播します。

`math/LazyMonoid.hpp` の `sora::LazyMonoid` Conceptを満たすクラスMを指定します。必要な型は集約値 `M::T` と作用素 `M::F`、必要な関数は `op`・`e`・`mapping`・`composition`・`id` です。引数・合成順序・必要な法則は [LazyMonoid]({{ '/math/LazyMonoid.hpp.html' | relative_url }}) を参照してください。`composition(f, g)`はgの後にfを適用する合成です。

`SegmentTree`と共通の名前・半開区間を使い、`apply`を追加しています。境界探索の `find_last` はACLの `max_right`、`find_first` はACLの `min_left` に対応します。[AtCoder Libraryのlazysegtree](https://atcoder.github.io/ac-library/master/document_ja/lazysegtree.html) と同様、取得や探索でも遅延作用を伝播するため、一部の操作は非constです。ACLへの依存はありません。

## 公開API・計算量

Nを要素数とします。表の時間計算量はモノイドの各演算と値・作用素のコピー・代入をO(1)として評価しています。保持領域はO(N)です。演算やコピーが定数時間でない場合は、そのコストを別途考慮してください。

| API | 内容 | 時間 | const |
| --- | --- | --- | --- |
| `LazySegmentTree<M>()` | 空列を構築 | O(1) | — |
| `LazySegmentTree<M>(int n)` | n要素を全て `M::e()` で初期化 | O(n) | — |
| `LazySegmentTree<M>(const std::vector<M::T>& a)` | aから構築。入力は変更しない | O(N) | — |
| `size()` / `empty()` | 要素数 / 空かどうか | O(1) | 可 |
| `set(p, x)` | a[p]をxで置き換える | O(log N) | 不可 |
| `get(p)` | a[p]のコピーを返す | O(log N) | 不可 |
| `prod(l, r)` | [l, r)の積を左から順に求める | O(log N) | 不可 |
| `all_prod()` | 列全体の積 | O(1) | 可 |
| `apply(p, f)` | a[p]に作用素fを適用 | O(log N) | 不可 |
| `apply(l, r, f)` | [l, r)の各要素に作用素fを適用 | O(log N) | 不可 |
| `find_last(l, g)` / `find_last<g>(l)` | gを満たす最大の右端 | O(log N) | 不可 |
| `find_first(r, g)` / `find_first<g>(r)` | gを満たす最小の左端 | O(log N) | 不可 |
| `os << seg` | 葉列を `[a[0], a[1], ...]` と出力 | O(N) | 不可 |

探索は述語gの評価、出力は各要素の出力をO(1)としています。出力時は全ての内部ノードの遅延作用を上から順に一度だけ伝播します。全ての葉で `get` を呼ぶ方法のO(N log N)を避け、O(N)で出力します。

## 前提条件・境界ケース

- `0 <= N <= 10^8` とします。添字・要素数はintで、区間は0-indexedの半開区間です。
- `set`・`get`・点への `apply` は `0 <= p < N`、`prod`・区間への `apply` は `0 <= l <= r <= N` を要求します。
- 空区間の積と空列の全体積は `M::e()` です。空区間への `apply` は何もしません。空列への点操作は呼べません。
- `set`は代入、`apply`は作用です。`get`・`prod`・境界探索・出力は内部の遅延状態を変更しますが、論理的な要素列は変更しません。
- 型T・Fにはコピー・代入など、各操作で使う処理が可能であることを要求します。デフォルトコンストラクタ・等値比較演算子は不要です。恒等作用かどうかの比較は行いません。
- `mapping`には集約値だけを渡します。作用に区間長が必要なら、Tに長さを含めます。長さ指定のコンストラクタは全ての葉を `e()` にするため、長さ0の単位元を使う場合は、実際の葉を長さ1として配列から構築してください。`LazySegmentTree(n)` が「長さ1の値0」をn個作るとは限りません。
- 出力演算子は `std::ostream << const M::T&` が有効な型で使えます。出力できない型も木には載せられます。元のN要素だけを出力し、空列は `[]`、末尾の改行はありません。要素に引用符やエスケープは加えません。出力後も更新や問い合わせを続けられます。

### 境界探索

述語gは、同じ引数に同じ値を返し、副作用を持たず、`g(M::e()) == true` を満たす必要があります。`find_last<g>`・`find_first<g>` 形式には `bool g(M::T)` の関数ポインタを指定します。

- `find_last(l, g)`: `0 <= l <= N`。`g(prod(l, r))`がtrueとなる最大のrを返します。右に区間を伸ばすとtrueからfalseへ単調に変わることが必要です。lがNならNを返し、最後までtrueならNです。
- `find_first(r, g)`: `0 <= r <= N`。`g(prod(l, r))`がtrueとなる最小のlを返します。左に区間を伸ばすとtrueからfalseへ単調に変わることが必要です。rが0なら0を返し、先頭までtrueなら0です。

常にtrueな述語も使えます。積の順序は、探索方向によらず元の配列の左から右です。

## 使用例

{% raw %}
```cpp
#include "data_structure/LazySegmentTree.hpp"

#include <cassert>
#include <ostream>
#include <sstream>
#include <vector>

// 区間加算・区間和の作用付きモノイド。
struct RangeAddSum {
    // 区間和と要素数。出力時は和だけを表示する。
    struct T {
        long long sum, length;
        friend std::ostream& operator<<(std::ostream& os, const T& x) { return os << x.sum; }
    };
    using F = long long;
    static T op(T x, T y) { return {x.sum + y.sum, x.length + y.length}; }
    static T e() { return {0, 0}; }
    static T mapping(F f, T x) { return {x.sum + f * x.length, x.length}; }
    static F composition(F f, F g) { return f + g; }
    static F id() { return 0; }
};

bool at_most_twelve(RangeAddSum::T x) { return x.sum <= 12; }

int main() {
    sora::LazySegmentTree<RangeAddSum> seg(
        std::vector<RangeAddSum::T>{{1, 1}, {2, 1}, {3, 1}, {4, 1}});
    seg.apply(1, 4, 2); // {1, 4, 5, 6}
    seg.apply(0, 3);    // 点への加算: {4, 4, 5, 6}
    seg.set(2, {7, 1}); // {4, 4, 7, 6}
    assert(seg.get(2).sum == 7);
    assert(seg.prod(1, 3).sum == 11);
    assert(seg.all_prod().sum == 21);
    assert(seg.find_last(0, [](RangeAddSum::T x) { return x.sum <= 8; }) == 2);
    assert(seg.find_first<at_most_twelve>(4) == 3);
    std::ostringstream out;
    out << seg;
    assert(out.str() == "[4, 4, 7, 6]");
    seg.apply(0, 4, 1);
    assert(seg.all_prod().sum == 25);
}
```
{% endraw %}

例は整数の演算結果が型の範囲に収まることを前提にしています。

## 検証コード・検証範囲

- `verify/RangeAffineRangeSum.test.cpp`: Library Checkerの [Range Affine Range Sum](https://judge.yosupo.jp/problem/range_affine_range_sum) 用です。区間への一次関数適用と区間和を検証します。作用素の合成が非可換なので、合成順序も検証できます。
- `verify/local/LazySegmentTree.local.cpp`: 単純な走査と比較するローカル検証です。空列、非2冪長、点代入、点・区間への作用、区間積、非可換な演算と作用素、境界探索、葉列の出力、出力後の操作を対象にします。Conceptの成立と不成立、デフォルト構築・等値比較・出力の要件も確認します。

境界探索・出力・点操作はローカル検証の対象で、上記の公式問題には含まれません。
