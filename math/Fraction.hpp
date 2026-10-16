#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <limits>
#include <ostream>
#include <utility>

namespace sora {

// 既約分数。有限値の比較はO(1)、構築・四則演算はユークリッド互除法でO(log M)。
// 空間O(1)。Mは128bit中間値の絶対値の最大。NaN（未初期化を含む）は有限値より後。
// LLONG_MIN、および約分後の分子・分母がlong longに収まらない場合は対象外。
// 公開メンバを変更する場合は、比較・ハッシュの前にnormalize()すること。
struct Fraction{
    long long num,den;
    std::int8_t state;
    // 未初期化のNaNを構築する。時間O(1)。
    Fraction(){
        num=den=0;
        state=-1;
    }
    // 整数から構築する。時間O(1)。
    Fraction(long long a) : Fraction(a,1) {}
    // 分子・分母から構築する。時間O(log M)。分母0はNaN。
    Fraction(long long a,long long b) {
        assert(a != std::numeric_limits<long long>::min());
        assert(b != std::numeric_limits<long long>::min());
        *this = from_wide(a, b);
    }

    // 正規化する。時間O(log M)、Mは分子・分母の絶対値の最大。
    void normalize() {
        assert(state == 1);
        *this = from_wide(num, den);
    }

private:
    static Fraction from_wide(__int128_t a, __int128_t b) {
        Fraction result;
        if (b == 0) {
            result.num = 1; result.den = 0; result.state = 0;
            return result;
        }
        if (b < 0) { a = -a; b = -b; }
        __int128_t x = a < 0 ? -a : a, y = b;
        while (y != 0) { const auto r = x % y; x = y; y = r; }
        a /= x; b /= x;
        [[maybe_unused]] const auto limit = std::numeric_limits<long long>::max();
        assert(-limit <= a && a <= limit && b <= limit);
        result.num = static_cast<long long>(a);
        result.den = static_cast<long long>(b);
        result.state = 1;
        return result;
    }

public:
    // 分子・分母を返す。時間O(1)。
    inline std::pair<long long,long long> get_pair() const {
        return {num,den};
    }
    // 分子を返す。時間O(1)。
    inline long long get_num() const {
        return num;
    }
    // 分母を返す。時間O(1)。
    inline long long get_den() const {
        return den;
    }

    // 分母0または未初期化か。時間O(1)。
    inline bool is_nan() const {
        return state!=1;
    }

    // 時間O(1)。任意のnum,denを超過する適当な定数を渡して1次元化
    // 結果がlong longに収まること。
    inline long long convert_1d(long long value) const {
        const __int128_t encoded = __int128_t(num) * value + den;
        assert(-__int128_t(std::numeric_limits<long long>::max()) <= encoded
               && encoded <= std::numeric_limits<long long>::max());
        return static_cast<long long>(encoded);
    }

    // double値への変換。時間O(1)。
    double to_double() const {
        if(is_nan()) return std::numeric_limits<double>::quiet_NaN();
        return (double)num / den;
    }
    
    // 絶対値。時間O(log M)。
    Fraction abs() const {
        if(is_nan()) return *this;
        return Fraction(std::abs(num), den);
    }

    // 乗法の逆元。時間O(log M)。0の逆元はNaN。
    Fraction inv() const {
        if(is_nan()) return *this;
        return Fraction(den, num);
    }
    
    // 符号反転。時間O(log M)。
    Fraction operator-() const {
        if(is_nan()) return *this;
        return Fraction(-num, den);
    }

    // 四則演算。時間O(log M)、空間O(1)。0除算・NaNとの演算はNaN。
    Fraction operator+(const Fraction& other) const {
        if(is_nan() || other.is_nan()) return Fraction(1, 0);
        return from_wide(__int128_t(num) * other.den + __int128_t(den) * other.num, __int128_t(den) * other.den);
    }
    
    Fraction operator-(const Fraction& other) const {
        if(is_nan() || other.is_nan()) return Fraction(1, 0);
        return from_wide(__int128_t(num) * other.den - __int128_t(den) * other.num, __int128_t(den) * other.den);
    }
    
    Fraction operator*(const Fraction& other) const {
        if(is_nan() || other.is_nan()) return Fraction(1, 0);
        return from_wide(__int128_t(num) * other.num, __int128_t(den) * other.den);
    }
    
    Fraction operator/(const Fraction& other) const {
        if(is_nan() || other.is_nan() || other.num == 0) return Fraction(1, 0);
        return from_wide(__int128_t(num) * other.den, __int128_t(den) * other.num);
    }
    
    // 比較演算子。時間O(1)。全てのNaNを同値とし、有限値より後へ置く。
    bool operator<(const Fraction& other) const {
        if (is_nan() || other.is_nan()) return !is_nan() && other.is_nan();
        return __int128_t(num) * other.den < __int128_t(den) * other.num;
    }
    
    bool operator==(const Fraction& other) const {
        if(is_nan() && other.is_nan()) return true;
        if(is_nan() || other.is_nan()) return false;
        return num == other.num && den == other.den;
    }
    
    bool operator!=(const Fraction& other) const { return !(*this == other); }
    bool operator<=(const Fraction& other) const { return *this < other || *this == other; }
    bool operator>(const Fraction& other) const { return !(*this <= other); }
    bool operator>=(const Fraction& other) const { return !(*this < other); }
};

// 分数を出力する。時間O(1)（64bit整数の桁数を定数とする）。
inline std::ostream& operator<<(std::ostream& os, const Fraction& f) {
    if(f.state == -1) {
        os << "Unnitialized";
    } else if(f.state == 0){
        os << "NaN";
    } else if(f.den == 1) {
        os << f.num;
    } else {
        os << f.num << "/" << f.den;
    }
    return os;
}

// 正規化済みFractionのハッシュ。NaNは全て同じ値。時間O(1)。
struct FractionHash {
    std::size_t operator()(const Fraction& f) const {
        if(f.is_nan()) return 0;
        auto h1 = std::hash<long long>{}(f.num);
        auto h2 = std::hash<long long>{}(f.den);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15LLU + (h1 << 12) + (h1 >> 4));
    }
};

} // namespace sora
