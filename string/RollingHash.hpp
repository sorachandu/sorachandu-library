#pragma once

#include <cassert>
#include <cstdint>
#include <limits>
#include <mutex>
#include <random>
#include <string>
#include <vector>

namespace sora {

// 2^61-1 を法とするローリングハッシュ。全インスタンスでランダムな基数を共有。
// 衝突の可能性がある。クエリは O(1)、保持空間 O(N)。
struct RollingHash{
	using u64 = std::uint64_t;
	using size_type = std::uint32_t;
	using i128 = __int128_t;
	
	static constexpr u64 mod = (1ULL<<61)-1;
	inline static u64 base = 0;
	inline static std::once_flag init_flag;
	std::string str;
	std::vector<u64> hash_values,powers;

	private:
		u64 mul(i128 a, i128 b) const {
			i128 t=a*b;
			t=(t>>61)+(t&mod);
			if(t>=mod) return t-mod;
			return t;
		}
		size_type xorshift(size_type x) const {
			x ^= x<<13;
			x ^= x>>17;
			x ^= x<<5;
			return x;
		}

	public:
		// コピーは時間・空間 O(N)。
		RollingHash(const RollingHash &) = default;
		// ムーブは時間 O(1)。
		RollingHash(RollingHash&&) = default;

		// 空文字列を構築。時間・空間 O(1)（初回の乱数源の時間を除く）。
		RollingHash() : RollingHash(std::string{}) {}
		// 時間・空間 O(N)。入力は変更しない。N は uint32_t に収まること。
		RollingHash(const std::string &str) : str(str){
		    assert(str.size() < std::numeric_limits<size_type>::max());
			std::call_once(init_flag,[](){
				std::random_device seed_gen;
				std::mt19937 engine(seed_gen());
				std::uniform_int_distribution<u64> dist(129,mod-1);
				base=dist(engine);
			});
			hash_values.resize(str.size()+1,0);
			powers.resize(str.size()+1,1);
			for(size_type i=0; i<str.size(); i++){
				hash_values[i+1] = mul(hash_values[i],base) + xorshift(static_cast<unsigned char>(str[i])+1);
				powers[i+1] = mul(powers[i],base);
				if(hash_values[i+1]>=mod) hash_values[i+1]-=mod;
			}
		}

		// 全体のハッシュ。時間 O(1)。
		u64 hash() const { return hash_values.back(); }
		// 半開区間 [l,r) のハッシュ。時間 O(1)。
		u64 hash(size_type l, size_type r) const {
		    assert(l <= r && r <= str.size());
			u64 ret = mod + hash_values[r] - mul(hash_values[l],powers[r-l]);
			return ret<mod ? ret : ret-mod;
		}

		// ハッシュの連結。時間 O(1)。h2_len <= size()、両ハッシュは本クラスで生成したもの。
		u64 concat(u64 h1, u64 h2, size_type h2_len) const {
		    assert(h2_len <= str.size());
			u64 ret=mul(h1,powers[h2_len])+h2;
			return ret<mod ? ret : ret-mod;
		}

		// 文字列長。時間 O(1)。
		size_type size() const { return str.size(); }
};

} // namespace sora
