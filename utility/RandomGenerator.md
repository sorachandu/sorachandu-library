---
title: "RandomGenerator — 乱数・テストケース生成"
documentation_of: ./RandomGenerator.hpp
---

## 概要

Pythonの`random`に近いAPIで、数値・シーケンス・テストケースを生成します。
`std::mt19937_64`を既定のエンジンとし、C++20以降で利用できます。
整数列、順列、文字列に加えて、整数の組、正整数への分割、木、グラフを生成できます。
今後の乱数生成にはこのクラスを使用してください。
`RangeRandom`は管理対象から外したため、本リポジトリには含めません。

```cpp
sora::RandomGenerator rnd(12345);
auto x = rnd.randint(1, 100);
auto a = rnd.randint_vec(10, -1000, 1000);
auto p = rnd.permutation(10);
auto s = rnd.randstr(10, "abc");
auto edges = rnd.tree(10);
```

## RangeRandomからの移行

`RangeRandom`は生成範囲をオブジェクトに保持しますが、`RandomGenerator`では生成するたびに範囲を指定します。
整数の生成範囲はどちらも閉区間`[l, r]`です。

| 旧API | 移行後 |
| --- | --- |
| `RangeRandom rnd(l, r); rnd.gen();` | `RandomGenerator rnd; rnd.randint(l, r);` |
| `rnd.set_range(l, r)` | 次回以降の`randint(l, r)`に新しい範囲を渡す |
| `rnd.get_range()` | 必要なら呼び出し側で範囲を変数に保持する |
| `rnd()` | 範囲付き整数には`rnd.randint(l, r)`を使う |

`RandomGenerator::operator()()`はエンジンの生の乱数を返すため、旧APIの`operator()()`とは用途が異なります。
旧実装の戻り値は`std::int64_t`でした。移行後も同じ型が必要なら、両端を`std::int64_t`で渡してください。
旧実装の既定範囲`[0, INT64_MAX]`も、移行後は両端を明示します。
エンジンは`std::mt19937`から既定の`std::mt19937_64`に変わるため、旧実装と同じ乱数列を生成することは保証しません。

## 型・再現性

`RandomGenerator<Engine = std::mt19937_64>`を`sora`名前空間に定義しています。
`engine_type`と`state_type`は`Engine`、`result_type`は`Engine::result_type`です。
`Engine`には`std::uniform_random_bit_generator`を要求します。
さらに、デフォルト構築、コピー、コピー代入、`seed(result_type)`、`seed(std::seed_seq&)`が必要です。
例えば`RandomGenerator<std::mt19937>`や`RandomGenerator<std::minstd_rand>`も利用できます。

固定seed・同一環境・同じ呼び出し手順なら、同じ結果を生成します。
環境には標準ライブラリとそのバージョンも含みます。
標準の分布やシャッフルを利用するため、異なる環境やPythonとの乱数列の一致は保証しません。
分布を呼び出しごとに構築するため、`getstate()`で取得したエンジンの状態だけで乱数列を復元できます。

以下の計算量はエンジンの1回の生成・固定seedの設定・状態コピーと、要素1個のコピーをそれぞれO(1)としたものです。
エンジン自体の状態に使う空間は、既定の`std::mt19937_64`ではO(1)です。
個数を表す引数は`permutation`・`tree`・`graph`の頂点数を除き`std::size_t`です。負の値は渡さないでください。
不正な引数は`assert`で検出します。`NDEBUG`を定義した場合も、呼び出し側で前提条件を満たす必要があります。

## 状態管理

| API | 内容 | 時間 |
| --- | --- | --- |
| `RandomGenerator()` | 非固定seedで初期化 | 乱数源に依存 |
| `RandomGenerator(seed)` | `result_type`の固定seedで初期化 | O(1) |
| `seed(s)` | 固定seedで再初期化 | O(1) |
| `seed()` | `std::random_device`と`std::seed_seq`で再初期化 | 乱数源に依存 |
| `getstate()` | エンジンの状態を値で取得 | O(1) |
| `setstate(state)` | 同じエンジン型の状態を復元 | O(1) |
| `operator()()` | エンジンの生の乱数 | ならしO(1) |
| `min()` / `max()` | 生の乱数の最小値・最大値 | O(1) |

`seed()`は乱数源に依存するため、呼び出すたびに異なる乱数列になるとは限りません。
`min()`と`max()`は静的メソッドです。`RandomGenerator`自体を標準の分布などに渡すこともできます。

## 数値・ビット・バイト

整数引数には`bool`と128bit整数を除く、64bit以下の標準整数型を使えます。
複数の整数引数は、すべて符号付き、またはすべて符号なしにそろえてください。
`randint(0, 100LL)`は利用できますが、`randint(0, 100ULL)`はコンパイル時に拒否します。
複数引数の戻り値はそれらの`std::common_type_t`です。1引数の`randrange`は引数と同じ型を返します。
端点を含む64bit整数の全域も扱えます。

| API | 内容・前提条件 | 期待時間 |
| --- | --- | --- |
| `randint(a, b)` | 閉区間`[a, b]`の一様整数。`a <= b` | O(1) |
| `randrange(b)` | `[0, b)`の一様整数。`b > 0` | O(1) |
| `randrange(a, b)` | `[a, b)`の一様整数。`a < b` | O(1) |
| `randrange(a, b, step)` | `a, a + step, ...`から一様に選ぶ。`b`を含まない | O(1) |
| `random()` | `[0, 1)`の`double` | O(1) |
| `uniform(a, b)` | 有限の`a <= b`に対して`[a, b)`の`double`。等しければ`a` | O(1) |
| `bernoulli(p)` | 確率`p`で`true`。有限の`0 <= p <= 1` | O(1) |
| `getrandbits(k)` | 下位`k`ビットが一様な`std::uint64_t`。`0 <= k <= 64` | O(1) |
| `randbytes(n)` | 一様な`n`バイトを`std::vector<std::uint8_t>`で返す | O(n) |

`randrange(a, b, step)`には、`step != 0`かつ候補が1個以上あることが必要です。
正のstepでは`a < b`、負のstepでは`a > b`を満たしてください。
例えば`randrange(0, 10, 2)`は`0, 2, 4, 6, 8`、`randrange(10, 0, -3)`は`10, 7, 4, 1`から選びます。
差やstepの計算にはGNU拡張の128bit整数を使い、64bit端点でのオーバーフローを避けています。

`getrandbits(0)`は乱数を消費せず0を返します。
`randbytes(0)`は空のvectorを返します。
実数は浮動小数点数で表現できる値に丸められます。`uniform`は上端への丸めを補正して、`a < b`なら`b`を返さないようにします。

## シーケンス操作

入力には`std::ranges::random_access_range`かつ`std::ranges::sized_range`を要求します。
`std::vector`、`std::array`、`std::string`、組み込み配列、`std::span`などを利用できます。
`choice`・`choices`・`sample`・`shuffled`では、要素を値としてコピーできる必要があります。
`shuffle`と`shuffled`には、さらに出力先のイテレータが`std::permutable`を満たすことが必要です。
`shuffle`以外は入力を変更しません。

`N`を入力の要素数、`k`を抽出数とします。

| API | 内容 | 期待時間 | 出力を除く追加空間 |
| --- | --- | --- | --- |
| `choice(v)` | 1要素を値で返す。非空が必要 | O(1) | O(1) |
| `choices(v, k)` | 重複を許して`k`個抽出 | O(k) | O(1) |
| `choices(v, k, weights)` | 重みに比例する確率で復元抽出 | O(N + k log N) | O(N) |
| `sample(v, k)` | 異なる位置から`k`個をランダムな順序で抽出。`k <= N` | O(k) | O(k) |
| `shuffle(v)` | 入力を破壊的にシャッフル | O(N) | O(1) |
| `shuffled(v)` | シャッフル済みのコピー | O(N) | O(1) |

`choices`・`sample`・`shuffled`の戻り値は入力の値型を要素とする`std::vector`です。
`shuffled`は入力が文字列でも`std::vector<char>`を返します。
空入力に対しては、`choices`と`sample`は`k == 0`のときだけ利用できます。`shuffle`と`shuffled`も空入力を受け付けます。
`sample`は値の重複を禁止しません。例えば`sample({7, 7, 7}, 3)`に相当する操作の結果は`{7, 7, 7}`です。
組み込み配列は全要素を対象とするため、文字列リテラルをそのまま渡すと末尾のNULも候補に入ります。文字列には`std::string_view`などを使ってください。

重みは要素と同数の、サイズを取得できる入力範囲で渡します。各要素は`double`に変換できる必要があります。
変換後の重みは有限・非負で、入力が非空なら`double`で計算した総和が正かつ有限であることが必要です。
重み0の位置は選びません。重みの累積和は`double`なので、極端に小さい重みは丸めにより失われる場合があります。
累積和をO(N)で構築し、各抽出を二分探索で行います。`N == 1`の場合の時間はO(k)です。

`sample`は、仮想的な配列への変更を`std::unordered_map`に保存する疎なFisher–Yates法を使います。
要素数Nに比例する走査や確保をせず、抽出した位置だけを扱います。
ハッシュ表の操作を期待O(1)としているため、計算量は期待値です。

## 競プロ向けの生成

以下も入力を変更しません。

| API | 内容・戻り値 | 期待時間 | 出力を含む追加空間 |
| --- | --- | --- | --- |
| `permutation(n)` | `[0, n)`の一様な順列。`std::vector<int>` | O(n) | O(n) |
| `randint_vec(n, a, b)` | 閉区間`[a, b]`の一様整数列 | O(n) | O(n) |
| `distinct(n, a, b)` | 閉区間`[a, b]`から相異なる整数をランダムな順序で抽出 | O(n) | O(n) |
| `randstr(n, alphabet)` | alphabetの位置を一様に選ぶ長さnの`std::string` | O(n) | O(n) |
| `randpair(a, b)` | `a <= l < r <= b`の整数の組を一様に選ぶ。`std::pair` | O(1) | O(1) |
| `composition(n, k)` | 和がnのk個の正整数からなる順序付き組。`std::vector<std::int64_t>` | O(k log k) | O(k) |
| `tree(n)` | n頂点のラベル付き木。`std::vector<std::pair<int, int>>` | O(n) | O(n) |
| `graph(n, m, directed = false)` | n頂点m辺の単純グラフ。同上 | 無向O(m log n)、有向O(m) | O(m) |

`randint_vec`・`distinct`の要素型と`randpair`の両端の型は`randint`と同じ整数型の規則に従います。
`permutation`・`tree`・`graph`の頂点数は非負の`int`です。
`composition`の総和は非負の`std::int64_t`です。

### 整数列・文字列・分割

`randint_vec`と`distinct`は、個数が0でも`a <= b`が必要です。
`distinct`は`n <= b - a + 1`を満たす必要があります。幅は128bit整数で確認するため、64bit整数の全域から抽出できます。
`randpair`は`a < b`が必要です。異なる2整数を抽出して昇順に並べ、すべての組を等確率にします。

`randstr`のalphabetは`std::string_view`で、既定値は`"abcdefghijklmnopqrstuvwxyz"`です。
`n > 0`なら空のalphabetは使えません。同じ文字が複数あれば、各位置を別の候補として扱います。

`composition`は`1 <= k <= n`が必要ですが、`(n, k) == (0, 0)`では空列を返します。
`k == 1`ではO(1)で`{n}`を返します。
`[1, n)`から異なるk−1個の切れ目を抽出してソートし、隣接する切れ目の差を取ります。
切れ目の集合と順序付きの正整数分割が一対一に対応するため、分割を一様に生成できます。

### 木

頂点番号は`[0, n)`です。`n == 0`と`n == 1`では辺のない空列を返します。
`n >= 2`では、長さn−2のPrüfer列を一様に生成し、最小の葉を追跡する方法で線形時間に復号します。
各ラベル付き木を等確率に生成し、辺は`u < v`に正規化します。
辺集合の分布は一様ですが、返す辺の列の順序は一様とは限りません。必要なら戻り値を`shuffle`してください。

### グラフ

既定では無向グラフです。`directed = true`なら有向グラフを生成します。
自己ループ・多重辺を含みません。連結性は保証しません。
無向では`0 <= m <= n(n - 1)/2`、有向では`0 <= m <= n(n - 1)`が必要です。
`n <= 1`では`m == 0`のみ有効です。

すべての候補辺に番号を付け、m個の番号を非復元抽出します。
各m辺の集合を一様に選び、辺もランダムな順序で返します。
無向辺は大きい端点ごとに並べ、三角数を二分探索して番号を`u < v`の端点に戻します。
有向辺は始点ごとにn−1本を割り当て、O(1)で復号します。
候補辺全体の配列や隣接行列は構築しません。

## 使用例

{% raw %}
```cpp
#include "utility/RandomGenerator.hpp"

#include <cassert>
#include <vector>

int main() {
    sora::RandomGenerator rnd(12345);
    std::vector<int> v{1, 2, 3, 4, 5};

    auto state = rnd.getstate();
    auto selected = rnd.sample(v, 3);
    rnd.setstate(state);
    assert(rnd.sample(v, 3) == selected);

    auto repeated = rnd.choices(v, 10);
    auto weighted = rnd.choices(v, 10, std::vector<double>{1, 0, 2, 0, 3});
    rnd.shuffle(v);

    auto a = rnd.randint_vec(100, -1000, 1000);
    auto unique = rnd.distinct(5, 0LL, 1000000000000LL);
    auto permutation = rnd.permutation(100);
    auto text = rnd.randstr(100, "abc");
    auto [l, r] = rnd.randpair(0, 100);
    auto parts = rnd.composition(100, 10);
    auto tree = rnd.tree(100);
    auto graph = rnd.graph(100, 200);
    auto digraph = rnd.graph(100, 200, true);
}
```
{% endraw %}

## 検証コード・検証範囲

対応する公式問題はなく、`verify/local/RandomGenerator.local.cpp`でローカル検証を行います。
`python3 verify/RunLocalTests.py`で、他のローカルテストとともに実行できます。

- `mt19937_64`・`mt19937`・`minstd_rand`で固定seed、生の出力、状態復元、コピー、非固定seedの呼び出しを確認します。
- 符号の混在と`bool`の拒否、整数の戻り値型、64bit整数の端点、負のstep、ビット数0・64、空列を確認します。
- 配列・文字列・span・`vector<bool>`からの抽出、位置の重複禁止、入力の保持、重み0の除外を確認します。
- 要素数10の12乗の仮想範囲から10要素を抽出し、全体の走査・確保がないことを確認します。
- 順列・相異なる整数・文字列・正整数分割の条件、木の連結性と閉路の不在、グラフの端点・辺数・単純性を確認します。
- 10万頂点の木、`INT_MAX`頂点の疎グラフ、完全グラフまでの生成を確認します。
- 固定seedで小さい木・グラフ・分割・整数の組・順序付き抽出の出現頻度を確認します。

頻度の確認は実装上の偏りを見つけるためのテストであり、乱数の品質や一様性を統計的に保証するものではありません。
暗号学的な品質、非固定seedの非決定性、異なる標準ライブラリ間の再現性は検証していません。
