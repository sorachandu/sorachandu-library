---
title: "WaveletMatrix — 静的区間の順位・頻度"
documentation_of: ./WaveletMatrix.hpp
---

## 概要・用途

整数列に対して、区間内のk番目の値、指定した値の出現回数、値域内の要素数、しきい値の直前・直後の値を求めます。構築後の更新には対応しません。

Nを入力長、σを値の種類数、Hを `ceil(log2(max(1, σ)))` とします。値を座標圧縮し、上位ビットから順に0と1で安定分割します。各段のビット列は64bit単位にまとめ、ブロックごとの累積個数と `std::popcount` で順位を求めます。

保持領域は O(σ + (N / 64 + 1) H) ワードです。各段でNビットと約N / 64個の `int` の累積個数を保持し、圧縮辞書にはσ個のTを保存します。構築中は別途 O(N) の作業領域を使います。全て同じ値ならHは0で、ビット列を保持しません。

## 公開API・計算量

`T` は `std::integral` を満たす、bool・const・volatile以外の型です。既定のテンプレート引数はありません。以下のクエリは全てconstです。

| API | 内容 | 時間 |
| --- | --- | --- |
| `WaveletMatrix<T>()` | 空列の構築 | O(1) |
| `WaveletMatrix<T>(const std::vector<T>& a)` | aから構築。aは変更しない | O((N + 1) log(N + 2)) |
| `size()` / `empty()` | 要素数 / 空かどうか | O(1) |
| `get(p)` | 元のa[p] | O(H + 1) |
| `rank(l, r, x)` | [l, r)内でxに等しい要素数 | O(log(σ + 2)) |
| `kth_smallest(l, r, k)` | [l, r)内の小さい方からk番目 | O(H + 1) |
| `kth_largest(l, r, k)` | [l, r)内の大きい方からk番目 | O(H + 1) |
| `range_freq(l, r, upper)` | [l, r)内でupper未満の要素数 | O(log(σ + 2)) |
| `range_freq(l, r, lower, upper)` | [l, r)内で値域[lower, upper)に入る要素数 | O(log(σ + 2)) |
| `prev_value(l, r, upper)` | [l, r)内でupper未満の最大値 | O(log(σ + 2)) |
| `next_value(l, r, lower)` | [l, r)内でlower以上の最小値 | O(log(σ + 2)) |

`get` とk番目の取得はTを返します。頻度はint、前後の値は `std::optional<T>` を返します。座標圧縮のため、頻度・前後の値のクエリには圧縮辞書の二分探索も含まれます。

## 前提条件・境界ケース

- `N <= INT_MAX` とします。添字・要素数・kはintで、区間は `0 <= l <= r <= N` の半開区間です。
- `get(p)` は `0 <= p < N`、k番目の取得は `0 <= k < r - l` が必要です。kは0-indexedで、重複も個別の要素として数えます。
- 値域を指定する `range_freq` は `lower <= upper` が必要です。空値域では0を返します。
- 空列・空区間では頻度は0、前後の値は `std::nullopt` を返します。該当する値がない場合も `std::nullopt` です。
- 負数、符号付き・符号なし整数型の最小値と最大値に対応します。しきい値はTとして渡してください。内部でしきい値への加減算は行いません。
- 入力に含まれない値もしきい値として指定できます。`prev_value` はしきい値を含まず、`next_value` はしきい値を含みます。
- lower以上の要素数は `r - l - range_freq(l, r, lower)` で求められます。最大値に1を足して上端を作る必要はありません。

## 使用例

{% raw %}
```cpp
#include "data_structure/WaveletMatrix.hpp"

#include <cassert>
#include <vector>

int main() {
    const std::vector<long long> a{3, -2, 3, 7, 1};
    const sora::WaveletMatrix<long long> wm(a);
    assert(wm.get(1) == -2);
    assert(wm.kth_smallest(1, 5, 2) == 3); // ソートすると {-2, 1, 3, 7}
    assert(wm.kth_largest(0, 5, 0) == 7);
    assert(wm.rank(0, 5, 3) == 2);
    assert(wm.range_freq(0, 5, -2, 3) == 2);
    assert(wm.prev_value(0, 5, 3).value() == 1);
    assert(wm.next_value(0, 5, 3).value() == 3);
    assert(!wm.next_value(0, 5, 8).has_value());
}
```
{% endraw %}

## 検証コード・検証範囲

Library Checker用の検証コードは次の2本です。公式問題で検証する機能は、それぞれk番目の取得と一致個数です。

- `verify/RangeKthSmallest.test.cpp`: [Range Kth Smallest](https://judge.yosupo.jp/problem/range_kth_smallest)
- `verify/StaticRangeFrequency.test.cpp`: [Static Range Frequency](https://judge.yosupo.jp/problem/static_range_frequency)

`verify/local/WaveletMatrix.local.cpp` では、区間を取り出してソートする単純解と全クエリを比較します。空列・空区間、重複、全要素同一、64bitブロックの境界、小規模配列の全区間、ランダム区間、符号付き・符号なし64bit整数の最小値・最大値を対象にします。前後の値や値域内個数、元の要素の取得はローカル検証の対象です。
