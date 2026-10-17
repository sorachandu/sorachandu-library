---
title: "MultipleSort — 複数配列の同時ソート"
documentation_of: ./MultipleSort.hpp
---

## 概要

先頭配列をキーとして複数配列を同じ順に並べ替えます。入力配列を変更し、並べ替え後の各位置に対応する元の添字を返します。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `multiple_sort(comp, first, rest...)` | キー順に全配列を並べ替え | O(K + N log(N + 1) + KN) |

## 仕様・使い方

Kは配列数、Nは共通の長さです。比較・コピーをO(1)とし、追加空間はO(N)です。配列はコピー・添字アクセス・代入が可能である必要があります。長さが異なるとstd::invalid_argumentを投げます。配列同士は別オブジェクトとし、同一配列を複数回渡さないでください。等しいキー同士の順序は保証しません。

## 使用例

{% raw %}
```cpp
#include "utility/MultipleSort.hpp"

#include <functional>
#include <vector>
std::vector<int> keys{30, 10, 20};
std::vector<char> values{'c', 'a', 'b'};
auto indices = sora::multiple_sort(std::less<int>{}, keys, values);
// keys: 10, 20, 30 / values: a, b, c / indices: 1, 2, 0
```
{% endraw %}

## 検証

`verify/local/MultipleSort.local.cpp`で要素の対応・空入力・長さ不一致を検証します。
