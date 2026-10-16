---
title: "Trie — 小文字英字の多重集合"
documentation_of: ./Trie.hpp
---

## 概要

a〜zからなる文字列を重複込みで管理し、接頭辞・辞書順による検索を行います。空文字列も登録できます。確保済みノード数をPとして空間O(26P)。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `Trie()` | 空で初期化 | O(1) |
| `size()` / `empty()` | 重複込みの文字列数 / 空か | O(1) |
| `insert(s)` | 1個追加 | O(長さs) |
| `count_word(s)` / `count_prefix(s)` | 完全一致 / 接頭辞一致の個数 | O(長さs) |
| `erase(s)` / `erase_all(s)` | 1個 / 全て削除、成功可否 / 削除数を返す | O(長さs) |
| `count_less(s)` / `count_less_equal(s)` | 辞書順でs未満 / 以下の個数 | O(26 × 長さs) |
| `kth(k)` | 0-indexedでk番目、なければstd::nullopt | O(26 × 答えの長さ + 1) |
| `lower_bound(s)` / `upper_bound(s)` | s以上 / sより大きい最小文字列 | O(長さs + 26 × 答えの長さ + 1) |

## 仕様・使い方

a〜z以外を渡してはいけません。検索・追加・削除は入力文字列を変更しません。文字列の個数やノード番号はintに収まる必要があります。削除は個数を減らすだけで、ノードのメモリは解放しません。

kthは重複を別々に数えます。bound系は該当文字列がなければstd::nulloptです。Nodeのpassはその接頭辞を持つ文字列数、endはそのノードで終わる文字列数で、公開配列tは通常読み取り専用として扱います。

## 使用例

{% raw %}
```cpp
#include "string/Trie.hpp"

sora::Trie trie;
trie.insert("apple");
trie.insert("app");
trie.insert("app");
int count = trie.count_prefix("app");  // 3
auto first = trie.kth(0);  // "app"
```
{% endraw %}

## 検証

`verify/local/HeaderMigration.local.cpp`で重複登録・削除・辞書順検索を検証します。
