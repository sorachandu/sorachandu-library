#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <vector>

namespace sora {

// [0, N) 上の遷移関数を二分累乗で処理する（単射でなくてもよい）。
// 最大遷移回数を K とし、時間・空間 O(N L)、クエリ O(L)。L = max(1, bit_width(K))。
// 入力は変更しない。T は整数の添字型、各遷移先は [0, N) であること。
template<typename T> class Doubling{
    using ull=unsigned long long;
    ull max_steps;
    unsigned int n, levels;
    std::vector<std::vector<T>> dp;
    
public:
    // 空の構造を作る。時間・空間 O(1)。query は呼べない。
    Doubling() : max_steps(0), n(0), levels(0) {}
    // 時間・空間 O(N L)。K = 0 にも対応。
    Doubling(const std::vector<T>& v, ull k = 1ull<<60) :
        max_steps(k), n(static_cast<unsigned int>(v.size())), levels(std::max(1U, static_cast<unsigned int>(std::bit_width(max_steps)))){
            for (T to : v) assert(to >= 0 && static_cast<unsigned long long>(to) < n);
            dp.resize(levels);
            for(unsigned int i=0;i<levels;i++) dp[i].resize(n);
            dp[0] = v;
            for(unsigned int i=1;i<levels;i++){
                for(unsigned int j=0;j<n;j++){
                    dp[i][j] = dp[i-1][dp[i-1][j]];
                }
            }
        }
    
    // 要素pからk回遷移した先の要素を返す。時間 O(L)。0 <= k <= K。
    T query(T p, ull k) const {
        assert(0 <= p && static_cast<unsigned long long>(p) < n && k <= max_steps);
        for(unsigned int i=0;i<levels;i++){
            if(1ull<<i & k){
                p = dp[i][p];
            }
        }
        return p;
    }
};

} // namespace sora
