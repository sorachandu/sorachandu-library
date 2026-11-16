#include "../../data_structure/FenwickTree.hpp"

#include <bit>
#include <cassert>
#include <cstdint>
#include <random>
#include <vector>

// 加算と減算だけを提供する独自の加法群要素。
struct GroupValue {
    long long value = 0;
    GroupValue() = default;
    constexpr GroupValue(long long value_) : value(value_) {}
    constexpr GroupValue& operator+=(const GroupValue& rhs) {
        value += rhs.value;
        return *this;
    }
    friend constexpr GroupValue operator-(GroupValue lhs, GroupValue rhs) {
        return GroupValue(lhs.value - rhs.value);
    }
};

// 和がlimit以下かを判定する探索用述語。
struct SumAtMost {
    long long limit;
    bool operator()(long long value) const { return value <= limit; }
};

// 述語の評価回数を数え、境界探索が対数回で済むことを確認する。
struct CountingSumAtMost {
    long long limit;
    int* calls;
    bool operator()(long long value) const {
        ++*calls;
        return value <= limit;
    }
};

bool sum_at_most_17(long long value) { return value <= 17; }
bool always_true_group(GroupValue) { return true; }
bool group_sum_at_most_5(GroupValue value) { return value.value <= 5; }
bool always_true_unsigned(std::uint64_t) { return true; }

void check_searches(const std::vector<long long>& values) {
    const sora::FenwickTree<long long> tree(static_cast<int>(values.size()));
    auto mutable_tree = tree;
    for (int i = 0; i < static_cast<int>(values.size()); ++i)
        mutable_tree.add(i, values[i]);
    const auto& const_tree = mutable_tree;

    long long total = 0;
    for (long long x : values) total += x;
    const std::vector<long long> limits{0, 1, 5, 17, total, total + 1};
    for (long long limit : limits) {
        for (int left = 0; left <= static_cast<int>(values.size()); ++left) {
            long long sum = 0;
            int expected = left;
            while (expected < static_cast<int>(values.size()) &&
                   sum + values[expected] <= limit)
                sum += values[expected++];
            assert(const_tree.find_last(left, SumAtMost{limit}) == expected);
        }
        for (int right = 0; right <= static_cast<int>(values.size()); ++right) {
            long long sum = 0;
            int expected = right;
            while (expected > 0 && values[expected - 1] + sum <= limit)
                sum += values[--expected];
            assert(const_tree.find_first(right, SumAtMost{limit}) == expected);
        }
    }

    for (int left = 0; left <= static_cast<int>(values.size()); ++left) {
        long long sum = 0;
        int expected = left;
        while (expected < static_cast<int>(values.size()) && sum + values[expected] <= 17)
            sum += values[expected++];
        assert(const_tree.template find_last<sum_at_most_17>(left) == expected);
    }
    for (int right = 0; right <= static_cast<int>(values.size()); ++right) {
        long long sum = 0;
        int expected = right;
        while (expected > 0 && values[expected - 1] + sum <= 17)
            sum += values[--expected];
        assert(const_tree.template find_first<sum_at_most_17>(right) == expected);
    }

    int calls = 0;
    const int probe_limit = 4 * std::bit_width(static_cast<unsigned int>(values.size() + 1)) + 4;
    (void)const_tree.find_last(0, CountingSumAtMost{total / 2, &calls});
    assert(calls <= probe_limit);
    calls = 0;
    (void)const_tree.find_first(static_cast<int>(values.size()),
                                CountingSumAtMost{total / 2, &calls});
    assert(calls <= probe_limit);
}

void check_updates(std::vector<long long> values, std::mt19937& rng) {
    sora::FenwickTree<long long> tree(static_cast<int>(values.size()));
    for (int i = 0; i < static_cast<int>(values.size()); ++i) tree.add(i, values[i]);
    const int n = static_cast<int>(values.size());

    auto check_all_ranges = [&] {
        long long prefix = 0;
        for (int right = 0; right <= n; ++right) {
            if (right > 0) prefix += values[right - 1];
            assert(tree.sum(right) == prefix);
        }
        for (int left = 0; left <= n; ++left) {
            long long expected = 0;
            for (int right = left; right <= n; ++right) {
                if (right > left) expected += values[right - 1];
                assert(tree.sum(left, right) == expected);
            }
        }
        for (int i = 0; i < n; ++i) assert(tree.get(i) == values[i]);
    };
    check_all_ranges();

    for (int iteration = 0; iteration < 100 && n > 0; ++iteration) {
        const int p = static_cast<int>(rng() % n);
        if (iteration % 2 == 0) {
            const long long delta = static_cast<int>(rng() % 101) - 50;
            tree.add(p, delta);
            values[p] += delta;
        } else {
            const long long value = static_cast<int>(rng() % 201) - 100;
            tree.set(p, value);
            values[p] = value;
        }
        check_all_ranges();
    }

    const auto copy = tree;
    sora::FenwickTree<long long> assigned(0);
    assigned = copy;
    for (int right = 0; right <= n; ++right)
        assert(assigned.sum(right) == tree.sum(right));
}

void check_custom_group() {
    sora::FenwickTree<GroupValue> tree(5);
    const GroupValue values[]{GroupValue(4), GroupValue(-8), GroupValue(11),
                              GroupValue(0), GroupValue(-2)};
    for (int i = 0; i < 5; ++i) tree.add(i, values[i]);
    const auto& const_tree = tree;
    for (int left = 0; left <= 5; ++left) {
        for (int right = left; right <= 5; ++right) {
            GroupValue expected{};
            for (int i = left; i < right; ++i) expected += values[i];
            assert(const_tree.sum(left, right).value == expected.value);
        }
    }
    assert(const_tree.get(2).value == values[2].value);
    assert(const_tree.find_last(1, [](GroupValue) { return true; }) == 5);
    assert(const_tree.find_first(4, [](GroupValue) { return true; }) == 0);
    assert(const_tree.template find_last<always_true_group>(2) == 5);
    assert(const_tree.template find_first<always_true_group>(3) == 0);
    tree.set(1, GroupValue(19));
    assert(tree.get(1).value == 19);

    sora::FenwickTree<GroupValue> nonnegative(5);
    const long long plain[]{2, 0, 1, 5, 0};
    for (int i = 0; i < 5; ++i) nonnegative.add(i, GroupValue(plain[i]));
    assert(nonnegative.find_last(0, [](GroupValue x) { return x.value <= 2; }) == 2);
    assert(nonnegative.template find_last<group_sum_at_most_5>(0) == 3);
    assert(nonnegative.find_first(5, [](GroupValue x) { return x.value <= 5; }) == 3);
}

int main() {
    std::mt19937 rng(314159);

    sora::FenwickTree<long long> empty(0);
    assert(empty.sum(0) == 0);
    assert(empty.sum(0, 0) == 0);
    assert(empty.find_last(0, SumAtMost{0}) == 0);
    assert(empty.find_first(0, SumAtMost{0}) == 0);

    for (int n = 0; n <= 20; ++n) {
        std::vector<long long> zeros(n, 0);
        check_searches(zeros);
        check_updates(zeros, rng);

        std::vector<long long> nonnegative(n);
        for (long long& value : nonnegative) value = static_cast<int>(rng() % 8);
        check_searches(nonnegative);

        std::vector<long long> mixed(n);
        for (long long& value : mixed) value = static_cast<int>(rng() % 201) - 100;
        check_updates(mixed, rng);
    }

    for (int n : {63, 64, 65, 127, 128, 129, 255, 256, 257}) {
        check_searches(std::vector<long long>(n, 0));
        std::vector<long long> nonnegative(n);
        for (long long& value : nonnegative) value = static_cast<int>(rng() % 6);
        check_searches(nonnegative);

        std::vector<long long> mixed(n);
        for (long long& value : mixed) value = static_cast<int>(rng() % 101) - 50;
        check_updates(mixed, rng);
    }

    // 非零要素を両端に置き、ゼロ区間と境界位置を作る。
    for (int n : {63, 64, 65, 127, 128, 129, 255, 256, 257}) {
        std::vector<long long> zero_runs(n, 0);
        if (n > 0) zero_runs[0] = 2;
        if (n > 1) zero_runs[n - 1] = 3;
        check_searches(zero_runs);
    }

    check_custom_group();

    sora::FenwickTree<std::uint64_t> unsigned_tree(4);
    unsigned_tree.add(0, 3);
    unsigned_tree.add(2, 8);
    unsigned_tree.set(0, 5);
    assert(unsigned_tree.get(0) == 5);
    assert(unsigned_tree.sum(0, 4) == 13);
    assert(unsigned_tree.find_last(2, [](std::uint64_t) { return true; }) == 4);
    assert(unsigned_tree.template find_first<always_true_unsigned>(3) == 0);
}
