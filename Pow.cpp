#include<bits/stdc++.h>
using namespace std;
using ll=long long;



// a^bを返す オーバーフローに注意
inline ll Pow(ll a,ll b){
    assert(b>=0);
    if(a==0 and b==0) return 1;
    if(a==1) return 1;
    if(a==-1) return (b&1)?-1:1;
    ll res=1;
    while(b){
        if(b&1) res*=a;
        b>>=1;
        if(b) a*=a;
    }
    return res;
}