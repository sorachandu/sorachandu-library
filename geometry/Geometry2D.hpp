#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <functional>
#include <istream>
#include <optional>
#include <ostream>
#include <type_traits>
#include <utility>
#include <vector>

namespace sora {

// 整数座標の積・差を計算する型。必要ならPointの第2テンプレート引数で変更できる。
template <class T>
using DefaultCalculation = std::conditional_t<std::is_integral_v<T>, __int128_t, T>;

// 平面上の点・ベクトル。座標演算はT、幾何判定はCで計算する。各演算は時間・空間O(1)。
// T/Cは符号付きの数値型とし、全中間値が各型に収まること。比較は厳密な辞書順。
template <class T, class C = DefaultCalculation<T>>
struct Point {
    T x{}, y{};
    constexpr Point() = default;
    constexpr Point(T x, T y) : x(x), y(y) {}
    constexpr bool operator==(const Point&) const = default;
    constexpr bool operator<(const Point& p) const { return x < p.x || (x == p.x && y < p.y); }
    constexpr Point operator+(Point p) const { return {x + p.x, y + p.y}; }
    constexpr Point operator-(Point p) const { return {x - p.x, y - p.y}; }
    constexpr Point operator-() const { return {-x, -y}; }
    constexpr Point operator*(T k) const { return {x * k, y * k}; }
    // 整数型では整数除算。k != 0を満たすこと。
    constexpr Point operator/(T k) const { assert(k != T{}); return {x / k, y / k}; }
    template <class R>
    constexpr Point<R> cast() const { return {static_cast<R>(x), static_cast<R>(y)}; }
};

// x、yの順に読み込み、pを書き換える。状態と失敗時の挙動はTの入力演算に従う。
// 時間はTの入力2回分、追加空間O(1)。入力ストリームを返す。
template <class T, class C>
std::istream& operator>>(std::istream& input, Point<T, C>& p) {
    return input >> p.x >> p.y;
}

// x、空白1文字、yの順に出力する。改行は付けない。
// 時間はTの出力2回分、追加空間O(1)。出力ストリームを返す。
template <class T, class C>
std::ostream& operator<<(std::ostream& output, const Point<T, C>& p) {
    return output << p.x << ' ' << p.y;
}

template <class T, class C = DefaultCalculation<T>>
using Vector = Point<T, C>;

// 2端点で表す線分。a == bも許す。
template <class T, class C = DefaultCalculation<T>>
struct Segment { Point<T, C> a, b; };

// 異なる2点で表す直線。直線に対する演算ではa != bを満たすこと。
template <class T, class C = DefaultCalculation<T>>
struct Line { Point<T, C> a, b; };

// 符号判定。epsは絶対許容誤差で非負とする。整数ではeps=0を使う。時間O(1)。
template <class C>
constexpr int sign(C value, std::type_identity_t<C> eps = C{}) {
    assert(eps >= C{});
    return value > eps ? 1 : value < -eps ? -1 : 0;
}

// 内積・外積。積を取る前にCへ変換する。時間O(1)。
template <class T, class C>
constexpr C dot(Point<T, C> a, Point<T, C> b) { return C(a.x) * C(b.x) + C(a.y) * C(b.y); }
template <class T, class C>
constexpr C cross(Point<T, C> a, Point<T, C> b) { return C(a.x) * C(b.y) - C(a.y) * C(b.x); }

// (b-a)×(c-a)。差を取る前にCへ変換する。時間O(1)。
template <class T, class C>
constexpr C cross(Point<T, C> a, Point<T, C> b, Point<T, C> c) {
    return (C(b.x) - C(a.x)) * (C(c.y) - C(a.y))
         - (C(b.y) - C(a.y)) * (C(c.x) - C(a.x));
}

// 2点間の距離の二乗。整数座標では整数のまま返す。時間O(1)。
template <class T, class C>
constexpr C squared_distance(Point<T, C> a, Point<T, C> b) {
    C x = C(b.x) - C(a.x), y = C(b.y) - C(a.y);
    return x * x + y * y;
}

inline constexpr int counter_clockwise = 1, clockwise = -1;
inline constexpr int online_back = 2, online_front = -2, on_segment = 0;

// 有向線分abに対するcの位置。反時計回り1、時計回り-1、後方2、前方-2、線分上0。
// a == bならc == aを線分上、それ以外を前方とする。時間O(1)。
template <class T, class C>
constexpr int ccw(Point<T, C> a, Point<T, C> b, Point<T, C> c, std::type_identity_t<C> eps = C{}) {
    int s = sign(cross(a, b, c), eps);
    if (s) return s;
    C ux = C(b.x) - C(a.x), uy = C(b.y) - C(a.y);
    C vx = C(c.x) - C(a.x), vy = C(c.y) - C(a.y);
    if (sign(ux * vx + uy * vy, eps) < 0) return online_back;
    if (sign(ux * ux + uy * uy - vx * vx - vy * vy, eps) < 0) return online_front;
    return on_segment;
}

// 点pが線分s上にあるか。退化線分も扱う。時間O(1)。
template <class T, class C>
constexpr bool contains(Segment<T, C> s, Point<T, C> p, std::type_identity_t<C> eps = C{}) {
    if (sign(cross(s.a, s.b, p), eps)) return false;
    C ax = C(p.x) - C(s.a.x), ay = C(p.y) - C(s.a.y);
    C bx = C(p.x) - C(s.b.x), by = C(p.y) - C(s.b.y);
    return sign(ax * bx + ay * by, eps) <= 0;
}

// 線分交差。端点の接触・重なり・退化線分を交差に含める。時間O(1)。
template <class T, class C>
constexpr bool intersect(Segment<T, C> s, Segment<T, C> t, std::type_identity_t<C> eps = C{}) {
    int a = sign(cross(s.a, s.b, t.a), eps), b = sign(cross(s.a, s.b, t.b), eps);
    int c = sign(cross(t.a, t.b, s.a), eps), d = sign(cross(t.a, t.b, s.b), eps);
    if (a * b < 0 && c * d < 0) return true;
    return (!a && contains(s, t.a, eps)) || (!b && contains(s, t.b, eps))
        || (!c && contains(t, s.a, eps)) || (!d && contains(t, s.b, eps));
}

// 符号付き面積の2倍。反時計回りが正。空・1点・2点は0。時間O(N)、追加空間O(1)。
template <class T, class C>
C twice_signed_area(const std::vector<Point<T, C>>& polygon) {
    C result{};
    for (std::size_t i = 1; i + 1 < polygon.size(); ++i)
        result += cross(polygon[0], polygon[i], polygon[i + 1]);
    return result;
}

// 単調鎖法による凸包。重複と辺上の中間点を除き、辞書順最小の頂点から反時計回りに返す。
// 全点が同一直線上なら両端点、空なら空を返す。入力をコピーする。時間O(N log(N+1))、空間O(N)。
template <class T, class C>
std::vector<Point<T, C>> convex_hull(std::vector<Point<T, C>> points, std::type_identity_t<C> eps = C{}) {
    std::sort(points.begin(), points.end());
    points.erase(std::unique(points.begin(), points.end()), points.end());
    if (points.size() <= 1) return points;
    std::vector<Point<T, C>> hull;
    hull.reserve(points.size() * 2);
    for (const auto& p : points) {
        while (hull.size() >= 2 && sign(cross(hull[hull.size()-2], hull.back(), p), eps) <= 0)
            hull.pop_back();
        hull.push_back(p);
    }
    const auto lower_size = hull.size();
    for (std::size_t i = points.size() - 1; i-- > 0;) {
        const auto& p = points[i];
        while (hull.size() > lower_size && sign(cross(hull[hull.size()-2], hull.back(), p), eps) <= 0)
            hull.pop_back();
        hull.push_back(p);
    }
    hull.pop_back();
    return hull;
}

// 多角形に対する点の位置。
enum class Location { outside, boundary, inside };

// 単純多角形の内外判定。頂点は周回順、終点の再掲は不要。空・1点・2点も扱う。
// 半直線との交差の偶奇を使う。時間O(N)、追加空間O(1)。入力は変更しない。
template <class T, class C>
Location locate(const std::vector<Point<T, C>>& polygon, Point<T, C> p, std::type_identity_t<C> eps = C{}) {
    bool inside = false;
    for (std::size_t i = 0; i < polygon.size(); ++i) {
        auto a = polygon[i], b = polygon[(i + 1) % polygon.size()];
        if (contains(Segment<T, C>{a, b}, p, eps)) return Location::boundary;
        if (a.y > b.y) std::swap(a, b);
        if (a.y <= p.y && p.y < b.y && sign(cross(a, b, p), eps) > 0) inside = !inside;
    }
    return inside ? Location::inside : Location::outside;
}

// 実数演算の内部補助。平方根はADLで利用者定義の型にも対応する。
struct StandardSqrt {
    template <class R> R operator()(R value) const { using std::sqrt; return sqrt(value); }
};

// 点間距離。Rは実数型。128bit拡張型ではsqrtqなどを第3引数へ渡す。時間O(1)。
template <class R = long double, class T, class C, class Sqrt = StandardSqrt>
R distance(Point<T, C> a, Point<T, C> b, Sqrt sqrt_fn = {}) {
    R x = R(b.x) - R(a.x), y = R(b.y) - R(a.y);
    return std::invoke(sqrt_fn, x * x + y * y);
}

// 点pの直線lへの射影。Rは実数型、l.a != l.b。変換後の差の二乗が非零であること。時間O(1)。
template <class R = long double, class T, class C>
Point<R> projection(Line<T, C> l, Point<T, C> p) {
    R x = R(l.b.x) - R(l.a.x), y = R(l.b.y) - R(l.a.y);
    R norm = x*x + y*y;
    assert(norm != R{});
    R t = ((R(p.x)-R(l.a.x))*x + (R(p.y)-R(l.a.y))*y) / norm;
    return {R(l.a.x) + t*x, R(l.a.y) + t*y};
}

// 直線交点。一意に交わらない場合（平行・一致）はnullopt。両直線は非退化とする。
// 平行判定はCで行い、座標計算は実数型Rで行う。時間O(1)。
template <class R = long double, class T, class C>
std::optional<Point<R>> line_intersection(Line<T, C> l, Line<T, C> m, std::type_identity_t<C> eps = C{}) {
    assert(l.a != l.b && m.a != m.b);
    C x = C(l.b.x)-C(l.a.x), y = C(l.b.y)-C(l.a.y);
    C u = C(m.b.x)-C(m.a.x), v = C(m.b.y)-C(m.a.y);
    C denominator = x*v - y*u;
    if (!sign(denominator, eps)) return std::nullopt;
    C numerator = (C(m.a.x)-C(l.a.x))*v - (C(m.a.y)-C(l.a.y))*u;
    R t = R(numerator) / R(denominator);
    return Point<R>{R(l.a.x)+R(x)*t, R(l.a.y)+R(y)*t};
}

// 点・線分間距離。退化線分では端点との距離を返す。Rは実数型。時間O(1)。
template <class R = long double, class T, class C, class Sqrt = StandardSqrt>
R distance(Segment<T, C> s, Point<T, C> p, Sqrt sqrt_fn = {}) {
    R x = R(s.b.x)-R(s.a.x), y = R(s.b.y)-R(s.a.y);
    R norm = x*x + y*y;
    if (norm == R{}) return distance<R>(s.a, p, sqrt_fn);
    R t = std::clamp(((R(p.x)-R(s.a.x))*x + (R(p.y)-R(s.a.y))*y)/norm, R{}, R(1));
    R dx = R(p.x)-R(s.a.x)-t*x, dy = R(p.y)-R(s.a.y)-t*y;
    return std::invoke(sqrt_fn, dx*dx + dy*dy);
}

} // namespace sora
