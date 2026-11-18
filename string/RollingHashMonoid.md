---
title: "RollingHashMonoid — 文字列連結のハッシュ"
documentation_of: ./RollingHashMonoid.hpp
---

## 概要

法2^61 − 1の多項式ハッシュを使う文字列連結モノイドです。`sora::Monoid`を満たし、`SegmentTree<RollingHashMonoid>`で点更新と区間ハッシュの取得ができます。

基数は初回の`from_char`で実行時にランダム生成し、プログラム内で共有します。複数の翻訳単位から利用する場合も同じ基数を使います。基数を設定・変更する操作はありません。

## API・計算量

| API | 内容 | 時間 |
| --- | --- | --- |
| `T` | `hash`、`power`、`len`を持つ値。既定値は空文字列 | O(1) |
| `e()` | 空文字列の値 `{0, 1, 0}` | O(1) |
| `from_char(unsigned char c)` | 1バイトを符号化した値 `{c + 1, base, 1}` | O(1) |
| `op(a, b)` | `a`の後ろに`b`を連結した値。入力は変更しない | O(1) |
| `a == b` / `a != b` | 長さ・ハッシュ・基数のべきの比較 | O(1) |

各操作の追加空間はO(1)です。初回の`from_char`には乱数源・乱数生成器の初期化時間が別途かかります。
`e()`と`op()`は乱数生成を行いません。

`T::hash`は文字列のハッシュ、`T::power`は`base^len mod mod`、`T::len`はバイト数です。`mod`はpublicの定数です。

## 連結演算

文字は`unsigned char`として解釈し、その値に1を加えて符号化します。`a`と`b`の連結を次の式で計算します。ハッシュとべきの演算はすべて法`mod`で行います。

```text
hash  = a.hash * b.power + b.hash
power = a.power * b.power
len   = a.len + b.len
```

この演算は結合則を満たし、空文字列が単位元になります。順序を入れ替えることはできません。
各値に基数のべきを持たせるため、べき乗テーブルの準備や拡張は不要です。

セグメント木の構築は時間・空間O(N)、点更新・区間ハッシュ取得は時間O(log N)です。

## 前提条件・境界ケース

`op`には`e`・`from_char`・`op`で生成した値を渡してください。コピーはできますが、各フィールドは変更しないでください。連結後の長さは`std::size_t`に収まる必要があります。

空文字列や空区間は`e()`に対応します。NUL文字や128以上のバイト値も扱えます。UTF-8の文字列もバイト単位で扱います。

ハッシュには衝突の可能性があります。`T`の比較は長さも含みますが、等しい値でも元の文字列が一致するとは限りません。基数は実行ごとに変わり得るため、ハッシュ値を別の実行に持ち越して比較しないでください。`RollingHash`の値との互換性はありません。

## 使用例

{% raw %}
```cpp
#include <cassert>
#include <string>
#include <vector>

#include "data_structure/SegmentTree.hpp"
#include "string/RollingHashMonoid.hpp"

int main() {
    using M = sora::RollingHashMonoid;
    std::string s = "abcabc";
    std::vector<M::T> a;
    a.reserve(s.size());
    for (unsigned char c : s) a.push_back(M::from_char(c));
    sora::SegmentTree<M> seg(a);

    assert(seg.prod(0, 3) == seg.prod(3, 6));
    seg.set(1, M::from_char('x'));
    assert(seg.prod(0, 3) ==
           M::op(M::op(M::from_char('a'), M::from_char('x')), M::from_char('c')));
    assert(seg.prod(2, 2) == M::e());
}
```
{% endraw %}

## 検証コードと範囲

- `verify/RollingHashMonoidZAlgorithm.test.cpp`: Library CheckerのZ Algorithm用。累積ハッシュの連結と二分探索で各接尾辞のLCPを求めます。点更新は対象外です。
- `verify/local/RollingHashMonoid.local.cpp`: 空文字列、全256バイト、結合則・単位元、独立した剰余演算による単純解との比較、セグメント木の点更新と区間取得を検証します。

これらはハッシュ衝突が起きないことを保証する検証ではありません。
