---
title: "Compression — 座標圧縮"
documentation_of: ./Compression.hpp
---

## 概要

配列の値を昇順の順位へ圧縮します。入力をコピーするため元配列は変更しません。Nは入力長、比較・コピーはO(1)、空間O(N)。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `Compression<T>(a)` | ソート・重複除去・各要素の順位計算 | O(N log(N + 1)) |
| `operator[](i)` / `get(i)` | 元配列a[i]の順位 | O(1) |
| `size()` | 異なる値の数 | O(1) |
| `get_from_val(value)` | value以上の最初の登録値の順位 | O(log(N + 1)) |

## 仕様・使い方

公開配列`cmp`は異なる値の昇順列、`id[i]`はa[i]の順位です。元の値は`cmp[id[i]]`で復元できます。`get_from_val`は未登録値も受け入れ、全登録値より大きければ`size()`を返します。構築後に公開配列を変更すると整合性が失われます。

## 使用例

{% raw %}
```cpp
#include "data_structure/Compression.hpp"

#include <vector>
sora::Compression<int> compressed(std::vector<int>{20, 10, 20});
int rank = compressed[0];  // 1
int value = compressed.cmp[rank];  // 20
```
{% endraw %}

## 検証

`verify/local/HeaderMigration.local.cpp`で空入力・重複を含む圧縮と順位を検証します。
