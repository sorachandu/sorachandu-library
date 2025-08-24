#include<bits/stdc++.h>
using namespace std;

struct rolling_hash{
	using u64 = uint64_t;
	using size_type = uint32_t;
	using i128 = __int128_t;
	
	static constexpr u64 MOD = (1uL<<61)-1;
	static u64 base;
	static once_flag initFlag;
	string str;
	vector<u64> Hash,Pow;

	private:
		u64 mul(i128 a, i128 b) const {
			i128 t=a*b;
			t=(t>>61)+(t&MOD);
			if(t>=MOD) return t-MOD;
			return t;
		}
		size_type xorshift(size_type x) const {
			x ^= x<<13;
			x ^= x>>17;
			x ^= x<<5;
			return x;
		}

	public:
		rolling_hash(const rolling_hash &) = default;
		rolling_hash(rolling_hash&&) = default;

		rolling_hash() : str() {
			call_once(initFlag,[](){
				random_device seed_gen;
				mt19937 engine(seed_gen());
				uniform_int_distribution<u64> dist(129,MOD-1);
				base=dist(engine);
			});
		};
		rolling_hash(const string &str) : str(str){
			call_once(initFlag,[](){
				random_device seed_gen;
				mt19937 engine(seed_gen());
				uniform_int_distribution<u64> dist(129,MOD-1);
				base=dist(engine);
			});
			Hash.resize(str.size()+1,0);
			Pow.resize(str.size()+1,1);
			for(size_type i=0; i<str.size(); i++){
				Hash[i+1] = mul(Hash[i],base) + xorshift(str[i]+1);
				Pow[i+1] = mul(Pow[i],base);
				if(Hash[i+1]>=MOD) Hash[i+1]-=MOD;
			}
		}

		u64 hash() const { return Hash.back(); }
		// return hash of str[l,r)
		u64 hash(size_type l, size_type r) const {
			u64 ret = MOD + Hash[r] - mul(Hash[l],Pow[r-l]);
			return ret<MOD ? ret : ret-MOD;
		}
		size_type size() const { return str.size(); }
};
uint64_t rolling_hash::base;
once_flag rolling_hash::initFlag;