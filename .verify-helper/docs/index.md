# sorachandu-library

競技プログラミング用のC++ライブラリです。

このサイトの ✔ はverification-helperによる公式問題の検証成功、⚠ は対応する公式検証がないことを示します。
なお、 ✔ はヘッダの全機能が検証済みであることを意味しません。例えば `FastPrimeFact.hpp` の公式検証は素数判定が対象です。

assert・ランダムテストは別途、GitHub Actionsの `Run local tests` で実行しています。
ローカルテストの成否はこのサイトのアイコンには反映しません。

- [CIの実行結果](https://github.com/sorachandu/sorachandu-library/actions/workflows/verify.yml)
- [検証の実行方法と対象](https://github.com/sorachandu/sorachandu-library/blob/main/verify/README.md)
- [test.cpp が無いものの使用例（/examples）](https://github.com/sorachandu/sorachandu-library/tree/main/examples)

## 使い方から探す

各ページにAPI・計算量・前提条件・使用例・検証範囲をまとめています。
添字は原則0-indexed、区間は半開区間です。例外は各ページで明記します。

### 区間クエリ・集合・遷移

- [FenwickTree：点加算・区間和]({{ '/data_structure/FenwickTree.hpp.html' | relative_url }})
- [DisjointSparseTable：静的区間積]({{ '/data_structure/DisjointSparseTable.hpp.html' | relative_url }})
- [Folds：左右累積積]({{ '/data_structure/Folds.hpp.html' | relative_url }})
- [IntervalSet：整数区間の集合]({{ '/data_structure/IntervalSet.hpp.html' | relative_url }})
- [DynamicPointAddRectangleSum：オンライン点加算・長方形和]({{ '/data_structure/DynamicPointAddRectangleSum.hpp.html' | relative_url }})
- [RangeDistinctCount：区間・値域内の種類数]({{ '/data_structure/RangeDistinctCount.hpp.html' | relative_url }})
- [Doubling：遷移関数の繰り返し]({{ '/data_structure/Doubling.hpp.html' | relative_url }})
- [Mo：オフライン区間クエリ]({{ '/utility/Mo.hpp.html' | relative_url }})

### 連結性・グラフ

- [DSU：併合方向を固定するUnion-Find]({{ '/data_structure/DSU.hpp.html' | relative_url }})
- [DSUChoose：論理代表元を選べるUnion-Find]({{ '/data_structure/DSUChoose.hpp.html' | relative_url }})
- [PotentialUF：ポテンシャル付きUnion-Find]({{ '/data_structure/PotentialUF.hpp.html' | relative_url }})
- [SCC：強連結成分分解]({{ '/graph/SCC.hpp.html' | relative_url }})
- [LowLink：関節点・橋の列挙]({{ '/graph/LowLink.hpp.html' | relative_url }})

### 木

- [EulerTour：木の入退場時刻]({{ '/graph/EulerTour.hpp.html' | relative_url }})
- [HeavyLightDecomposition：重軽分解]({{ '/graph/HeavyLightDecomposition.hpp.html' | relative_url }})
- [LowestCommonAncestor：二分累乗によるLCA]({{ '/graph/LowestCommonAncestor.hpp.html' | relative_url }})
- [RerootingDP：全方位木DP]({{ '/graph/RerootingDP.hpp.html' | relative_url }})

### 文字列・配列の変換と検索

- [Compression：座標圧縮]({{ '/data_structure/Compression.hpp.html' | relative_url }})
- [RunLengthEncoding：連続要素の圧縮]({{ '/utility/RunLengthEncoding.hpp.html' | relative_url }})
- [MultipleSort：複数配列の同時ソート]({{ '/utility/MultipleSort.hpp.html' | relative_url }})
- [Trie：小文字英字の多重集合]({{ '/string/Trie.hpp.html' | relative_url }})
- [RollingHash：文字列の部分ハッシュ]({{ '/string/RollingHash.hpp.html' | relative_url }})

### 数学

- [Monoid：モノイドのConcept]({{ '/math/Monoid.hpp.html' | relative_url }})
- [Combination：素数法の階乗・二項係数]({{ '/math/Combination.hpp.html' | relative_url }})
- [AtCoderCombination：ACL modint用の別名]({{ '/math/AtCoderCombination.hpp.html' | relative_url }})
- [Eratosthenes：素数の篩]({{ '/math/Eratosthenes.hpp.html' | relative_url }})
- [FastPrimeFact：64bit素数判定・素因数分解]({{ '/math/FastPrimeFact.hpp.html' | relative_url }})
- [Pow：整数の二分累乗]({{ '/math/Pow.hpp.html' | relative_url }})
- [Matrix：行列演算]({{ '/math/Matrix.hpp.html' | relative_url }})
- [Fraction：既約分数]({{ '/math/Fraction.hpp.html' | relative_url }})

### 幾何・実行補助

- [Geometry2D：整数のCCW・線分交差・凸包・多角形、型を選べる距離・交点]({{ '/geometry/Geometry2D.hpp.html' | relative_url }})

- [ArgumentSort：整数の外積による偏角ソート]({{ '/geometry/ArgumentSort.hpp.html' | relative_url }})

- [RangeRandom：範囲付き整数乱数]({{ '/utility/RangeRandom.hpp.html' | relative_url }})
- [Timer：経過時間の計測]({{ '/utility/Timer.hpp.html' | relative_url }})

## ドキュメントの更新

説明の原稿は各ヘッダと同じディレクトリにある同名のMarkdownです。
`documentation_of`でヘッダに対応付け、`oj-verify docs`でソース・検証情報と統合します。
フォルダは大まかな分類とし、複数の用途から探したい場合はこの索引で補います。
