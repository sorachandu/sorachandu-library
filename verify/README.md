# 検証コード

検証は次の3種類に分けます。

- `verify/*.test.cpp`: Library Checkerの問題を使い、`oj-verify run` で実行。
- `verify/local/*.local.cpp`: assert・ランダムテストを専用スクリプトで実行。
- `examples/*.cpp`: AtCoderへの手動提出・使い方の例示用。

リポジトリのルートで次を実行します。

```sh
oj-verify run
python3 verify/RunLocalTests.py
```

CIでも公式検証とローカル検証を別ステップで実行し、どちらかが失敗するとCIも失敗します。
ローカル検証はC++23・assert有効で実行し、`CXX` でコンパイラを指定できます。
ACL依存テストは通常スキップし、次で追加実行できます。

```sh
python3 verify/RunLocalTests.py --acl /path/to/ac-library
```

ローカルテストを追加するときは `verify/local/PascalCase.local.cpp` に置きます。
各ファイルは独立した `main` を持ち、失敗時はassert等で異常終了させてください。
実行ファイルは一時ディレクトリに作成し、終了後に削除します。

サイトの ✔ はhelperの公式検証成功、⚠ はhelperで認識する検証なしを表します。
ローカル検証はhelperの状態に混ぜず、CIの `Run local tests` で確認します。
`verify/local` は生成ドキュメントから除外します。

ヘッダ化済みの本体は `data_structure/`・`graph/`・`math/`・`string/`・`utility/`・`geometry/` に置いています。
その他のルート直下の `.cpp` は未移行です。
`template.cpp` と `Simple_template.cpp` は提出用テンプレートとして維持しています。

`HeaderMigration.local.cpp` は移行した13ヘッダの境界ケースと単純解比較を行います。
篩、圧縮、Doubling、Trie、LowLink、LCA、ポテンシャル付きUF、
行列、RLE、RollingHash、累乗、乱数範囲、時間計測を検証します。

追加のローカル検証:

- `DSUAndFolds.local.cpp`: DSU2種の単純解比較、20万頂点の鎖、非可換モノイドによるFolds/DSTの検証。
- `Combination.local.cpp`: ACL非依存のテスト型で、Pascalの三角形・積による単純解と比較。
- `AtCoderCombination.local.cpp`: 外部ACLの静的・動的modintで検証（任意実行）。

ACL向けヘッダ・検証コードをコンパイルするときは、外部に配置したACLへの
includeパスを指定してください（ACL本体はこのリポジトリに同梱しません）。

```sh
g++ -std=c++23 -O2 -I /path/to/ac-library verify/local/AtCoderCombination.local.cpp -o /tmp/combination-test
/tmp/combination-test
```

ローカルで確認したACLは `<atcoder/modint>` のみでも複数翻訳単位で
`floor_sum_unsigned` が多重定義になるため、ACL向け検証は1翻訳単位で実行しています。
汎用版 `Combination.hpp` はこの制限を持ちません。

`MultipleSort.local.cpp` は先頭配列での比較、複数配列との対応、空入力、長さ不一致を検証します。

`TreeAlgorithms.local.cpp` はHLDの頂点番号列・途中停止と、LCA/距離/移動、
全方位木DPの全ての根・部分木クエリを単純解と比較します。

`Fraction.local.cpp` はNaNの順序・ハッシュと四則演算、`FastPrimeFact.local.cpp` は
素数判定・素因数分解の単純解比較と64bit境界付近の回帰例を検証します。

Library Checker向けの検証:

- `PrimalityTest.test.cpp`: `FastPrimeFact.hpp` の素数判定（素因数分解はローカル検証のみ）。
- `BinomialCoefficientPrimeMod.test.cpp`: ACL非依存の動的modintで `Combination` の二項係数を検証。最大nをMとして時間・空間O(M + T)。
- `VertexAddPathSum.test.cpp`: HLDとFenwickTreeによる点加算・頂点パス和。前処理O(N log N)、点加算O(log N)、パス和O(log² N)、空間O(N)。
- `FenwickTree.test.cpp`: 点加算・区間和。
- `DisjointSparseTable.test.cpp`: 静的区間最小値。

AtCoder向けの `examples/Mo.cpp` と `examples/RangeDistinctCount.cpp` は手動提出用です。
helperが検証ファイルとして認識しない名前にし、問題URLを参照用コメントとして残しています。
手動提出で確認した場合は、提出URLを各ファイルにコメントで記録します。

- `SCC.test.cpp`: 強連結成分分解と成分のトポロジカル順。
- `DynamicPointAddRectangleSum.test.cpp`: 座標の事前登録をしない点加算・長方形和。
- `AddedLibraries.local.cpp`: SCCの到達可能性・縮約グラフとの比較、20万頂点の鎖、
  IntervalSetの集合との比較と64bit端点、長方形和の全点走査とのランダム比較。
