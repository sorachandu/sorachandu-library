#pragma once

#include <cassert>
#include <vector>

namespace sora {

// 両関数とも n >= 0。時間 O(n log log(n+2))、空間 O(n)。
// エラトステネスの篩, 1以上N以下の整数について素数かどうか判定する
inline std::vector<bool> eratosthenes(long long n) {
    assert(0 <= n);
    std::vector<bool> isprime(n + 1, true);
    isprime[0] = false;
    if (n >= 1) isprime[1] = false;

    for (long long p = 2; p <= n / p; p++) {
        if (!isprime[p]) continue;
        for (long long q = p * p; q <= n; q += p) {
            isprime[q] = false;
        }
    }
    return isprime;
}

// 1以上N以下の素数を格納したvectorを返す,Eratosthenes関数と併せて使う
inline std::vector<long long> get_primes(long long n){
    std::vector sieve = eratosthenes(n);
    std::vector<long long> primes;
    for(long long i = 2; i <= n; i++){
        if(sieve[i]) primes.emplace_back(i);
    }
    return primes;
}

} // namespace sora
