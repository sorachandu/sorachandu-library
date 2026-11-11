#include "../../data_structure/SegmentTree.hpp"

#include <cassert>
#include <cstdint>
#include <random>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

// 整数の加算モノイド。
struct SumMonoid {
    using T = long long;
    static T op(T a, T b) { return a + b; }
    static constexpr T e() { return 0; }
};

// 単位元が0と異なるモノイド。
struct OffsetMonoid {
    using T = int;
    static constexpr T op(T a, T b) { return a + b - e(); }
    static constexpr T e() { return 13; }
};

// 順序を検証する文字列連結モノイド。
struct StringMonoid {
    using T = std::string;
    static T op(const T& a, const T& b) { return a + b; }
    static T e() { return {}; }
};

// 2行2列の整数行列。
struct Matrix {
    int a00, a01, a10, a11;
    friend bool operator==(const Matrix&, const Matrix&) = default;
};

// 非可換な行列積のモノイド。
struct MatrixMonoid {
    using T = Matrix;
    static T op(T a, T b) {
        constexpr int mod = 101;
        return {(a.a00 * b.a00 + a.a01 * b.a10) % mod,
                (a.a00 * b.a01 + a.a01 * b.a11) % mod,
                (a.a10 * b.a00 + a.a11 * b.a10) % mod,
                (a.a10 * b.a01 + a.a11 * b.a11) % mod};
    }
    static constexpr T e() { return {1, 0, 0, 1}; }
};

// 出力演算子を持たない値の型。
struct NoStream {
    int value;
    friend bool operator==(const NoStream&, const NoStream&) = default;
};

// 出力できない値を使う加算モノイド。
struct NoStreamMonoid {
    using T = NoStream;
    static T op(T a, T b) { return {a.value + b.value}; }
    static constexpr T e() { return {0}; }
};

template <class T>
concept HasOstreamInsertion = requires(std::ostream& out, const T& value) {
    out << value;
};

static_assert(sora::Monoid<SumMonoid>);
static_assert(sora::Monoid<NoStreamMonoid>);
static_assert(!HasOstreamInsertion<NoStream>);
static_assert(!HasOstreamInsertion<sora::SegmentTree<NoStreamMonoid>>);

bool sum_at_most_17(long long value) { return value <= 17; }

std::string random_string(std::mt19937& rng, int length) {
    std::string result;
    for (int i = 0; i < length; ++i)
        result.push_back(static_cast<char>('a' + rng() % 3));
    return result;
}

bool is_prefix_of(const std::string& candidate, const std::string& target) {
    return candidate.size() <= target.size() &&
           target.compare(0, candidate.size(), candidate) == 0;
}

bool is_suffix_of(const std::string& candidate, const std::string& target) {
    return candidate.size() <= target.size() &&
           target.compare(target.size() - candidate.size(), candidate.size(), candidate) == 0;
}

void check_sum_tree(std::vector<long long> values) {
    sora::SegmentTree<SumMonoid> tree(values);
    assert(tree.size() == static_cast<int>(values.size()));
    assert(tree.empty() == values.empty());
    long long total = 0;
    for (long long value : values) total += value;
    assert(tree.all_prod() == total);

    for (int left = 0; left <= static_cast<int>(values.size()); ++left) {
        long long expected = 0;
        for (int right = left; right <= static_cast<int>(values.size()); ++right) {
            if (right > left) expected += values[right - 1];
            assert(tree.prod(left, right) == expected);
        }
    }
    for (int left = 0; left <= static_cast<int>(values.size()); ++left) {
        long long sum = 0;
        int expected = left;
        while (expected < static_cast<int>(values.size()) && sum + values[expected] <= 17)
            sum += values[expected++];
        assert(tree.find_last(left, [=](long long x) { return x <= 17; }) == expected);
        assert((tree.template find_last<sum_at_most_17>(left)) == expected);
    }
    for (int right = 0; right <= static_cast<int>(values.size()); ++right) {
        long long sum = 0;
        int expected = right;
        while (expected > 0 && values[expected - 1] + sum <= 17)
            sum += values[--expected];
        assert(tree.find_first(right, [=](long long x) { return x <= 17; }) == expected);
        assert((tree.template find_first<sum_at_most_17>(right)) == expected);
    }
}

void check_string_tree(std::vector<std::string> values, std::mt19937& rng) {
    sora::SegmentTree<StringMonoid> tree(values);
    auto brute_prod = [&](int left, int right) {
        std::string result;
        for (int i = left; i < right; ++i) result += values[i];
        return result;
    };

    for (int left = 0; left <= static_cast<int>(values.size()); ++left)
        for (int right = left; right <= static_cast<int>(values.size()); ++right)
            assert(tree.prod(left, right) == brute_prod(left, right));
    assert(tree.all_prod() == brute_prod(0, static_cast<int>(values.size())));

    for (int iteration = 0; iteration < 100; ++iteration) {
        const int left = static_cast<int>(rng() % (values.size() + 1));
        const int right = left + static_cast<int>(rng() % (values.size() - left + 1));
        const std::string target = random_string(rng, static_cast<int>(rng() % 12));

        std::string prefix;
        int expected_right = left;
        while (expected_right < static_cast<int>(values.size()) &&
               is_prefix_of(prefix + values[expected_right], target))
            prefix += values[expected_right++];
        assert(tree.find_last(left, [&](const std::string& x) {
            return is_prefix_of(x, target);
        }) == expected_right);

        std::string suffix;
        int expected_left = right;
        while (expected_left > 0 &&
               is_suffix_of(values[expected_left - 1] + suffix, target))
            suffix = values[--expected_left] + suffix;
        assert(tree.find_first(right, [&](const std::string& x) {
            return is_suffix_of(x, target);
        }) == expected_left);
    }

    std::ostringstream output;
    output << tree << ":tail";
    std::string expected = "[";
    for (int i = 0; i < static_cast<int>(values.size()); ++i) {
        if (i) expected += ", ";
        expected += values[i];
    }
    expected += "]:tail";
    assert(output.str() == expected);
}

void check_matrix_tree(const std::vector<Matrix>& values) {
    sora::SegmentTree<MatrixMonoid> tree(values);
    for (int left = 0; left <= static_cast<int>(values.size()); ++left) {
        for (int right = left; right <= static_cast<int>(values.size()); ++right) {
            Matrix expected = MatrixMonoid::e();
            for (int i = left; i < right; ++i) expected = MatrixMonoid::op(expected, values[i]);
            assert(tree.prod(left, right) == expected);
        }
    }
}

int main() {
    // 空列・0以外の単位元・非2冪長。
    sora::SegmentTree<SumMonoid> empty;
    assert(empty.empty() && empty.size() == 0 && empty.all_prod() == 0);
    assert(empty.prod(0, 0) == 0);
    std::vector<long long> empty_values;
    check_sum_tree(empty_values);

    sora::SegmentTree<OffsetMonoid> offset_empty;
    assert(offset_empty.empty() && offset_empty.all_prod() == 13);
    sora::SegmentTree<OffsetMonoid> offset_tree(5);
    assert(offset_tree.size() == 5 && offset_tree.all_prod() == 13);
    std::vector<int> offset_values(5, 13);
    for (int i = 0; i < 5; ++i) {
        assert(offset_tree.get(i) == 13);
        offset_tree.set(i, i + 20);
        offset_values[i] = i + 20;
    }
    for (int left = 0; left <= 5; ++left) {
        int expected = 13;
        for (int right = left; right <= 5; ++right) {
            if (right > left) expected = OffsetMonoid::op(expected, offset_values[right - 1]);
            assert(offset_tree.prod(left, right) == expected);
        }
    }

    std::ostringstream empty_output;
    empty_output << empty << ":tail";
    assert(empty_output.str() == "[]:tail");

    sora::SegmentTree<NoStreamMonoid> no_stream(std::vector<NoStream>{{4}, {7}, {-2}});
    assert(no_stream.prod(0, 3).value == 9);

    std::mt19937 rng(831721);
    for (int n = 0; n <= 20; ++n) {
        std::vector<long long> values(n);
        for (long long& x : values) x = static_cast<int>(rng() % 11);
        check_sum_tree(values);

        sora::SegmentTree<SumMonoid> tree(values);
        auto copied = tree;
        sora::SegmentTree<SumMonoid> assigned;
        assigned = copied;
        assert(assigned.all_prod() == tree.all_prod());
        if (n > 0) {
            for (int update = 0; update < 30; ++update) {
                const int p = static_cast<int>(rng() % n);
                const long long value = static_cast<int>(rng() % 50);
                const long long old_value = tree.get(p);
                assigned.set(p, value);
                values[p] = value;
                assert(assigned.get(p) == value);
                assert(tree.get(p) == old_value);
                long long total = 0;
                for (long long x : values) total += x;
                assert(assigned.all_prod() == total);
                for (int left = 0; left <= n; ++left) {
                    long long expected = 0;
                    for (int right = left; right <= n; ++right) {
                        if (right > left) expected += values[right - 1];
                        assert(assigned.prod(left, right) == expected);
                    }
                }
            }
        }

        std::vector<std::string> strings(n);
        for (auto& x : strings) x = random_string(rng, static_cast<int>(rng() % 3));
        check_string_tree(strings, rng);

        std::vector<Matrix> matrices(n);
        for (auto& x : matrices)
            x = {static_cast<int>(rng() % 10), static_cast<int>(rng() % 10),
                 static_cast<int>(rng() % 10), static_cast<int>(rng() % 10)};
        check_matrix_tree(matrices);
    }
}
