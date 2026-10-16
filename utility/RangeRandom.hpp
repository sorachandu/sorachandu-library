#pragma once

#include <cassert>
#include <cstdint>
#include <random>
#include <utility>

namespace sora {

// std::uniform_int_distributionを利用した一様乱数生成クラス
class RangeRandom{
    std::random_device seed_gen;
    std::mt19937 engine;
    std::uniform_int_distribution<std::int64_t> dist;
    public:
        // 範囲は閉区間 [l, r]。既定は [0, INT64_MAX]。初期化時間は乱数源に依存。
        RangeRandom() : engine(seed_gen()) {}
        RangeRandom(std::int64_t l, std::int64_t r) : engine(seed_gen()), dist(l,r) { assert(l <= r); }
        
        // 時間 O(1)。現在の生成する値の範囲をstd::pairで返す
        std::pair<std::int64_t,std::int64_t> get_range() const {
            return std::make_pair(dist.min(),dist.max());
        }
        // 時間 O(1)。生成する値の範囲を[l,r]に変更する
        void set_range(std::int64_t l, std::int64_t r){
            assert(l <= r);
            std::uniform_int_distribution<std::int64_t>::param_type param(l,r);
            dist.param(param);
        }
        // 期待時間 O(1)。[l,r]内の一様分布の整数を返す
        std::int64_t gen(){
            return dist(engine);
        }
        // 期待時間 O(1)。gen() と同じ。
        std::int64_t operator()(){ return gen(); }
};

} // namespace sora
