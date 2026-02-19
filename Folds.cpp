#include <bits/stdc++.h>
// Implemented by Claude Opus 4.6

// =====================================================================
// Monoid concept
// =====================================================================
// Monoid M は以下の3つを静的メンバとして提供する構造体:
//   - M::Type        … モノイドの台集合の型 (nested type)
//   - M::op(a, b)    … 二項演算 (static member function)
//   - M::id()        … 単位元   (static member function)
//
// 使い方:
//   template <Monoid M> class SomeDataStructure { ... };
//   のように、テンプレート引数をコンセプトで制約する。
// =====================================================================

template <typename M>
concept Monoid = requires {
    // 1. 台集合の型 M::Type が存在すること
    typename M::Type;
} && requires (typename M::Type a, typename M::Type b) {
    // 2. 二項演算 M::op(a, b) が存在し、戻り値型が M::Type に変換可能であること
    { M::op(a, b) } -> std::convertible_to<typename M::Type>;

    // 3. 単位元 M::id() が存在し、戻り値型が M::Type に変換可能であること
    { M::id() } -> std::convertible_to<typename M::Type>;
};

// =====================================================================
// folds: 静的配列に対する左・右からの累積fold
// =====================================================================
// 要素数 N の配列 a[0..N) に対して:
//   fold_left(r)  = op(a[0], op(a[1], ... op(a[r-2], a[r-1])...))  = op[0, r)
//   fold_right(l) = op(a[l], op(a[l+1], ... op(a[N-2], a[N-1])...)) = op[l, N)
//
// 構築 O(N), クエリ O(1)
// 逆元を要求しない（群でなくモノイドで十分）
// =====================================================================

template <Monoid M>
class folds {
  private:
    using T = typename M::Type;
    int n;
    std::vector<T> prefix; // prefix[i] = op[0, i)
    std::vector<T> suffix; // suffix[i] = op[i, N)

  public:
    folds() = default;

    explicit folds(const std::vector<T>& a) : n((int)a.size()), prefix(n + 1), suffix(n + 1) {
        prefix[0] = M::id();
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = M::op(prefix[i], a[i]);
        }
        suffix[n] = M::id();
        for (int i = n - 1; i >= 0; i--) {
            suffix[i] = M::op(a[i], suffix[i + 1]);
        }
    }

    // op[0, r)  (0 <= r <= n)
    T fold_left(int r) const {
        assert(0 <= r && r <= n);
        return prefix[r];
    }

    // op[l, N)  (0 <= l <= n)
    T fold_right(int l) const {
        assert(0 <= l && l <= n);
        return suffix[l];
    }

    // a[l,r]を省いたall_op l!=0に注意
    // 直観的には、(1-indexedで)要素l...rを省く
    T fold_without_range(int l, int r) const {
        assert(0 < l && l <= r && r <= n);
        return M::op(prefix[l-1],suffix[r]);
    }

    // a[i]を省いたN-1項のall_op without_range(i,i)と等価
    // 直観的には、(1-indexedで)要素iのみを省く
    T fold_without1(int i) const {
        assert(0 < i && i <= n);
        return M::op(prefix[i-1],suffix[i]);
    }

    int size() const { return n; }
};

// =====================================================================
// モノイドの定義例: (long long, +, 0)
// =====================================================================

struct AddLL {
    using Type = long long;
    static Type op(Type a, Type b) { return a + b; }
    static Type id() { return 0; }
};

// 他のモノイド定義例 (参考):

// struct MaxInt {
//     using Type = int;
//     static Type op(Type a, Type b) { return std::max(a, b); }
//     static Type id() { return INT_MIN; }  // -∞
// };

// struct MinInt {
//     using Type = int;
//     static Type op(Type a, Type b) { return std::min(a, b); }
//     static Type id() { return INT_MAX; }  // +∞
// };

// struct MulLL {
//     using Type = long long;
//     static Type op(Type a, Type b) { return a * b; }
//     static Type id() { return 1; }
// };
