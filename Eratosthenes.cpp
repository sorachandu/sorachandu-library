#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
//------------------------------------------------------------

// エラトステネスの篩, 1以上N以下の整数について素数かどうか判定する
vector<bool> Eratosthenes(ll N) {
    vector<bool> isprime(N + 1, true);
    isprime[0] = isprime[1] = false;

    for (int p = 2; p <= N; p++) {
        if (!isprime[p]) continue;
        for (int q = p * 2; q <= N; q += p) {
            isprime[q] = false;
        }
    }
    return isprime;
}

// 1以上N以下の素数を格納したvectorを返す,Eratosthenes関数と併せて使う
vector<ll> get_primes(ll N){
    vector eratosthenes = Eratosthenes(N);
    vector<ll> primes;
    for(ll i = 2; i <= N; i++){
        if(eratosthenes[i]) primes.emplace_back(i);
    }
    return primes;
}