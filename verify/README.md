# 検証コード

各検証用 `.cpp` は対応するヘッダを include する独立したプログラムです。
`PROBLEM` があるファイルは、記載した問題の入出力形式で実行します。
`PROBLEM` がないファイルはローカルの assert による検証です。コンパイル時に
`NDEBUG` を定義しないでください。先頭の `// verification-helper: IGNORE` により
`oj-verify run` からは除外し、個別にコンパイル・実行します。

例（リポジトリのルートから）:

```sh
g++ -std=c++23 -O2 verify/EulerTour.test.cpp -o /tmp/euler-tour-test
/tmp/euler-tour-test
```

ヘッダ化済みの本体は `data_structure/`・`graph/`・`math/`・`string/`・`utility/`・`geometry/` に置いています。
その他のルート直下の `.cpp` は未移行です。
`template.cpp` と `Simple_template.cpp` は提出用テンプレートとして維持しています。

`HeaderMigration.test.cpp` は移行した13ヘッダの境界ケースと単純解比較を行います。
篩、圧縮、Doubling、Trie、LowLink、LCA、ポテンシャル付きUF、
行列、RLE、RollingHash、累乗、乱数範囲、時間計測を検証します。

追加のローカル検証:

- `DSUAndFolds.test.cpp`: DSU2種の単純解比較、20万頂点の鎖、非可換モノイドによるFolds/DSTの検証。
- `Combination.test.cpp`: ACL非依存のテスト型で、Pascalの三角形・積による単純解と比較。
- `AtCoderCombination.test.cpp`: 外部ACLの静的・動的modintで検証。

ACL向けヘッダ・検証コードをコンパイルするときは、外部に配置したACLへの
includeパスを指定してください（ACL本体はこのリポジトリに同梱しません）。

```sh
g++ -std=c++23 -O2 -I /path/to/ac-library verify/AtCoderCombination.test.cpp -o /tmp/combination-test
/tmp/combination-test
```

ローカルで確認したACLは `<atcoder/modint>` のみでも複数翻訳単位で
`floor_sum_unsigned` が多重定義になるため、ACL向け検証は1翻訳単位で実行しています。
汎用版 `Combination.hpp` はこの制限を持ちません。

`MultipleSort.test.cpp` は先頭配列での比較、複数配列との対応、空入力、長さ不一致を検証します。

`TreeAlgorithms.test.cpp` はHLDの頂点番号列・途中停止と、LCA/距離/移動、
全方位木DPの全ての根・部分木クエリを単純解と比較します。

`Fraction.test.cpp` はNaNの順序・ハッシュと四則演算、`FastPrimeFact.test.cpp` は
素数判定・素因数分解の単純解比較と64bit境界付近の回帰例を検証します。

Library Checker向けの検証:

- `PrimalityTest.test.cpp`: `FastPrimeFact.hpp` の素数判定（素因数分解はローカル検証のみ）。
- `BinomialCoefficientPrimeMod.test.cpp`: ACL非依存の動的modintで `Combination` の二項係数を検証。最大nをMとして時間・空間O(M + T)。
- `VertexAddPathSum.test.cpp`: HLDとFenwickTreeによる点加算・頂点パス和。前処理O(N log N)、点加算O(log N)、パス和O(log² N)、空間O(N)。
- `FenwickTree.test.cpp`: 点加算・区間和。
- `DisjointSparseTable.test.cpp`: 静的区間最小値。

AtCoder向けの `Mo.test.cpp` と `RangeDistinctCount.cpp` は手動提出・使い方の例示用に
残し、`oj-verify` の対象から除外します。`PROBLEM` は対応する問題の参照用です。
手動提出で確認した場合は、提出URLを各ファイルにコメントで記録します。
