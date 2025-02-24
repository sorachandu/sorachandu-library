#include<bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned ll;
#include<atcoder/fenwicktree>
using namespace atcoder;
#define rep(i,x,lim) for(ll i = ll(x); i < ll(lim); i++)
#define all(x) (x).begin(), (x).end()

// 座標圧縮
template<typename T> inline vector<uint> compression(vector<T> &a){
    uint n = uint(a.size());
    vector<uint> res(n);
    vector b = a;
    sort(all(b));
    b.erase(unique(all(b)),b.end());
    rep(i,0,n){
        auto iter = lower_bound(all(b),a[i]);
        res[i] = iter - b.begin();
    }
    return res;
}

// 転倒数をO(NlogN)で求める
template<typename T> inline ull Num_of_Inversions(const vector<T>& v){
    ull n = ull(v.size());
    vector V = compression(v);
    ull N = *max_element(all(V));
    fenwick_tree<ull> fw(N+1);
    ull ans = 0;
    rep(i,0,n){
        ans += fw.sum(V[i]+1, N+1);
        fw.add(V[i], 1);
    }
    return ans;
}

int main(){
    int n;
    vector<ll> a(n);
    rep(i,0,n) cin >> a[i];

    cout << Num_of_Inversions(a) << '\n';
}