
```cpp
// ボール 箱 区別
int main(){
    cin.tie(nullptr)->ios::sync_with_stdio(false);
    int N,K;
    cin>>N>>K;
    cout<<mint::raw(K).pow(N)<<"\n";
}
```

```cpp
int main(){
    cin.tie(nullptr)->ios::sync_with_stdio(false);
    /*//--------------------------------------------------------
    ボール,箱ともに区別できるパターンは、
    人iをチームjに入れる方法と考えると直感的
    https://qiita.com/drken/items/f2ea4b58b0d21621bd51
    ボール区別,箱区別,1個以上: 包除原理　空にする箱の個数を固定
    *///--------------------------------------------------------
    int N,K;
    cin>>N>>K;
    const int siz=1010;
    Combination<mint> comb(siz);
    mint ans;
    for(int i=0;i<=K;i++){
        ans+=(((K-i)&1)?-1:1)*comb.comb(K,i)*mint(i).pow(N);
    }
    cout<<ans<<"\n";
}
```

```cpp
int main(){
    cin.tie(nullptr)->ios::sync_with_stdio(false);
    /*//--------------------------------------------------------
    https://qiita.com/drken/items/f2ea4b58b0d21621bd51
    ボール区別しない,箱区別,制限なし: _{n+k-1}C_n 重複組合せ
    *///--------------------------------------------------------
    int N,K;
    cin>>N>>K;
    const int siz=2010;
    Combination<mint> comb(siz);
    mint ans=comb.comb(N+K-1,N);
    cout<<ans<<"\n";
}
```

```cpp
int main(){
    cin.tie(nullptr)->ios::sync_with_stdio(false);
    /*//--------------------------------------------------------
    https://qiita.com/drken/items/f2ea4b58b0d21621bd51
    ボール区別しない,箱区別,1個以内: _nC_k　使う箱を選ぶだけ
    *///--------------------------------------------------------
    int N,K;
    cin>>N>>K;
    const int siz=2010;
    Combination<mint> comb(siz);
    mint ans=comb.comb(K,N);
    cout<<ans<<"\n";
}
```

```cpp
int main(){
    cin.tie(nullptr)->ios::sync_with_stdio(false);
    /*//--------------------------------------------------------
    https://qiita.com/drken/items/f2ea4b58b0d21621bd51
    ボール区別しない,箱区別,1個以上:
    n個の玉の隙間n-1個にk-1個の仕切り, 仕切りの位置に重複なし　→ _{n-1}C_{k-1}
    *///--------------------------------------------------------
    int N,K;
    cin>>N>>K;
    const int siz=2010;
    Combination<mint> comb(siz);
    mint ans=comb.comb(N-1,K-1);
    cout<<ans<<"\n";
}
```