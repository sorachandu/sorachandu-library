---
title: "Timer — 経過時間の計測"
documentation_of: ./Timer.hpp
---

## 概要

std::chrono::steady_clockを使い、構築時からの経過時間を秒で取得します。保持空間はO(1)です。

## API・計算量

| 操作 | 内容 | 時間 |
| --- | --- | --- |
| `Timer()` | 計測開始 | O(1) |
| `count()` | 経過秒数をdoubleで返す | O(1) |
| `is_under(x)` | 経過秒数がx未満か | O(1) |

## 仕様・使い方

単調時計による経過時間で、CPU時間ではありません。リセットAPIはなく、計測をやり直すなら新しいTimerを作ります。時計の分解能は実行環境に依存します。

## 使用例

{% raw %}
```cpp
#include "utility/Timer.hpp"

sora::Timer timer;
// 計測対象の処理
double elapsed = timer.count();
bool remaining = timer.is_under(1.8);
```
{% endraw %}

## 検証

`verify/local/HeaderMigration.local.cpp`で基本操作を検証します。厳密な計時精度の検証ではありません。
