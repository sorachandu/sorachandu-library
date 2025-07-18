# gmpxx.hを見る会

GMPとかいう便利な数値もにょもにょライブラリがあるらしい. 多倍長整数とか有理数型があるらしい

文献を漁るのがだるいのでChat-GPT o4-miniに聞いた.

mpz_class用のpow関数(扱いやすくした版)を付録程度に置く

```cpp
// base^expをmpz_class型で計算
// 注意: baseはmpz_class型
inline mpz_class mpz_pow(mpz_class base, uint64_t Exp){
    mpz_class res;
    mpz_pow_ui(res.get_mpz_t(),base.get_mpz_t(),Exp);
    return res;
}
```


sorachandu: The GNU Multiple Precision Arithmetic Library (GMP) についての解説及びinclude方法を教えてもらえますか.

GitHub Copilot: GMP (GNU Multiple Precision Arithmetic Library) は任意精度の整数・有理数・浮動小数点演算をサポートする C ライブラリです。C++ から使う場合は C++ 用ラッパーである `<gmpxx.h>` を使い，`mpz_class`（大きな整数）や `mpq_class`（有理数），`mpf_class`（浮動小数）などを扱えます。

### インストール (Ubuntu/Debian 例)

```bash
sudo apt update
sudo apt install libgmp-dev           # C ヘッダ＋ライブラリ
```

### インクルード方法

```cpp
#include <bits/stdc++.h>
#include <gmpxx.h>   // ← これを追加
using namespace std;

int main(){
    mpz_class a, b, c, d, e, f;
    cin >> a >> b >> c >> d >> e >> f;
    cout << (a*b*c - d*e*f) % 998244353 << "\n";
    return 0;
}
```

### コンパイル

GMP の C++ ラッパー部と本体をリンクするために，以下のように `-lgmpxx -lgmp` を指定します。

```bash
g++ -std=c++17 free.cpp -o free -lgmpxx -lgmp
```

これで `mpz_class` をはじめとした GMP の多倍長演算型が使えるようになります。

sorachandu: mpz_classやmpq_class,mpf_classについての扱い方、特徴、メソッド関数などをより詳しく教えてもらえますか？

GitHub Copilot: 以下、C++ 用ラッパー `<gmpxx.h>` が提供する主な任意精度クラスと，よく使うコンストラクタ・演算・メソッドをまとめます。

---

## mpz_class（任意精度整数）

### 主な特徴

- 整数同士の加減乗除，剰余，ビット演算が可能
- 自動でメモリ管理（必要に応じて拡張／収縮）
- 標準演算子（+ - * / % & \| ^ << >>）に対応

### コンストラクタ例

````cpp
mpz_class x;              // 0
mpz_class a(12345);       // 整数から
mpz_class b("FF", 16);    // 文字列＋基数(16進数)から
mpz_class c = a;          // コピー
````

### 便利メソッド

````cpp
x.get_ui();               // unsigned long に変換
x.get_si();               // signed long に変換
x.get_str(base = 10);     // 文字列に変換（base：2～62）
x.set_str("101010", 2);   // 文字列＋基数から設定
void x += y;              // 代入付き加算（-=,*=,/=,%= なども同様）
mpz_pow_ui(x.get_mpz_t(), y.get_mpz_t(), 10);  
                          // C API と組み合わせて高速累乗も可
````

### 使用例

````cpp
mpz_class x = 2, y = 100;
mpz_class z = 1;
z = pow(x, y.convert_to<unsigned long>()); // pow は注意が必要
// → gmp_pow_ui(x.get_mpz_t(), y_ui) を使うほうが安全
````

---

## mpq_class（任意精度有理数）

### 主な特徴

- 分子・分母を自動で既約化
- 分数同士の四則演算が自然に扱える

### 注意点[記:sorachandu] 

- いうほど自動で既約化されてるのか不安かも　canonicalize()をよく噛まそう
- (3/-2)みたいなものは「0より大きい」判定されるらしい(!?) → 既約化すれば安心

### コンストラクタ例

````cpp
mpq_class r;                  // 0/1
mpq_class r1(3, 4);           // 3/4
mpq_class r2("22/7");         // 文字列から
````

### 便利メソッド

````cpp
r.get_num();                  // 分子(mpz_class を返す)
r.get_den();                  // 分母(mpz_class を返す)
r.get_str();                  // "numerator/denominator"
r.canonicalize();             // 明示的に既約化
````

### 使用例

````cpp
mpq_class a(1,3), b(1,6);
mp
````

---

[回答止まったのでregenerate]

GMP の C++ ラッパークラスには主に以下の 3 つがあります。  

1. `mpz_class` … 任意精度整数  
2. `mpq_class` … 任意精度有理数（分数）  
3. `mpf_class` … 任意精度浮動小数点  

---  
## 1. mpz_class（整数）  
– 内部は符号付き・任意長の整数。  
– 算術演算子（+, -, *, /, %, += …）がオーバーロード済。  
– コンストラクタ  
  - `mpz_class x;`              // 0  
  - `mpz_class x = 12345;`      // 整数リテラルから  
  - `mpz_class x("FF",16);`     // 文字列＋基数指定  
– 文字列表現  
  - `x.get_str(base)` → `std::string`  
  - `x.set_str("101101",2)`    // 2 進文字列を再設定  
– 主なメソッド／関数  
  - `x += y; x *= y; x %= y;`  
  - `x.pow_ui(u)`              // x^u (`unsigned long`)  
  - `bit_test(i)`, `bit_set(i)`, `bit_clear(i)`  
  - `abs(x)`, `gcd(x,y)`, `lcm(x,y)`（フリー関数）  
  - `invert(x, mod)`            // 逆元 (x⁻¹ mod mod)  
  - `fac_ui(n)`                 // n! を返す（`n` は unsigned）  

例：  
````cpp
#include <gmpxx.h>
mpz_class a = 123, b("FF",16);
mpz_class c = a * b + 1;
std::string s = c.get_str(10);   // 10 進文字列化
mpz_class g = gcd(a, b);
mpz_class inv;
invert(inv, a, b);               // inv ≡ a⁻¹ mod b
````

---  
## 2. mpq_class（有理数）  

- 分母・分子を `mpz_class` で管理し，自動で約分・符号整理される。  
- コンストラクタ  
  - `mpq_class q;`                // 0/1  
  - `mpq_class q(3,4);`           // 3/4  
  - `mpq_class q("22/7");`        // 文字列から  
- 演算子は整数版と同様（+ – * /…）  
- メソッド  
  - `q.get_num()` → 分子 (`mpz_class`)  
  - `q.get_den()` → 分母  
  - `q.canonicalize()`            // 手動約分（通常不要）  
  - 文字列化は `q.get_str(base)` → `"num/den"`  

例：  

````cpp
#include <gmpxx.h>
mpq_class x(1,3), y("2/5");
mpq_class z = x + y;             // 11/15
auto p = z.get_num();            // 11
auto q = z.get_den();            // 15
std::string zs = z.get_str(10);  // "11/15"
````

---

## 3. mpf_class（浮動小数点）  

- 任意精度の浮動小数。デフォルト精度は 53 ビット（倍精度相当）。  
- コンストラクタ & 精度設定  
  - `mpf_class f;`                       // デフォルト精度  
  - `mpf_class f(0.1, 200);`             // 初期値＋200 ビット精度  
  - `mpf_class::default_prec(256);`      // 今後生成されるすべての mpf の精度設定  
- 演算子（+ – * /）  
- メソッド  
  - `f.get_prec()`                      // ビット単位の精度  
  - `f.set_prec(new_prec)`             // 精度変更  
  - `f.get_d()`                         // `double` に変換  
  - `f.get_str(base, digits)`           // 文字列化（基数＋桁数指定）  

例：  

````cpp
#include <gmpxx.h>
mpf_class::default_prec(128);
mpf_class a("3.1415926535"), b(2.0);
mpf_class c = a * b;                   // ≈ 6.28318...
std::string s = c.get_str(10, 20);     // 10 進 20 桁
double d = c.get_d();                  // double にキャスト
````

---

以上を組み合わせることで，大きな整数計算／厳密有理数計算／任意精度浮動小数点計算が容易に行えます。  
必要に応じて `<gmp.h>`（C API）も併用可能ですが，C++ ではラッパーを使うのが便利です。

---

sorachandu:(mpz_class).pow_uiは存在しないようです. 代わりに存在するmpz_pow_uiについて教えていただけますか.

GitHub Copilot:
以下のように GMP の C API 関数 `mpz_pow_ui` を使って大きな整数の累乗を計算できます。

―――――――――――――――――  
### シグネチャ  

```c
void mpz_pow_ui(mpz_t rop, const mpz_t base, unsigned long int exp);
```

- `rop`…計算結果を格納する `mpz_t`  
- `base`…底（べき乗される整数）  
- `exp` …指数（符号なし long）  
- 戻り値は void。`rop = base^exp` を計算するだけです。  

### C++ ラッパー (`mpz_class`) からの呼び出し  

`mpz_class` から生の `mpz_t` を取り出すにはメンバ関数 `get_mpz_t()` を使います。

例：ファイル free.cpp の修正例

````cpp
…existing code…
int main(){
    mpz_class a, b;
    cin >> a >> b;
    // a をそのまま出力
    cout << a.get_str(10) << "\n";
    // a = b^3 を計算
    mpz_pow_ui( a.get_mpz_t(),   // rop: 計算結果を a に
                b.get_mpz_t(),   // base: 底は b
                3 );              // exp: 3乗
    cout << a.get_str(10) << "\n";
}
…existing code…
````

### ポイント

- `mpz_class` のメンバ関数としては存在しない → 低レベル C API を直呼び出し
- `get_mpz_t()` で C スタイルのハンドル (`mpz_t`) を取り出す
- 計算結果は第 1 引数の `rop` に書き込まれる

このようにすれば任意精度の整数累乗が簡単に扱えます。
