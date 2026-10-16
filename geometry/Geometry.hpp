#pragma once

#include <cmath>
#include <istream>
#include <vector>

namespace sora {

// 未完成の旧幾何実装を移植。比較演算は厳密比較と誤差比較が混在している。
inline constexpr double eps = 1e-10;

// 平面上の点・ベクトル。各演算は時間・追加空間O(1)。
struct Point {
    double x,y;
    Point(double x = 0, double y = 0): x(x), y(y) {}

    Point operator + (Point p) { return Point(x+p.x, y+p.y); }
    Point operator - (Point p) { return Point(x-p.x, y-p.y); }
    Point operator * (double a) { return Point(x*a, y*a); }
    Point operator / (double a) { return Point(x/a, y/a); }

    double abs() { return std::sqrt(norm()); }
    double norm() { return x*x + y*y; }

    bool operator < (const Point &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }
    bool operator > (const Point &p) const {
        return x != p.x ? x > p.x : y > p.y;
    }
    bool operator == (const Point &p) const {
        return std::fabs(x-p.x) < eps && std::fabs(y-p.y) < eps;
    }

    friend std::istream& operator>>(std::istream& is, Point& p) {
        return is >> p.x >> p.y;
    }
};
using Vector = Point;

// 2端点で表す線分。Lineも同じ表現を使う。
struct Segment{
    Point p1, p2;
};
using Line = Segment;

// 中心と半径で表す円。構築は時間・空間O(1)。
struct Circle{
    Point c;
    double r;
    Circle(Point c = Point(), double r = 0.0): c(c), r(r) {}
};

using Polygon = std::vector<Point>;

// 内積。時間O(1)。
inline double dot(Vector a, Vector b){
    return a.x * b.x + a.y * b.y;
}

// 外積。時間O(1)。
inline double cross(Vector a, Vector b){
    return a.x*b.y - a.y*b.x;
}

// 旧実装を維持した距離計算。時間O(1)。
// 注意: 射影が線分内なら符号付き値を返す。長さ0の線分では0除算が生じる。
inline double get_distance_sto_p(Segment s,Point p){
    if(dot(s.p2-s.p1, p-s.p1) < 0.0) return (p-s.p1).abs();
    if(dot(s.p1-s.p2, p-s.p2) < 0.0) return (p-s.p2).abs();
    return cross(s.p2-s.p1, p-s.p1) / (s.p2-s.p1).abs();
}

inline constexpr int counter_clockwise = 1;
inline constexpr int clockwise = -1;
inline constexpr int online_back = 2;
inline constexpr int online_front = -2;
inline constexpr int on_segment = 0;

// 点の位置関係。時間O(1)。
inline long long ccw(Point p0, Point p1, Point p2){
    Vector a = p1 - p0;
    Vector b = p2 - p0;
    if(cross(a,b) > eps) return counter_clockwise;
    if(cross(a,b) < -eps) return clockwise;
    if(dot(a,b) < -eps) return online_back;
    if(a.norm() < b.norm()) return online_front;
    return on_segment;
}

// 線分の交差判定。時間O(1)。
inline bool intersect(Point p1, Point p2, Point p3, Point p4){
    return (ccw(p1,p2,p3) * ccw(p1,p2,p4) <= 0 && ccw(p3,p4,p1) * ccw(p3,p4,p2) <= 0 );
}
// 線分の交差判定。時間O(1)。
inline bool intersect(Segment s1, Segment s2){
    return intersect(s1.p1, s1.p2, s2.p1, s2.p2);
}

} // namespace sora
