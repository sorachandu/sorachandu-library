#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <limits>
#include <optional>
#include <set>
#include <utility>

namespace sora {

// 整数の半開区間[l, r)を管理し、重なる区間・隣接区間を併合する。
// Mを保持区間数として空間O(M)。端点はlong longの範囲内とする。
class IntervalSet {
public:
    using Coordinate = long long;
    using ll = Coordinate;  // 既存コードとの互換用。
    using Interval = std::pair<ll, ll>;

private:
    std::set<Interval> s;

public:
    // [l, r)を追加する。l >= rなら何もしない。 O(log(M + 1) + k)、kは併合する区間数。
    void insert(ll l, ll r) {
        if (l >= r) return;
        auto it = s.lower_bound({l, l});
        if (it != s.begin() && std::prev(it)->second >= l)
            --it;
        while (it != s.end() && it->first <= r) {
            l = std::min(l, it->first);
            r = std::max(r, it->second);
            it = s.erase(it);
        }
        s.emplace_hint(it, l, r);
    }

    // [l, r)を削除する。l >= rなら何もしない。 O(log(M + 1) + k)、kは交差する区間数。
    void erase(ll l, ll r) {
        if (l >= r) return;
        auto it = s.lower_bound({l, l});
        if (it != s.begin() && std::prev(it)->second > l)
            --it;
        while (it != s.end() && it->first < r) {
            auto [a, b] = *it;
            it = s.erase(it);
            if (a < l) s.emplace_hint(it, a, l);
            if (r < b) {
                s.emplace_hint(it, r, b);
                break;
            }
        }
    }

    // xを含む区間を返す。存在しなければstd::nullopt。O(log(M + 1))。
    std::optional<Interval> find_interval(ll x) const {
        auto it = s.upper_bound({x, std::numeric_limits<ll>::max()});
        if (it == s.begin()) return std::nullopt;
        --it;
        if (x < it->second) return *it;
        return std::nullopt;
    }

    // xが集合に含まれるかを返す。O(log(M + 1))。
    bool contains(ll x) const {
        return find_interval(x).has_value();
    }

    // [l, r)全体が含まれるかを返す。l >= rならtrue。O(log(M + 1))。
    bool contains(ll l, ll r) const {
        if (l >= r) return true;
        auto p = find_interval(l);
        return p && r <= p->second;
    }

    // x以上で集合に含まれない最小の整数を返す。O(log(M + 1))。
    ll mex(ll x = 0) const {
        auto p = find_interval(x);
        return p ? p->second : x;
    }

    // [l, r)と交差するかを返す。l >= rならfalse。O(log(M + 1))。
    bool intersects(ll l, ll r) const {
        if (l >= r) return false;
        auto it = s.lower_bound({l, l});
        if (it != s.end() && it->first < r) return true;
        return it != s.begin() && std::prev(it)->second > l;
    }

    // 区間を左端の昇順で参照する。O(1)。変更操作で反復子が無効になり得る。
    const std::set<Interval>& intervals() const { return s; }
    std::size_t size() const { return s.size(); }
    bool empty() const { return s.empty(); }
    // 全区間を削除する。O(M)。
    void clear() { s.clear(); }
};

}  // namespace sora
