// verification-helper: IGNORE
#include <algorithm>
#include <cassert>
#include <vector>

#include "../math/FastPrimeFact.hpp"

// 篩との比較、素数冪、大きな合成数でのオーバーフロー回帰を検証する。
int main() {
    std::vector<bool> sieve(100001,true);
    sieve[0]=sieve[1]=false;
    for(int p=2;p*p<=100000;++p)if(sieve[p])for(int q=p*p;q<=100000;q+=p)sieve[q]=false;
    for(int n=0;n<=100000;++n)assert(sora::is_prime(n)==sieve[n]);
    assert(!sora::is_prime(-1));
    assert(!sora::is_prime(341550071728321LL));
    assert(!sora::is_prime(3825123056546413051LL));
    for(long long n : {1LL,2LL,4LL,27LL,325LL,9375LL,600851475143LL,
                      9223372021822390277LL,9223372036854775807LL,2305843009213693951LL}) {
        const auto factors=sora::prime_factorize(n);
        assert(std::is_sorted(factors.begin(),factors.end()));
        __int128_t product=1;
        for(long long p:factors){assert(sora::is_prime(p));product*=p;}
        assert(product==n);
    }
    for(int n=1;n<=3000;++n) {
        std::vector<long long> expected;
        int x=n;
        for(int p=2;p<=x/p;++p)while(x%p==0){expected.push_back(p);x/=p;}
        if(x>1)expected.push_back(x);
        assert(sora::prime_factorize(n)==expected);
    }
}
