---
title: "RollingHash — 文字列の部分ハッシュ"
documentation_of: ./RollingHash.hpp
---

## 概要

法2^61 − 1の多項式ハッシュで部分文字列を比較します。全インスタンスでランダムに選んだ基数を共有します。Nは文字列長で、保持空間はO(N)です。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `RollingHash()` / `RollingHash(s)` | 空 / 文字列から構築 | O(1) / O(N) |
| `hash()` / `hash(l, r)` | 全体 / [l, r)のハッシュ | O(1) |
| `concat(h1, h2, h2_len)` | 2つのハッシュの連結 | O(1) |
| `size()` | 文字列長 | O(1) |
| コピー / ムーブ | 同じ内容のオブジェクト生成 | O(N) / O(1) |

## 仕様・使い方

入力文字列はコピーして保持します。文字列長はuint32_tの最大値未満とし、区間は0 <= l <= r <= Nを満たす必要があります。初回構築には乱数源の処理時間が別途かかります。

衝突の可能性があり、ハッシュの一致は文字列の一致を保証しません。長さも併せて比較してください。concatに渡すハッシュは本クラスで生成したもので、h2_lenは呼び出すオブジェクトのsize()以下である必要があります。公開の基数・文字列・前計算配列は変更しないでください。

## 使用例

{% raw %}
```cpp
#include "string/RollingHash.hpp"

sora::RollingHash hash("abcabc");
bool equal = hash.hash(0, 3) == hash.hash(3, 6);  // true
auto combined = hash.concat(hash.hash(0, 3), hash.hash(3, 6), 3);
// combined == hash.hash()
```
{% endraw %}

## 検証

`verify/local/HeaderMigration.local.cpp`で空文字列・部分文字列・連結を検証します。衝突が起きないことを証明する検証ではありません。
