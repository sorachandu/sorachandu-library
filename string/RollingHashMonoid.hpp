#pragma once

#include <cstddef>
#include <cstdint>
#include <random>

namespace sora {

// 法2^61-1の文字列連結モノイド。基数は初回のfrom_charでランダムに決め、以後固定する。
// RollingHashから独立。衝突の可能性がある。各操作の追加空間はO(1)。
struct RollingHashMonoid {
    using u64 = std::uint64_t;
    static constexpr u64 mod = (u64{1} << 61) - 1;

    // 文字列のハッシュ、基数のlen乗、長さ。既定値は空文字列。
    struct T {
        u64 hash = 0;
        u64 power = 1;
        std::size_t len = 0;

        // 長さを含めて比較する。等しくても文字列の一致は保証しない。時間O(1)。
        friend bool operator==(const T&, const T&) = default;
    };

private:
    static u64 base() {
        static const u64 value = [] {
            std::random_device seed;
            std::mt19937_64 engine(seed());
            return std::uniform_int_distribution<u64>(257, mod - 2)(engine);
        }();
        return value;
    }

    static u64 mul(u64 a, u64 b) {
        const __uint128_t product = static_cast<__uint128_t>(a) * b;
        const u64 reduced = static_cast<u64>(product >> 61) + (static_cast<u64>(product) & mod);
        return reduced >= mod ? reduced - mod : reduced;
    }

public:
    // 空文字列を返す。乱数生成は行わない。時間O(1)。
    static constexpr T e() { return {}; }

    // 1バイトを値に変換する。符号化はunsigned charの値+1。時間O(1)。
    // 初回のみ乱数源・乱数生成器の初期化時間が別途かかる。
    static T from_char(unsigned char c) { return {static_cast<u64>(c) + 1, base(), 1}; }

    // aの後ろにbを連結する。入力は変更しない。時間O(1)。
    // 引数はe/from_char/opで生成した値とし、各フィールドは変更しないこと。
    // 連結後の長さがstd::size_tに収まること。
    static T op(const T& a, const T& b) {
        u64 hash = mul(a.hash, b.power) + b.hash;
        if (hash >= mod) hash -= mod;
        return {hash, mul(a.power, b.power), a.len + b.len};
    }
};

} // namespace sora
