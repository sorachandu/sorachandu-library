---
title: "RunLengthEncoding — 連続要素の圧縮"
documentation_of: ./RunLengthEncoding.hpp
---

## 概要

連続する同じ要素を「値・連続数」のpairにまとめます。文字列とstd::vectorの両方を扱い、入力を変更しません。比較・コピーをO(1)とし、時間計算量はO(N)、出力空間はO(N)です。

## API・計算量

| 操作 | 戻り値 | 時間 |
| --- | --- | --- |
| `run_length_encoding(string)` | vector<pair<char, long long>> | O(N) |
| `run_length_encoding(vector<T>)` | vector<pair<T, long long>> | O(N) |

## 仕様・使い方

同じ値でも離れて現れるものは別の区間になります。空入力の結果は空です。連続数はlong long、入力長は現在の実装ではintに収まる必要があります。配列版のTには等値比較とコピーが必要です。

## 使用例

{% raw %}
```cpp
#include "utility/RunLengthEncoding.hpp"

#include <string>
#include <vector>
auto text = sora::run_length_encoding(std::string("aaabb"));
// ('a', 3), ('b', 2)
auto array = sora::run_length_encoding(std::vector<int>{1, 1, 2, 1});
// (1, 2), (2, 1), (1, 1)
```
{% endraw %}

## 検証

`verify/local/HeaderMigration.local.cpp`で文字列・配列の圧縮と空入力を検証します。
