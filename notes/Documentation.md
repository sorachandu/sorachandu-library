# ライブラリドキュメントの執筆

ヘッダは競技中に仕様を確認するための短い説明、Pagesは使い方・設計を理解するための説明として使います。
アルゴリズム自体の修正と、コメント・ドキュメントの変更は区別して確認します。

## ファイルの置き場所

ヘッダと同じディレクトリに同じ名前のMarkdownを作ります。
たとえば`graph/SCC.hpp`には`graph/SCC.md`を添えます。

```yaml
---
title: "SCC — 強連結成分分解"
documentation_of: ./SCC.hpp
---
```

verification-helperが説明をヘッダのページに統合します。
生成先は`.verify-helper/markdown/graph/SCC.hpp.md`、Pages上では`graph/SCC.hpp.html`です。
`.verify-helper/markdown/`の生成物は直接編集しません。

## 記載する内容

- 概要: 何に使えるか。必要に応じてアルゴリズム・類似手法との使い分け。
- API・計算量: 引数・戻り値と時間計算量。N等の意味・演算コスト・空間計算量も説明。
- 仕様: 型の要件、添字・区間、前提条件、入力の破壊、空入力・重複・範囲外の扱い。
- 使用例: 実際のincludeパスと公開APIを使う短いC++コード。C++の二重波括弧をLiquidとして解釈させないよう、コードブロックをrawタグで囲む。
- 検証: 公式問題・ローカルテスト・手動使用例を区別し、検証対象と未検証部分を記載。

ヘッダには前提条件・特殊な挙動・計算量を残し、長い使用例や解説を重複させません。
内部コメントでは、実装上の理由や不変条件を残します。著作権・ライセンス・出典は保持します。

## 索引とリンク

新しいライブラリは`.verify-helper/docs/index.md`の用途別索引にも追加します。
フォルダの分類だけで探しづらい機能は、用途に応じて複数の項目から案内して構いません。
Pagesのリンクにはリポジトリ名を固定せず、次の形式を使います。

{% raw %}
```liquid
[SCC]({{ '/graph/SCC.hpp.html' | relative_url }})
```
{% endraw %}

GitHub上のREADMEから原稿へリンクする場合は、通常の相対パスを使います。

## 確認

```sh
oj-verify docs
```

生成されたヘッダページに説明が入り、front matterの`documentation_of`が正しいことを確認します。
使用例は必要なincludeを付けてC++23でコンパイルし、外部依存が必要な例はその条件を明記します。
Jekyllが利用できる環境ではCIと同じ形でHTMLを確認できます。

```sh
jekyll build --source .verify-helper/markdown --destination /tmp/sora-library-site --baseurl /sorachandu-library
```

このコマンドはローカルのHTMLを生成します。公開は既存のPages用CIが担当します。
