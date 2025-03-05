#include <bits/stdc++.h>
#include <atcoder/modint>
using namespace std;
//------------------------------------

template<typename T> struct is_static_modint : false_type {};
template<int M, enable_if_t<(1<=M)>* Ptr>
  struct is_static_modint<atcoder::static_modint<M,Ptr>> : true_type {};
template<typename T> concept static_modint = is_static_modint<T>::value;

// nCkを任意の素数mod下で高速に計算する構造体
// atcoder::static_modint<M>をテンプレート引数に取る
// Tips: Combination<atcoder::modint998244353> comb(n)
template<static_modint mint> struct Combination{
    vector<mint> fac,finv,inv;
    
    Combination(int n) : fac(n+1), finv(n+1), inv(n+1) {
        constexpr int MOD = mint::mod();
        fac[0]=fac[1]=1;
        finv[0]=finv[1]=1;
        inv[1]=1;
        for(int i=2;i<=n;i++){
            fac[i]=fac[i-1]*i;
            inv[i]=MOD-inv[MOD%i]*(MOD/i);
            finv[i]=finv[i-1]*inv[i];
        }
    }

    // return nCk : O(1)
    uint64_t calc(int n, int k){
        if(n<k) return 0;
        if(n<0 or k<0) return 0;
        return (fac[n]*finv[k]*finv[n-k]).val();
    }
};

int main(){
    Combination<atcoder::static_modint<1000000009>> comb(30);
    cout<<comb.calc(10,8)<<'\n'<<comb.calc(5,3)<<endl;
    cout<<comb.calc(0,1)<<'\n';
}