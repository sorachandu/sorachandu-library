#include "../../data_structure/LazySegmentTree.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <cstdint>
#include <ostream>
#include <random>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

// 区間和と要素数を保持する集約値。
struct SumLength {
    int sum;
    int length;
    SumLength() = delete;
    constexpr SumLength(int sum_, int length_) : sum(sum_), length(length_) {}
    friend bool operator==(const SumLength&, const SumLength&) = default;
};

// 等値比較演算子を持たないアフィン変換。
struct Affine {
    int a;
    int b;
    Affine() = delete;
    constexpr Affine(int a_, int b_) : a(a_), b(b_) {}
};

// 区間和を要素数込みで結合する遅延モノイド。
struct SumLengthMonoid {
    using T = SumLength;
    using F = Affine;
    static constexpr T op(T a, T b) { return {a.sum + b.sum, a.length + b.length}; }
    static constexpr T e() { return {0, 0}; }
    static constexpr T mapping(F f, T x) {
        return {f.a * x.sum + f.b * x.length, x.length};
    }
    static constexpr F composition(F f, F g) {
        return {f.a * g.a, f.a * g.b + f.b};
    }
    static constexpr F id() { return {1, 0}; }
};

// 3文字の置換を表す変換値。
struct LetterMap {
    std::array<unsigned char, 3> map;
    LetterMap() = delete;
    constexpr LetterMap(unsigned char a, unsigned char b, unsigned char c) : map{a, b, c} {}
};

// 文字列連結に文字置換を組み合わせた遅延モノイド。
struct StringMonoid {
    using T = std::string;
    using F = LetterMap;
    static T op(const T& a, const T& b) { return a + b; }
    static T e() { return {}; }
    static T mapping(F f, const T& value) {
        std::string result;
        result.reserve(value.size());
        for (char c : value) result.push_back(static_cast<char>('a' + f.map[c - 'a']));
        return result;
    }
    static constexpr F composition(F f, F g) {
        return {f.map[g.map[0]], f.map[g.map[1]], f.map[g.map[2]]};
    }
    static constexpr F id() { return {0, 1, 2}; }
};

// 出力できない値の型。
struct NoStream {
    int value;
    NoStream() = delete;
    constexpr explicit NoStream(int value_) : value(value_) {}
};

// 出力できない値を保持し、乗算作用を受けるモノイド。
struct NoStreamMonoid {
    using T = NoStream;
    using F = Affine;
    static constexpr T op(T a, T b) { return T(a.value + b.value); }
    static constexpr T e() { return T(0); }
    static constexpr T mapping(F f, T x) { return T(f.a * x.value); }
    static constexpr F composition(F f, F g) { return {f.a * g.a, 0}; }
    static constexpr F id() { return {1, 0}; }
};

// 出力演算子の有無を調べるコンセプト。
template <class T>
concept HasOstreamInsertion = requires(std::ostream& out, T& value) {
    out << value;
};

static_assert(sora::Monoid<SumLengthMonoid>);
static_assert(sora::LazyMonoid<SumLengthMonoid>);
static_assert(sora::LazyMonoid<StringMonoid>);
static_assert(sora::LazyMonoid<NoStreamMonoid>);
static_assert(!sora::LazyMonoid<SumLength>);
static_assert(!std::default_initializable<SumLength>);
static_assert(!std::default_initializable<Affine>);
static_assert(!std::default_initializable<LetterMap>);
static_assert(!std::default_initializable<NoStream>);
static_assert(!HasOstreamInsertion<NoStream>);
static_assert(HasOstreamInsertion<sora::LazySegmentTree<StringMonoid>>);
static_assert(!HasOstreamInsertion<sora::LazySegmentTree<NoStreamMonoid>>);

bool sum_at_most_20(SumLength x) { return x.sum <= 20; }

bool is_prefix_of(const std::string& value, const std::string& target) {
    return value.size() <= target.size() && target.compare(0, value.size(), value) == 0;
}

bool is_suffix_of(const std::string& value, const std::string& target) {
    return value.size() <= target.size() &&
           target.compare(target.size() - value.size(), value.size(), value) == 0;
}

std::string brute_join(const std::vector<std::string>& values, int left, int right) {
    std::string result;
    for (int i = left; i < right; ++i) result += values[i];
    return result;
}

std::string random_word(std::mt19937& rng, int length) {
    std::string result;
    for (int i = 0; i < length; ++i)
        result.push_back(static_cast<char>('a' + rng() % 3));
    return result;
}

void check_affine_tree(std::vector<int> values, std::mt19937& rng) {
    std::vector<SumLength> initial;
    for (int value : values) initial.emplace_back(value, 1);
    sora::LazySegmentTree<SumLengthMonoid> tree(initial);
    const int n = static_cast<int>(values.size());

    auto check_all_ranges = [&] {
        for (int left = 0; left <= static_cast<int>(values.size()); ++left) {
            SumLength expected(0, 0);
            for (int right = left; right <= static_cast<int>(values.size()); ++right) {
                if (right > left) expected = SumLengthMonoid::op(expected, {values[right - 1], 1});
                assert(tree.prod(left, right) == expected);
            }
        }
        int total = 0;
        for (int value : values) total += value;
        assert(tree.all_prod() == SumLength(total, static_cast<int>(values.size())));
    };
    check_all_ranges();

    tree.apply(0, n, Affine(2, 1));
    for (int& value : values) value = 2 * value + 1;
    auto copied = tree;
    sora::LazySegmentTree<SumLengthMonoid> assigned;
    assigned = copied;
    assert(assigned.all_prod() == tree.all_prod());
    assigned.apply(0, n, Affine(1, 3));
    for (int& value : values) value += 3;
    int assigned_total = 0;
    for (int value : values) assigned_total += value;
    assert(assigned.all_prod() == SumLength(assigned_total, n));
    assert(tree.all_prod() != assigned.all_prod() || n == 0);
    tree = assigned;

    // 探索前はprodを呼ばず、遅延作用が残った状態を保つ。
    for (int left = 0; left <= n; ++left) {
        int sum = 0, expected = left;
        while (expected < n && sum + values[expected] <= 20)
            sum += values[expected++];
        assert(tree.find_last(left, [](SumLength x) { return x.sum <= 20; }) == expected);
        assert(tree.template find_last<sum_at_most_20>(left) == expected);
    }
    for (int right = 0; right <= n; ++right) {
        int sum = 0, expected = right;
        while (expected > 0 && values[expected - 1] + sum <= 20)
            sum += values[--expected];
        assert(tree.find_first(right, [](SumLength x) { return x.sum <= 20; }) == expected);
        assert(tree.template find_first<sum_at_most_20>(right) == expected);
    }
    check_all_ranges();

    for (int iteration = 0; iteration < 100; ++iteration) {
        // 重なる作用をprodで伝播させずに続けて合成する。
        for (int update = 0; update < 3; ++update) {
            int left = static_cast<int>(rng() % (n + 1));
            int right = static_cast<int>(rng() % (n + 1));
            if (left > right) std::swap(left, right);
            const Affine f(static_cast<int>(rng() % 3), static_cast<int>(rng() % 5) - 2);
            tree.apply(left, right, f);
            for (int i = left; i < right; ++i) values[i] = f.a * values[i] + f.b;
        }
        check_all_ranges();

        if (n > 0) {
            const int p = static_cast<int>(rng() % n);
            const int value = static_cast<int>(rng() % 11) - 5;
            tree.set(p, SumLength(value, 1));
            values[p] = value;
            assert(tree.get(p) == SumLength(value, 1));
            check_all_ranges();

            const Affine point_action(static_cast<int>(rng() % 3), static_cast<int>(rng() % 5) - 2);
            tree.apply(p, point_action);
            values[p] = point_action.a * values[p] + point_action.b;
            check_all_ranges();
        }
    }
}

void check_letter_tree(std::vector<std::string> values, std::mt19937& rng) {
    sora::LazySegmentTree<StringMonoid> tree(values);
    const int n = static_cast<int>(values.size());
    for (int iteration = 0; iteration < 120; ++iteration) {
        for (int update = 0; update < 3; ++update) {
            int left = static_cast<int>(rng() % (n + 1));
            int right = static_cast<int>(rng() % (n + 1));
            if (left > right) std::swap(left, right);
            const LetterMap f(static_cast<unsigned char>(rng() % 3),
                              static_cast<unsigned char>(rng() % 3),
                              static_cast<unsigned char>(rng() % 3));
            tree.apply(left, right, f);
            for (int i = left; i < right; ++i) values[i] = StringMonoid::mapping(f, values[i]);
        }

        const int start = static_cast<int>(rng() % (n + 1));
        const std::string target = random_word(rng, static_cast<int>(rng() % (2 * n + 1)));
        std::string prefix;
        int expected_right = start;
        while (expected_right < n &&
               is_prefix_of(prefix + values[expected_right], target))
            prefix += values[expected_right++];
        assert(tree.find_last(start, [&](const std::string& x) {
            return is_prefix_of(x, target);
        }) == expected_right);

        const int end = static_cast<int>(rng() % (n + 1));
        const std::string left_target = random_word(rng, static_cast<int>(rng() % (2 * n + 1)));
        std::string suffix;
        int expected_left = end;
        while (expected_left > 0 &&
               is_suffix_of(values[expected_left - 1] + suffix, left_target))
            suffix = values[--expected_left] + suffix;
        assert(tree.find_first(end, [&](const std::string& x) {
            return is_suffix_of(x, left_target);
        }) == expected_left);

        for (int l = 0; l <= n; ++l)
            for (int r = l; r <= n; ++r)
                assert(tree.prod(l, r) == brute_join(values, l, r));
    }

    tree.apply(0, n, LetterMap(1, 2, 0));
    for (int i = 0; i < n; ++i) values[i] = StringMonoid::mapping(LetterMap(1, 2, 0), values[i]);
    const std::string all_before = tree.all_prod();
    std::ostringstream output;
    output << tree << ":tail";
    std::string expected = "[";
    for (int i = 0; i < n; ++i) {
        if (i) expected += ", ";
        expected += values[i];
    }
    expected += "]:tail";
    assert(output.str() == expected);
    assert(tree.all_prod() == all_before);
    assert(tree.prod(0, n) == all_before);

    tree.apply(0, n, LetterMap(1, 2, 0));
    for (int i = 0; i < n; ++i) values[i] = StringMonoid::mapping(LetterMap(1, 2, 0), values[i]);
    assert(tree.all_prod() == brute_join(values, 0, n));
}

int main() {
    std::mt19937 rng(20261112);

    sora::LazySegmentTree<SumLengthMonoid> empty;
    assert(empty.empty() && empty.size() == 0);
    assert(empty.all_prod() == SumLength(0, 0));
    assert(empty.prod(0, 0) == SumLength(0, 0));
    empty.apply(0, 0, Affine(4, 7));

    sora::LazySegmentTree<SumLengthMonoid> sized(5);
    assert(sized.size() == 5 && !sized.empty());
    assert(sized.all_prod() == SumLength(0, 0));
    for (int i = 0; i < 5; ++i) assert(sized.get(i) == SumLength(0, 0));
    sized.apply(0, 5, Affine(1, 9));
    assert(sized.all_prod() == SumLength(0, 0));

    sora::LazySegmentTree<NoStreamMonoid> no_stream(
        std::vector<NoStream>{NoStream(3), NoStream(8)});
    assert(no_stream.prod(0, 2).value == 11);

    for (int n = 0; n <= 17; ++n) {
        std::vector<int> values(n);
        for (int& value : values) value = static_cast<int>(rng() % 9);
        check_affine_tree(values, rng);

        std::vector<std::string> strings(n);
        for (std::string& value : strings) {
            const int length = static_cast<int>(rng() % 3);
            for (int j = 0; j < length; ++j)
                value.push_back(static_cast<char>('a' + rng() % 3));
        }
        check_letter_tree(strings, rng);
    }

    std::vector<std::string> empty_strings;
    sora::LazySegmentTree<StringMonoid> empty_string_tree(empty_strings);
    std::ostringstream empty_output;
    empty_output << empty_string_tree << ":tail";
    assert(empty_output.str() == "[]:tail");
}
