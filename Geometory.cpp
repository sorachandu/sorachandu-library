#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define eps (1e-10)
#define Equals(a,b) (fabs((a) - (b)) < eps )
//-----------------------------------------------------------

struct Point { 
    double x,y;
    Point(double x = 0, double y = 0): x(x), y(y) {}

    Point operator + (Point p) { return Point(x+p.x, y+p.y); }
    Point operator - (Point p) { return Point(x-p.x, y-p.y); }
    Point operator * (double a) { return Point(x*a, y*a); }
    Point operator / (double a) { return Point(x/a, y/a); }

    double magnitude() { return sqrt(norm()); }
    double norm() { return x*x + y*y; }

    bool operator < (const Point &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }
    bool operator > (const Point &p) const {
        return x != p.x ? x > p.x : y > p.y;
    }
    bool operator == (const Point &p) const {
        return fabs(x-p.x) < eps && fabs(y-p.y) < eps;
    }
};
typedef Point Vector;

struct Segment{
    Point p1, p2;
};
typedef Segment Line;

struct Circle{
    Point c;
    double r;
    Circle(Point c = Point(), double r = 0.0): c(c), r(r) {}
};

typedef vector<Point> Polygon;

double Dot(Vector a, Vector b){
    return a.x * b.x + a.y * b.y;
}

double Cross(Vector a, Vector b){
    return a.x*b.y - a.y*b.x;
}

static const int COUNTER_CLOCKWISE = 1;
static const int CLOCKWISE = -1;
static const int ONLINE_BACK = 2;
static const int ONLINE_FRONT = -2;
static const int ON_SEGMENT = 0;

ll ccw(Point p0, Point p1, Point p2){
    Vector a = p1 - p0;
    Vector b = p2 - p0;
    if(Cross(a,b) > eps) return COUNTER_CLOCKWISE;
    if(Cross(a,b) < -eps) return CLOCKWISE;
    if(Dot(a,b) < -eps) return ONLINE_BACK;
    if(a.norm() < b.norm()) return ONLINE_FRONT;
    return ON_SEGMENT;
}

bool intersect(Point p1, Point p2, Point p3, Point p4){
    return (ccw(p1,p2,p3) * ccw(p1,p2,p4) <= 0 && ccw(p3,p4,p1) * ccw(p3,p4,p2) <= 0 );
}
bool intersect(Segment s1, Segment s2){
    return intersect(s1.p1, s1.p2, s2.p1, s2.p2);
}