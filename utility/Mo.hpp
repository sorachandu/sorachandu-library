#pragma once

#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>
#include <utility>
#include <vector>

namespace sora {

// 静的配列の半開区間クエリを並べ替え、端点の移動に応じたコールバックで処理する。
// 参考: https://ei1333.hateblo.jp/entry/2017/09/11/211011
class Mo{
    int N;
    std::vector<std::pair<int,int>> LR;

public:
    explicit Mo(int N): N(N) {}

    // 半開区間 [l, r) を登録する。償却時間 O(1)。
    void add(int l,int r){
        LR.emplace_back(l,r);
    }

    // 時間 O(Q log Q + α(N sqrt(Q) + N))、追加空間 O(Q)。
    // α は区間を1回更新する時間。結果を1回記録する時間を O(1) とする。
    template<typename AL,typename AR,typename DL,typename DR,typename REM>
    void build(const AL &add_left, const AR &add_right, const DR &del_left, const DL &del_right,const REM &rem){
        int Q=std::ssize(LR);
        if (Q == 0) return;
        int bs=std::max(1, N/std::max(1, static_cast<int>(std::sqrt(Q))));
        std::vector<int> ord(Q);
        std::iota(ord.begin(),ord.end(),0);
        std::sort(ord.begin(),ord.end(),[&](int a,int b){
            int ablock=LR[a].first/bs, bblock=LR[b].first/bs;
            if(ablock!=bblock) return ablock<bblock;
            return (ablock&1)?LR[a].second>LR[b].second:LR[a].second<LR[b].second;
        });
        int L=0,R=0;
        for(auto idx:ord){
            while(L>LR[idx].first) add_left(--L);
            while(R<LR[idx].second) add_right(R++);
            while(L<LR[idx].first) del_left(L++);
            while(R>LR[idx].second) del_right(--R);
            rem(idx);
        }
    }

    // 左右で同じ更新処理を用いる。計算量は上の build と同じ。
    template<typename A,typename D,typename R>
    void build(const A &add,const D &del,const R &rem){
        build(add,add,del,del,rem);
    }
};

} // namespace sora
