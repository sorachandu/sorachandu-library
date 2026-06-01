https://algo-logic.info/range-dp/ より

区間DP
dp[l][r] := 区間 [ l, r ) について、最適な状況下での何かしらの値

- 区間 [ l, r ) を更新する際に、[ l+1, r ) と [ l, r-1 ) などの左右から1つ増減させたものを確認する
- 区間 [ l, r ) を更新する際に、[ l, i ) と [ i, r ) を全ての i について確認する

言い換えると

- dp[ l ][ r ] = dp[ l+1 ][ r ] と dp[ l ][ r-1 ] から更新（左端か右端の1つが変化する）
- dp[ l ][ r ] = 全ての i について dp[ l ][ i ] と dp[ i ][ r ] を確認して更新（2つの区間を組み合わせて新しいものを得る）

よくある適用可能な状況

区間の除去・圧縮・合体などが生じる時
ある区間 [l, r) について、最適に除去・圧縮・合体した時の値を dp[ l ][ r ] などと保持する場合が多い


$O(N^2)$ が間に合いそうなとき（N≤3000 前後）
一回の更新が定数時間でできる場合、このような制約になりがち
dp[ l ][ r ] = dp[ l+1 ][ r ] と dp[ l ][ r-1 ] から更新

$O(N^3)$ が間に合いそうなとき（N≤500 前後）
一回の更新が N に比例した回数でできる場合、このような制約になりがち
dp[ l ][ r ] = 全ての i について dp[ l ][ i ] と dp[ i ][ r ] を確認して更新

例題
AOJ Daruma Otoshi

問題概要
長さ $n$ の数列 ${w1,w2,w3,⋯,wn}$ が与えられ、以下の操作を好きなだけ行える。

数列の中から隣り合うペアを1つ選び、差が 1 以内になるなら除去する
最大でいくつ取り除くことができるか求めよ。

制約
- $1≤n≤300$
- $1≤wi≤1000$

考え方
問題設定から、区間DPの典型的な状況であると分かります。「区間の除去」という操作ですし、制約からいっても $O(n^3)$ 程度なら計算が間に合いそうです。

区間DPのテンプレートに沿って、以下のようなDPを考えてみましょう。

dp[l][r] := 区間 [ l, r ) について、取り除くことができる数の最大値

更新は、2通りの場合を考えればよいです。

wl,wr−1 が一緒に取り除かれる時：区間 [l+1, r-1) は全て取り除け、更に wl,wr−1 の差が 1以下の時のみ生じる
1. 以外の時：区間 [ l, i ) と 区間 [ i, r) に分けて考える
1つ目の方は、区間 [ l, r ) が全て取り除かれるので、全体で r-l 個取り除けることになります。

2つ目の方はほぼテンプレート通りで、区間 [ l, i ) と 区間 [ i, r) での値を組み合わせれば良く、

dp[l][r]=max(dp[l][i]+dp[i][r])
のようになります。

```cpp
// refer: https://algo-logic.info/educational-dp-contest-l/
#include<bits/stdc++.h>
using namespace std;
int main(){
    cin.tie(nullptr)->ios::sync_with_stdio(false);
    /*//--------------------------------------------------------
    先頭か末尾を削除できる　先手はX点、後手は-X点を得る
    区間DPで、dp[l][r]:=A[l:r]についてのX-Yの値
    *///--------------------------------------------------------
    int N;
    cin>>N;
    vector<int> A(N);
    for(auto &e:A) cin>>e;
    const long INF=1l<<60;
    vector dp(N+2,vector<long>(N+2,-INF));
    auto dfs=[&](auto dfs,int l,int r) -> long {
        if(dp[l][r]!=-INF) return dp[l][r];
        if(r<=l) return 0;
        bool isFirst=(N-(r-l))%2==0;
        long res;
        if(isFirst){
            res=-INF;
            res=max(dfs(dfs,l+1,r)+A[l],dfs(dfs,l,r-1)+A[r-1]);
        }else{
            res=INF;
            res=min(dfs(dfs,l+1,r)-A[l],dfs(dfs,l,r-1)-A[r-1]);
        }
        return dp[l][r]=res;
    };
    cout<<dfs(dfs,0,N)<<"\n";
}
```

```cpp
// refer: https://algo-logic.info/educational-dp-contest-n/
#include<bits/stdc++.h>
using namespace std;
int main(){
    cin.tie(nullptr)->ios::sync_with_stdio(false);
    /*//--------------------------------------------------------
    dp[l][r]:=[l,r)について、1つになるまで操作を行った時のコスト最小値
    [l,r)をmを用いて2つに分けて、それらをmergeする
    このとき加算されるコストはsum[l,r)に等しい
    [l,r)の全部を併合して1つにするので、そうなる
    *///--------------------------------------------------------
    int N;
    cin>>N;
    vector<int> A(N);
    for(auto &e:A) cin>>e;
    vector<long> ac(N+1);
    for(int i=0;i<N;i++) ac[i+1]=ac[i]+A[i];
    const long INF=1ll<<60;
    vector dp(N+1,vector<long>(N+1,INF));
    auto dfs=[&](auto dfs,int l,int r) -> long {
        if(dp[l][r]!=INF) return dp[l][r];
        if(r-l==1) return 0;
        long res=INF;
        for(int m=l+1;m<r;m++){
            res=min(res,dfs(dfs,l,m)+dfs(dfs,m,r)+ac[r]-ac[l]);
        }
        return dp[l][r]=res;
    };
    cout<<dfs(dfs,0,N)<<"\n";
}
```