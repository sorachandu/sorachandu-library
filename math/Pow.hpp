#pragma once

#include <cassert>

namespace sora {

// 二分累乗で a^b を返す。時間 O(log(b+1))、空間 O(1)。
// b >= 0、計算中の積が long long に収まること。0^0 は 1。
inline long long pow(long long a,long long b){
    assert(b>=0);
    if(a==0 and b==0) return 1;
    if(a==1) return 1;
    if(a==-1) return (b&1)?-1:1;
    long long res=1;
    while(b){
        if(b&1) res*=a;
        b>>=1;
        if(b) a*=a;
    }
    return res;
}

} // namespace sora
