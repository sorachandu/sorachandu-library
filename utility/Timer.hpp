#pragma once

#include <chrono>

namespace sora {

// std::chronoを利用した時間計測用クラス
class Timer{
    std::chrono::steady_clock::time_point start;
    public:
        // 計測を開始する。時間 O(1)。
        Timer() : start(std::chrono::steady_clock::now()) {}
    
        // 経過秒数を返す。時間 O(1)。
        double count() const {
            std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - start;
            return elapsed.count();
        }

        // 経過秒数が x 未満か。時間 O(1)。
        bool is_under(double x) const {
            return (this -> count()) < x;
        }
};

} // namespace sora
