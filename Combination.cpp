#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
/*------使用例-----------------------
    cominit();
    int w,h;
    cin >> w >> h;
    w--,h--;
    cout << com(w+h,min(w,h)) << el;
---------------------------------- */


// MAX, MODは適宜調整すべし
const int MAX = 510000;
const int MOD = 1e9+7;
ll fac[MAX], finv[MAX], inv[MAX];

// 前処理 main関数内で最初に呼ぶこと
void cominit(){
    fac[0] = fac[1] = 1;
    finv[0] = finv[1] = 1;
    inv[1] = 1;
    for(int i = 2; i < MAX; i++){
        fac[i] = fac[i-1] * i % MOD;
        inv[i] = MOD - inv[MOD%i] * (MOD/i) % MOD;
        finv[i] = finv[i-1] * inv[i] % MOD;
    }
}

// 二項係数をO(1)で求める
ll com(int n, int k){
    if(n < k) return 0;
    if(n < 0 || k < 0) return 0;
    return fac[n] * (finv[k] * finv[n-k] % MOD) % MOD;
}