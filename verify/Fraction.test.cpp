// verification-helper: IGNORE
#include <algorithm>
#include <cassert>
#include <limits>
#include <numeric>
#include <random>
#include <unordered_set>
#include <vector>

#include "../math/Fraction.hpp"

// 中間積の回帰例、NaNの全順序、小さい値の単純解との比較。
int main() {
    using F = sora::Fraction;
    F nan(1,0), unset;
    assert(nan == unset && !(nan < unset) && !(nan > unset));
    assert(F(0) < nan && nan > F(0) && !(nan <= F(0)));
    assert((F(1)/F(0)).is_nan() && F(0).inv().is_nan());
    assert((nan+F(1)).is_nan() && (unset*F(2)).is_nan());
    std::vector<F> order{nan,F(1),F(-1),unset,F(0)};
    std::sort(order.begin(),order.end());
    assert(order[0]==F(-1) && order[1]==F(0) && order[2]==F(1));
    assert(order[3].is_nan() && order[4].is_nan());
    std::unordered_set<F,sora::FractionHash> set{nan,unset,F(1,2),F(2,4)};
    assert(set.size()==2);
    F small(1,4000000000LL);
    assert(small+small==F(1,2000000000LL));
    assert(F(4000000000LL,4000000001LL)<F(4000000001LL,4000000002LL));
    const long long limit=std::numeric_limits<long long>::max();
    assert(F(limit,limit-1)*F(limit-1,limit)==F(1));
    assert(F(limit,limit-1)/F(limit,limit-1)==F(1));
    assert(F(limit,limit-1)-F(limit,limit-1)==F(0));
    assert(F(-limit,limit-1)+F(limit,limit-1)==F(0));
    assert(F(1,-2)==F(-1,2) && F(0,-2)==F(0));
    auto expected=[](long long n,long long d) {
        if(d<0){n=-n;d=-d;}
        auto g=std::gcd(n,d);
        return std::pair(n/g,d/g);
    };
    std::mt19937 rng(123);
    for(int t=0;t<3000;++t) {
        long long a=static_cast<int>(rng()%201)-100,b=rng()%100+1;
        long long c=static_cast<int>(rng()%201)-100,d=rng()%100+1;
        F x(a,b),y(c,d);
        assert((x+y).get_pair()==expected(a*d+b*c,b*d));
        assert((x-y).get_pair()==expected(a*d-b*c,b*d));
        assert((x*y).get_pair()==expected(a*c,b*d));
        if(c)assert((x/y).get_pair()==expected(a*d,b*c));
        assert((x<y)==(a*d<c*b));
        assert((x==y)==(a*d==c*b));
        assert((x>y)==(y<x) && (x<=y)==!(y<x) && (x>=y)==!(x<y));
        assert(-x==F(-a,b) && x.abs()==F(a<0?-a:a,b));
    }
}
