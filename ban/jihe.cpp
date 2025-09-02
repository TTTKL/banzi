#include<bits/stdc++.h>
 
using i64 = long long;
using l64 = long double;
 
template<class T>
struct Point {
    T x;
    T y;
 
    Point<T>(T x_ = 0, T y_ = 0) : x(x_), y(y_) {}
 
    template<class U>
    operator Point<U>() {
        return Point<U>(U(x), U(y));
    }
 
    Point &operator+=(Point p) &{
        x += p.x;
        y += p.y;
        return *this;
    }
 
    Point &operator-=(Point p) &{
        x -= p.x;
        y -= p.y;
        return *this;
    }
 
    Point &operator*=(T v) &{
        x *= v;
        y *= v;
        return *this;
    }
 
    Point operator-() const {
        return Point<T>(-x, -y);
    }
 
    friend Point operator+(Point a, Point b) {
        return a += b;
    }
 
    friend Point operator-(Point a, Point b) {
        return a -= b;
    }
 
    friend Point operator*(Point a, T b) {
        return a *= b;
    }
 
    friend Point operator*(T a, Point b) {
        return b *= a;
    }
 
    friend bool operator==(Point a, Point b) {
        return a.x == b.x && a.y == b.y;
    }
 
    friend std::istream &operator>>(std::istream &is, Point &p) {
        return is >> p.x >> p.y;
    }
 
    friend std::ostream &operator<<(std::ostream &os, Point p) {
        return os << p.x << " " << p.y;
    }
};
 
//a*b==0,两个向量垂直
template<class T>
T dot(Point<T> a, Point<T> b) {//计算两个点之间的点积。
    return a.x * b.x + a.y * b.y;
}
 
//a*b==0,两个向量平行
template<class T>
T cross(Point<T> a, Point<T> b) {//计算两个点之间的叉积
    return a.x * b.y - a.y * b.x;
}
 
template<class T>
T square(Point<T> p) {//计算一个点与其自身的点积
    return dot(p, p);
}
 
template<class T>
double length(Point<T> p) {//计算一个点表示的向量的长度
    return std::sqrt(double(square(p)));
}
 
long double length(Point<long double> p) {
    return std::sqrt(square(p));
}
//cos=a*b/(|a|*|b|)
 
template<class T>
struct Line {
    Point<T> a;
    Point<T> b;
 
    Line<T>(Point<T> a_ = Point<T>(), Point<T> b_ = Point<T>()) : a(a_), b(b_) {}
 
    friend std::istream &operator>>(std::istream &is, Line &l) {
        return is >> l.a.x >> l.a.y >> l.b.x >> l.b.y;
    }
};
 
template<class T>
T dist(Point<T> p, Line<T> line) {// 计算点道线段的距离
    if (line.a == line.b) {
        return dist(p, line.a);
    }
    Point<T> p1 = line.b - line.a, p2 = p - line.a, p3 = p - line.b;
    if (dot(p1, p2) < 0) return length(p2);
    if (dot(p1, p3) > 0) return length(p3);
 
    return fabs(cross(line.b - line.a, p - line.a) / length(line.b - line.a));
}
 
template<class T>
T dist(Point<T> a, Point<T> b) {//计算两点之间的距离
    return std::hypot(a.x - b.x, a.y - b.y);
}
 
template<class T>
T dist(Line<T> line) {//计算直线的距离
    return std::hypot(line.a.x - line.b.x, line.a.y - line.b.y);
}
 
template<class T>
Point<T> rotate(Point<T> a) {//将一个点绕原点旋转 90 度（逆时针）
    return Point<T>(-a.y, a.x);
}
 
//根据点的位置（相对于原点）返回一个符号值。如果点在 x 轴上方（或在 x 轴上但 y = 0 且 x > 0），则返回 1；否则返回 -1。
template<class T>
int sgn(Point<T> a) {
    return a.y > 0 || (a.y == 0 && a.x > 0) ? 1 : -1;
}
 
template<class T>
bool pointOnLineLeft(Point<T> p, Line<T> l) {//判断点 p 是否在线段 l 的左侧（不包括线段上）。
    return cross(l.b - l.a, p - l.a) > 0;
}
 
template<class T>
Point<T> lineIntersection(Line<T> l1, Line<T> l2) {//计算两条线段 l1 和 l2 的交点。
    return l1.a + (l1.b - l1.a) * (cross(l2.b - l2.a, l1.a - l2.a) / cross(l2.b - l2.a, l1.a - l1.b));
}
 
template<class T>
bool pointOnSegment(Point<T> p, Line<T> l) {//判断点 p 是否在线段 l 上（包括端点）。
    return cross(p - l.a, l.b - l.a) == 0 && std::min(l.a.x, l.b.x) <= p.x && p.x <= std::max(l.a.x, l.b.x)
           && std::min(l.a.y, l.b.y) <= p.y && p.y <= std::max(l.a.y, l.b.y);
}
 
template<class T>
bool pointInPolygon(Point<T> a, std::vector<Point<T>> p) {//判断点 a 是否在多边形 p 内部。
    int n = p.size();
    for (int i = 0; i < n; i++) {
        if (pointOnSegment(a, Line<T>(p[i], p[(i + 1) % n]))) {
            return true;
        }
    }
 
    int t = 0;
    for (int i = 0; i < n; i++) {
        auto u = p[i];
        auto v = p[(i + 1) % n];
        if (u.x < a.x && v.x >= a.x && pointOnLineLeft(a, Line<T>(v, u))) {
            t ^= 1;
        }
        if (u.x >= a.x && v.x < a.x && pointOnLineLeft(a, Line<T>(u, v))) {
            t ^= 1;
        }
    }
 
    return t == 1;
}
 
template<class T>
std::vector<Point<T>> Andrew(std::vector<Point<T>> p) {//求凸包
    std::sort(p.begin(), p.end(), [&](Point<T> x, Point<T> y) {
        return x.x != y.x ? x.x < y.x : x.y < y.y;
    });
    std::vector<Point<T>> stk;
    int n = p.size();
    for (int i = 0; i < n; i++) {
        while (stk.size() > 1 && cross(stk.back() - stk[stk.size() - 2], p[i] - stk[stk.size() - 2]) <= 0)
            stk.pop_back();
        stk.push_back(p[i]);
    }
    int tmp = stk.size();
    for (int i = n - 2; i >= 0; i--) {
        while (stk.size() > tmp && cross(stk.back() - stk[stk.size() - 2], p[i] - stk[stk.size() - 2]) <= 0)
            stk.pop_back();
        stk.push_back(p[i]);
    }
    stk.pop_back();
    return stk;
}
 
template<class T>
std::pair<Point<T>, Point<T>> rotatingCalipers(std::vector<Point<T>> &p) {//旋转卡壳求最远点对距离
    T res = 0;
    std::pair<Point<T>, Point<T>> ans;
    int n = p.size();
    for (int i = 0, j = 1; i < n; i++) {
        while (cross(p[i + 1] - p[i], p[j] - p[i]) < cross(p[i + 1] - p[i], p[j + 1] - p[i])) j = (j + 1) % n;
        if (dist(p[i], p[j]) > res) {
            ans = {p[i], p[j]};
            res = dist(p[i], p[j]);
        }
        if (dist(p[i + 1], p[j]) > res) {
            ans = {p[i + 1], p[j]};
            res = dist(p[i + 1], p[j]);
        }
    }
    return ans;
}
 
// 0 : not intersect不相交
// 1 : strictly intersect严格相交
// 2 : overlap重叠
// 3 : intersect at endpoint在端点相交
//判断两条线段 l1 和 l2 是否相交，
template<class T>
std::tuple<int, Point<T>, Point<T>> segmentIntersection(Line<T> l1, Line<T> l2) {
    if (std::max(l1.a.x, l1.b.x) < std::min(l2.a.x, l2.b.x)) {
        return {0, Point<T>(), Point<T>()};
    }
    if (std::min(l1.a.x, l1.b.x) > std::max(l2.a.x, l2.b.x)) {
        return {0, Point<T>(), Point<T>()};
    }
    if (std::max(l1.a.y, l1.b.y) < std::min(l2.a.y, l2.b.y)) {
        return {0, Point<T>(), Point<T>()};
    }
    if (std::min(l1.a.y, l1.b.y) > std::max(l2.a.y, l2.b.y)) {
        return {0, Point<T>(), Point<T>()};
    }
    if (cross(l1.b - l1.a, l2.b - l2.a) == 0) {
        if (cross(l1.b - l1.a, l2.a - l1.a) != 0) {
            return {0, Point<T>(), Point<T>()};
        } else {
            auto maxx1 = std::max(l1.a.x, l1.b.x);
            auto minx1 = std::min(l1.a.x, l1.b.x);
            auto maxy1 = std::max(l1.a.y, l1.b.y);
            auto miny1 = std::min(l1.a.y, l1.b.y);
            auto maxx2 = std::max(l2.a.x, l2.b.x);
            auto minx2 = std::min(l2.a.x, l2.b.x);
            auto maxy2 = std::max(l2.a.y, l2.b.y);
            auto miny2 = std::min(l2.a.y, l2.b.y);
            Point<T> p1(std::max(minx1, minx2), std::max(miny1, miny2));
            Point<T> p2(std::min(maxx1, maxx2), std::min(maxy1, maxy2));
            if (!pointOnSegment(p1, l1)) {
                std::swap(p1.y, p2.y);
            }
            if (p1 == p2) {
                return {3, p1, p2};
            } else {
                return {2, p1, p2};
            }
        }
    }
    auto cp1 = cross(l2.a - l1.a, l2.b - l1.a);
    auto cp2 = cross(l2.a - l1.b, l2.b - l1.b);
    auto cp3 = cross(l1.a - l2.a, l1.b - l2.a);
    auto cp4 = cross(l1.a - l2.b, l1.b - l2.b);
 
    if ((cp1 > 0 && cp2 > 0) || (cp1 < 0 && cp2 < 0) || (cp3 > 0 && cp4 > 0) || (cp3 < 0 && cp4 < 0)) {
        return {0, Point<T>(), Point<T>()};
    }
 
    Point p = lineIntersection<T>(l1, l2);
    if (cp1 != 0 && cp2 != 0 && cp3 != 0 && cp4 != 0) {
        return {1, p, p};
    } else {
        return {3, p, p};
    }
}
 
//判断一条线段 l 是否完全位于一个多边形 p 内部
template<class T>
bool segmentInPolygon(Line<T> l, std::vector<Point<T>> p) {
    int n = p.size();
    if (!pointInPolygon(l.a, p)) {
        return false;
    }
    if (!pointInPolygon(l.b, p)) {
        return false;
    }
    for (int i = 0; i < n; i++) {
        auto u = p[i];
        auto v = p[(i + 1) % n];
        auto w = p[(i + 2) % n];
        auto[t, p1, p2] = segmentIntersection(l, Line<T>(u, v));
 
        if (t == 1) {
            return false;
        }
        if (t == 0) {
            continue;
        }
        if (t == 2) {
            if (pointOnSegment(v, l) && v != l.a && v != l.b) {
                if (cross(v - u, w - v) > 0) {
                    return false;
                }
            }
        } else {
            if (p1 != u && p1 != v) {
                if (pointOnLineLeft(l.a, Line<T>(v, u))
                    || pointOnLineLeft(l.b, Line<T>(v, u))) {
                    return false;
                }
            } else if (p1 == v) {
                if (l.a == v) {
                    if (pointOnLineLeft(u, l)) {
                        if (pointOnLineLeft(w, l)
                            && pointOnLineLeft(w, Line<T>(u, v))) {
                            return false;
                        }
                    } else {
                        if (pointOnLineLeft(w, l)
                            || pointOnLineLeft(w, Line<T>(u, v))) {
                            return false;
                        }
                    }
                } else if (l.b == v) {
                    if (pointOnLineLeft(u, Line<T>(l.b, l.a))) {
                        if (pointOnLineLeft(w, Line<T>(l.b, l.a))
                            && pointOnLineLeft(w, Line<T>(u, v))) {
                            return false;
                        }
                    } else {
                        if (pointOnLineLeft(w, Line<T>(l.b, l.a))
                            || pointOnLineLeft(w, Line<T>(u, v))) {
                            return false;
                        }
                    }
                } else {
                    if (pointOnLineLeft(u, l)) {
                        if (pointOnLineLeft(w, Line<T>(l.b, l.a))
                            || pointOnLineLeft(w, Line<T>(u, v))) {
                            return false;
                        }
                    } else {
                        if (pointOnLineLeft(w, l)
                            || pointOnLineLeft(w, Line<T>(u, v))) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}
 
template<class T>
std::vector<Point<T>> hp(std::vector<Line<T>> lines) {
    std::sort(lines.begin(), lines.end(), [&](auto l1, auto l2) {
        auto d1 = l1.b - l1.a;
        auto d2 = l2.b - l2.a;
 
        if (sgn(d1) != sgn(d2)) {
            return sgn(d1) == 1;
        }
 
        return cross(d1, d2) > 0;
    });
 
    std::deque<Line<T>> ls;
    std::deque<Point<T>> ps;
    for (auto l: lines) {
        if (ls.empty()) {
            ls.push_back(l);
            continue;
        }
 
        while (!ps.empty() && !pointOnLineLeft(ps.back(), l)) {
            ps.pop_back();
            ls.pop_back();
        }
 
        while (!ps.empty() && !pointOnLineLeft(ps[0], l)) {
            ps.pop_front();
            ls.pop_front();
        }
 
        if (cross(l.b - l.a, ls.back().b - ls.back().a) == 0) {
            if (dot(l.b - l.a, ls.back().b - ls.back().a) > 0) {
 
                if (!pointOnLineLeft(ls.back().a, l)) {
                    assert(ls.size() == 1);
                    ls[0] = l;
                }
                continue;
            }
            return {};
        }
 
        ps.push_back(lineIntersection(ls.back(), l));
        ls.push_back(l);
    }
 
    while (!ps.empty() && !pointOnLineLeft(ps.back(), ls[0])) {
        ps.pop_back();
        ls.pop_back();
    }
    if (ls.size() <= 2) {
        return {};
    }
    ps.push_back(lineIntersection(ls[0], ls.back()));
 
    return std::vector<T>(ps.begin(), ps.end());
}
 
void DAOQI() {
    std::cout << "0 0\n";
}
 
signed main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int T = 1;
    //std::cin >> T;
    while (T--) DAOQI();
    return 0;
}