#include "../../geometry/Geometry2D.hpp"
#include <cassert>
#include <cmath>
#include <random>
#include <iomanip>
#include <sstream>
#include <vector>

namespace geo = sora;
using P = geo::Point<long long>;
using S = geo::Segment<long long>;
using L = geo::Line<long long>;
using Big = long long;

int orientation(P a, P b, P c) {
    Big value = (Big(b.x)-a.x)*(Big(c.y)-a.y) - (Big(b.y)-a.y)*(Big(c.x)-a.x);
    return value > 0 ? 1 : value < 0 ? -1 : 0;
}
bool on(S s, P p) {
    return orientation(s.a,s.b,p)==0 && std::min(s.a.x,s.b.x)<=p.x && p.x<=std::max(s.a.x,s.b.x)
        && std::min(s.a.y,s.b.y)<=p.y && p.y<=std::max(s.a.y,s.b.y);
}
bool intersection(S a, S b) {
    return on(a,b.a)||on(a,b.b)||on(b,a.a)||on(b,a.b)
        || (orientation(a.a,a.b,b.a)*orientation(a.a,a.b,b.b)<0
         && orientation(b.a,b.b,a.a)*orientation(b.a,b.b,a.b)<0);
}

int main() {
    std::istringstream input("-3 7 2 -4");
    P first, second;
    assert(&(input >> first >> second) == &input);
    assert(first == P(-3, 7) && second == P(2, -4));
    std::ostringstream output;
    assert(&(output << first << '\n' << second) == &output);
    assert(output.str() == "-3 7\n2 -4");
    std::istringstream real_input("1.25 -2.5");
    sora::Point<double, long double> real_point;
    assert(real_input >> real_point);
    std::ostringstream real_output;
    real_output << std::fixed << std::setprecision(2) << real_point;
    assert(real_output.str() == "1.25 -2.50");
    std::istringstream invalid("5 invalid");
    invalid >> first;
    assert(invalid.fail() && first.x == 5);
    std::istringstream failed("8 9");
    failed.setstate(std::ios::failbit);
    first = P(3, 4);
    failed >> first;
    assert(first == P(3, 4));
    P a{0,0}, b{2,0};
    assert(geo::ccw(a,b,P{0,1})==1 && geo::ccw(a,b,P{0,-1})==-1);
    assert(geo::ccw(a,b,P{-1,0})==2 && geo::ccw(a,b,P{3,0})==-2);
    assert(geo::ccw(a,b,P{1,0})==0);
    assert(geo::ccw(a,b,P{1,0},0)==0);
    assert(geo::ccw(a,a,a)==0 && geo::ccw(a,a,b)==-2);
    assert(geo::intersect(S{a,a},S{a,b}));
    assert(!geo::intersect(S{a,a},S{b,b}));
    assert(geo::dot(P{1,2},P{3,4})==11);
    assert(geo::cross(P{1,2},P{3,4})==-2);
    assert(geo::squared_distance(a,P{3,4})==25);
    assert((P{1,2}+P{3,4}==P{4,6}));
    assert((P{1,2}-P{3,4}==P{-2,-2}));
    assert((-P{1,2}==P{-1,-2}));
    assert((P{1,2}*2==P{2,4}));
    assert((P{2,4}/2==P{1,2}));
    assert(geo::convex_hull(std::vector<P>{}).empty());
    assert(geo::convex_hull(std::vector<P>{a,a})==std::vector<P>{a});
    assert((geo::convex_hull(std::vector<P>{{0,0},{1,0},{2,0},{1,0}})==std::vector<P>{{0,0},{2,0}}));
    std::vector<P> square{{0,0},{4,0},{4,4},{0,4}};
    assert(geo::twice_signed_area(square)==32);
    assert(geo::locate(square,P{2,2})==geo::Location::inside);
    assert(geo::locate(square,P{4,2})==geo::Location::boundary);
    assert(geo::locate(square,P{5,2})==geo::Location::outside);
    std::reverse(square.begin(),square.end());
    assert(geo::twice_signed_area(square)==-32);
    assert(geo::locate(square,P{2,2})==geo::Location::inside);
    assert(geo::locate(std::vector<P>{},a)==geo::Location::outside);
    assert(geo::locate(std::vector<P>{a},a)==geo::Location::boundary);
    assert(geo::locate(std::vector<P>{a,b},P{1,0})==geo::Location::boundary);
    std::vector<P> concave{{0,0},{4,0},{4,4},{2,2},{0,4}};
    assert(geo::locate(concave,P{2,3})==geo::Location::outside);
    assert(geo::locate(concave,P{2,1})==geo::Location::inside);

    // 差がlong longを超える座標でも、拡張してから引くことを確認する。
    const long long large = 5000000000000000000LL;
    assert(geo::cross(P{-large,0},P{large,0},P{0,1})==__int128_t(large)*2);
    std::mt19937_64 rng(20261025);
    for(int trial=0;trial<2000;++trial) {
        auto random_point=[&] { return P{static_cast<long long>(rng()%41)-20,static_cast<long long>(rng()%41)-20}; };
        S s{random_point(),random_point()},t{random_point(),random_point()};
        assert(geo::intersect(s,t)==intersection(s,t));
        std::vector<P> points;
        for(int i=0;i<20;++i)points.push_back(random_point());
        auto hull=geo::convex_hull(points);
        for(std::size_t i=0;i<hull.size();++i) {
            assert(orientation(hull[i],hull[(i+1)%hull.size()],hull[(i+2)%hull.size()])>0);
            for(auto p:points)assert(orientation(hull[i],hull[(i+1)%hull.size()],p)>=0);
            assert(std::find(points.begin(),points.end(),hull[i])!=points.end());
        }
        auto p=random_point();
        assert(geo::sign(geo::cross(s.a,s.b,p))==orientation(s.a,s.b,p));
    }
    assert(std::abs(geo::distance(a,P{3,4})-5)<1e-15L);
    assert(std::abs(geo::distance<double>(S{a,b},P{1,-3})-3)<1e-12);
    assert(geo::distance(S{a,a},P{3,4})==5);
    assert(geo::distance(S{a,b},P{5,4})==5);
    assert((geo::projection(L{a,b},P{1,3})==geo::Point<long double>{1,0}));
    auto ip=geo::line_intersection<double>(L{{0,0},{3,3}},L{{0,3},{3,0}});
    assert(ip && ip->x==1.5 && ip->y==1.5);
    assert(!geo::line_intersection(L{a,b},L{{0,1},{2,1}}));
    assert(!geo::line_intersection(L{a,b},L{a,b}));
    using F=geo::Point<long double>;
    assert(geo::ccw(F{0,0},F{1,0},F{0.5L,1e-12L},1e-10L)==0);
    assert(geo::ccw(F{0,0},F{1,0},F{0.5L,1e-12L})==1);
    assert(geo::ccw(F{0,0},F{1,0},F{0.5L,1e-12L},1e-10)==0);
    // 拡張実数型でも、平方根の関数を明示すれば暗黙の精度低下なしで利用できる。
    using Q=__float128;
    auto root=[](Q x) { Q r=x>1?x:1; for(int i=0;i<100;++i) r=(r+x/r)/2; return r; };
    assert(geo::distance<Q>(a,P{3,4},root)==Q(5));
    assert(geo::distance<Q>(S{a,b},P{1,3},root)==Q(3));
    auto qp=geo::projection<Q>(L{a,b},P{1,3});
    assert(qp.x==Q(1) && qp.y==Q(0));
    auto qi=geo::line_intersection<Q>(L{{0,0},{3,3}},L{{0,3},{3,0}});
    assert(qi && qi->x==Q(1.5));
}
