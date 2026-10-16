# ヘッダ移行の状況

新規ヘッダは `#pragma once` と必要な標準ヘッダを持ち、`sora` 名前空間に配置しています。
ファイル・クラス名はPascalCase、関数名はsnake_caseとします。
提出用の `template.cpp` と `Simple_template.cpp` は維持しています。
`snippets/cpp.json` は変更していません。今回変更したAPIとの同期は別途行う必要があります。

## 今回移行したもの

| ヘッダ | 主なAPI・変更 | 時間計算量 | 保持空間 |
|---|---|---|---|
| `data_structure/Compression.hpp` | `Compression<T>`。元配列は変更しない | 構築 O(N log N)、添字取得 O(1)、値検索 O(log N) | O(N) |
| `data_structure/Doubling.hpp` | `Doubling<T>`。最大遷移回数0も可 | 構築 O(NL)、クエリ O(L)。L = max(1, bit_width(K)) | O(NL) |
| `data_structure/PotentialUF.hpp` | `PotentialUF<T>`。既存APIを維持 | 構築 O(N)、基本操作は償却 O(α(N))、groups は O(N α(N)) | O(N) |
| `graph/LowestCommonAncestor.hpp` | クラス名にファイル名を統一。buildで根を変えて再構築可 | 構築 O(N log N)、LCA関連 O(log N)、親・深さ O(1) | O(N log N) |
| `graph/LowLink.hpp` | 単純無向グラフ用。入力参照の寿命に注意 | 構築 O(V+E) | 入力を除き O(V) |
| `math/Eratosthenes.hpp` | `eratosthenes(n)`、`get_primes(n)`。n=0も可 | O(N log log(N+2)) | O(N) |
| `math/Pow.hpp` | `Pow` → `pow`。0^0=1、整数の中間積が収まること | O(log(K+1)) | O(1) |
| `math/Matrix.hpp` | `I` → `identity`、公開配列 `A` → `a`、`UNIT` → `unit` | 和差 O(NM)、N×P と P×M の積 O(NPM)、正方行列累乗 O(N³ log(K+1))、行列式 O(N³) | O(NM) |
| `string/Trie.hpp` | `countWord` → `count_word` など。対象は小文字英字 | 文字列長をLとして基本操作 O(L)、kth は答えの長さに比例 | 作成したノード数に比例 |
| `string/RunLengthEncoding.hpp` | `RunLengthEncoding` → `run_length_encoding`。空入力も可 | O(N) | 出力 O(N) |
| `string/RollingHash.hpp` | `rolling_hash` → `RollingHash`。静的データをinline化、空文字列も可 | 構築 O(N)、ハッシュ取得・連結 O(1) | O(N) |
| `utility/RangeRandom.hpp` | `Random_Gen` → `RangeRandom`。閉区間の整数乱数 | 生成は期待 O(1)、初期化は乱数源に依存 | O(1) |
| `utility/Timer.hpp` | `Timer`。経過時間向けにsteady_clockを使用 | O(1) | O(1) |
| `data_structure/DSU.hpp` | 単純版の `DSU`。u側の根へ併合する | 構築 O(N)、各操作は最悪 O(N) | O(N) |
| `data_structure/DSUChoose.hpp` | 代表元選択版を `DSUChoose` に改名 | 構築 O(N)、基本操作は償却 O(α(N))。mergeには選択関数の時間を加算 | O(N) |
| `math/Monoid.hpp` | DSTとFoldsの `M::T/op/e()` を共有 | 実行時処理なし | 実行時領域なし |
| `data_structure/Folds.hpp` | `folds` → `Folds`。基本名は0-indexed、旧仕様は `_one_indexed` | 構築 O(N)、各クエリ O(1) | O(N) |
| `math/Combination.hpp` | `Combination<ModInt>`。自作modintを使用でき、ACL非依存 | 構築 O(N)、各クエリ O(1) | O(N) |
| `math/AtCoderCombination.hpp` | `AtCoderCombination<ModInt>`。外部ACLをincludeする型制約付き別名 | 本体と同じ | 本体と同じ |
| `geometry/Geometry.hpp` | 未完成の幾何実装を挙動を維持して移植。Dot/Cross等はsnake_case化 | 各操作 O(1) | 各操作 O(1) |
| `utility/MultipleSort.hpp` | `multiple_sort(comp, first, rest...)`。先頭配列のみで比較し、全配列を変更する | K配列・長さNに対し O(K + N log(N+1) + KN) | O(N) |
| `graph/HeavyLightDecomposition.hpp` | `heavy_path` を頂点番号列へ修正。関数名をsnake_case化 | 構築 O(N)、祖先・距離・移動 O(log N)、パス列挙 O(K) | O(N)、列挙の出力 O(K) |
| `graph/RerootingDP.hpp` | `sora::rerootingdp<V,E>`。APIを維持しコメントを修正 | 構築 O(N)、全体クエリ O(1)、部分木クエリ O(log N) | O(N) |
| `math/Fraction.hpp` | `Fraction`・`FractionHash`。NaNを有限値の後ろへ配置。128bit中間値で約分 | 比較 O(1)、構築・四則演算 O(log M) | O(1) |
| `math/FastPrimeFact.hpp` | `is_prime`・`prime_factorize`。正の入力をassertし、Pollardの不要な加算を除去 | 素数判定 O(log N)、分解はPollardの探索に依存 | 分解・出力 O(log N) |





型の比較・コピー・四則演算は O(1) として記載しています。
行列式は体上の除算を前提にしており、整数除算には対応しません。
RollingHashは全インスタンスで基数を共有し、ハッシュ衝突の可能性があります。
LowLinkは再帰実装です。長いパスでのスタック消費は従来どおり残ります。

## 追加で確定したAPI

FoldsのモノイドはDSTと同じ `T`・`op`・`e()` です。

```cpp
sora::Folds<Monoid> folds(values);
folds.fold_without_range(l, r);             // 0-indexed、半開区間 [l, r)
folds.fold_without1(i);                     // 0-indexed、要素 i
folds.fold_without_range_one_indexed(l, r); // 1-indexed、閉区間 [l, r]
folds.fold_without1_one_indexed(i);         // 1-indexed、要素 i
```

Combinationは必要な演算をConceptで制約した汎用版を本体とします。
自作modintのほか、別途ACLをincludeすれば同じ `Combination<ModInt>` にACL型も渡せます。
ACL型だけに制約する場合は `AtCoderCombination.hpp` を使います。

```cpp
#include "math/AtCoderCombination.hpp"
sora::AtCoderCombination<atcoder::modint998244353> comb(100);
auto value = comb.comb(10, 3); // 戻り値は atcoder::modint998244353
```

素数modと `0 <= N < mod` が前提です。動的modintの場合も使えますが、
本オブジェクトの利用中にmodを変更してはいけません。N=0にも対応しています。

Geometryは利用頻度が低い未完成コードとして、そのまま移植しています。
`get_distance_sto_p` は射影が線分内にある場合に符号付き値を返し、
長さ0の線分には対応しません。誤差付き等値比較と厳密な大小比較も元のままです。
`Dot` → `dot`、`Cross` → `cross`、`getDistanceStoP` → `get_distance_sto_p` とし、
位置関係の定数名もsnake_caseにしました。未使用の `Equals` マクロは除去しています。

MultipleSortはユーザー指定どおり先頭配列だけで比較します。元の個別cppにあった
tuple比較は採用せず、template内の版と比較対象を統一しました。
同じキー同士の順序は未規定で、戻り値は並べ替え後の各要素に対応する元の添字です。
複数配列を辞書順で比較するオプションは追加していません。
`Vector_functions.cpp` はユーザーがスニペットへ移動・削除済みです。

## FractionとFastPrimeFactの確定仕様

両方ともヘッダへ移行済みです。残っているルートのcppは提出用テンプレートです。

- Fraction: 全てのNaN（未初期化を含む）を同値とし、有限値より後ろへ置く。
  分母0・0除算はNaN。LLONG_MINは対象外。
  比較には__int128、四則演算には__int128で計算・約分してから64bitへ格納する。
  約分後の分子・分母がlong long（LLONG_MINを除く）に収まることを前提としてassertする。
  Mは中間値の絶対値の最大。追加の任意精度型への依存はない。
- FastPrimeFact: prime_factorizeはN>=1をassertする。1の結果は空配列。
  Pollard法の差の計算は64bitのままで、`gcd(y-x+N,N)` を `gcd(y-x,N)` に修正。
  x/yは[0,N)内に保つ。剰余乗算に元から使っていた__int128は維持する。
- 改名: `frac` → `Fraction`、`FracHash` → `FractionHash`、
  `isNaN` → `is_nan`、`convert1D` → `convert_1d`。
  FastPrimeFactの補助関数はdetail名前空間へ隔離する。

## HLDとrerootingDPの確認結果

### HeavyLightDecomposition

旧cppの `heavyPath` には頂点番号とHLD順の添字の混在による不具合がありました。
例えば根0、辺 `0-1, 0-2, 2-3, 3-4` の木では、重いパスは `0,2,3,4`、
頂点1のHLD順の添字は4になります。旧実装は次を返しました。

```text
heavyPath(0) = [0, 2, 3, 4]
heavyPath(1) = [4]
```

前者は頂点番号としての列、後者はHLD添字であり、どちらかに統一されていません。
原因は `idx = id[head[v]]` でHLD添字から開始し、その後 `heavyC(idx)` に
頂点番号として渡していることです。`stopV` の比較も同様です。
`heavy_path` は `head[v]` から開始するよう修正し、頂点番号列を返します。
`heavy_path(v, true)` はパス先頭からvまで、falseならパス末尾までを返します。

一方、`LCA/dist/jump` は1〜30頂点のランダム木・異なる根について単純解と一致しました。
移行後も非0の根や軽いパスを含め、全頂点のパス列・途中停止を検証しました。
`LCA/LA/heavyR/heavyC/toSeq/toVertex` などは
`lca/la/heavy_root/heavy_child/to_seq/to_vertex` へ変更しています。

### rerootingDP

現在のアルゴリズムに具体的な不具合を確認したわけではありません。
以下の契約に合わせて日本語コメントを修正し、`graph/RerootingDP.hpp` へ移行しました。
関数名・コールバック・クエリAPIは維持しています（名前空間はsora）。
元の著作権・ライセンス表記は維持しています。

- `E`: 隣接する部分木からの寄与を合併する型。`op: E×E→E`、`e: E`。
- `V`: 頂点で確定したDP値。`put_vertex(E, 頂点番号)→V`。
- `put_edge(V, 辺ID)→E`。
- `g[v]` の要素 `(c, eid)` では、子cの値をvへ取り込む際に `put_edge(child_value, eid)` を呼ぶ。
  逆側の伝播には `~eid` を渡す。辺の両方向のIDは互いにビット反転であること。
- 戻り値 `dp(v)` はvを根とした木全体のDP値。
- `dp(r, v)` はrを根としたときのvの部分木のDP値。
- 入力は空でない連結な木。`E` の演算は可換モノイドであること。

`V` と `E` を別の構造体にし、向きによって異なる辺重みを使って、
1〜30頂点のランダム木で全ての根・部分木のクエリを単純な木DPと比較しました。
ASan/UBSanを含めて一致しています。ただし一般的な正しさの証明の代わりではありません。
各操作とコピーをO(1)として、前処理時間・空間O(N)、`dp(v)` はO(1)、
`dp(r,v)` は最悪O(log N)です。

HLD・rerootingDPとも再帰深さは最悪O(N)で、長いパスではスタック消費が残ります。
両者の元cppはヘッダへ移し、`verify/TreeAlgorithms.test.cpp` にローカル検証を追加しました。

## 検証

- 全29ヘッダの単独・二重includeを確認（ACL向けには外部ACLのincludeパスを指定）。
- ACL非依存の28ヘッダは全ヘッダ同時include・複数翻訳単位でのリンクを確認。
- ACL向けヘッダは1翻訳単位で検証。ローカルの外部ACLは内部関数
  `floor_sum_unsigned` がinlineでなく、2翻訳単位からincludeすると多重定義で失敗する。
  `<atcoder/modint>` だけでも同じ問題を再現した。外部ACLは変更していない。
- C++23、`-Wall -Wextra -Werror` で確認。
- `verify/HeaderMigration.test.cpp`・`DSUAndFolds.test.cpp`・`Combination.test.cpp` に境界ケースと単純解比較を用意。
- `verify/AtCoderCombination.test.cpp` は外部ACLの静的・動的modintを使って検証。
- AddressSanitizer・UndefinedBehaviorSanitizerでローカル検証。
  実行環境の制約でLeakSanitizerは無効化。
- Geometryの基本操作・既存の符号付き距離、MultipleSortの並べ替え・添字対応・長さ不一致をローカル検証。
- オンラインジャッジへの提出・oj-verifyの実行は未実施。

Fraction・高速素因数分解の具体的な確認結果は [FractionAndPrimeReview.md](FractionAndPrimeReview.md) を参照してください。

Fraction/FastPrimeFactの回帰例・単純解比較は `verify/Fraction.test.cpp` と `verify/FastPrimeFact.test.cpp` にあります。
