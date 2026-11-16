#include "../../utility/RandomGenerator.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdint>
#include <limits>
#include <map>
#include <numeric>
#include <random>
#include <ranges>
#include <set>
#include <span>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

using Random = sora::RandomGenerator<>;

template<class T>
concept AcceptsMixedSigns = requires(T& rnd) { rnd.randint(-1, 2U); };
template<class T>
concept AcceptsBool = requires(T& rnd) { rnd.randint(false, true); };

static_assert(std::uniform_random_bit_generator<Random>);
static_assert(!AcceptsMixedSigns<Random>);
static_assert(!AcceptsBool<Random>);
static_assert(std::same_as<decltype(std::declval<Random&>().randint(0, 1LL)), long long>);
static_assert(std::same_as<decltype(std::declval<Random&>().choice(std::declval<const std::vector<int>&>())), int>);
static_assert(std::same_as<decltype(std::declval<Random&>().choice(std::declval<std::vector<bool>&>())), bool>);

template<class T>
void check_unique(const std::vector<T>& values) {
    assert(std::set<T>(values.begin(), values.end()).size() == values.size());
}

template<class Engine>
void check_state() {
    sora::RandomGenerator<Engine> rnd(12345);
    Engine engine(12345);
    for (int i = 0; i < 2000; ++i) assert(rnd() == engine());
    rnd.seed(54321);
    engine.seed(54321);
    for (int i = 0; i < 2000; ++i) assert(rnd() == engine());

    const std::vector<int> values{1, 3, 5, 7, 9};
    const std::array<double, 5> weights{1, 0, 3, 5, 0};
    const auto exercise = [&](auto& target) {
        std::vector<std::uint64_t> result;
        result.push_back(target.getrandbits(64));
        result.push_back(target.randint(0ULL, ULLONG_MAX));
        result.push_back(target.randrange(-100LL, 100LL, 3LL));
        result.push_back(static_cast<std::uint64_t>(target.random() * 1000000));
        result.push_back(static_cast<std::uint64_t>(target.uniform(-1, 1) * 1000000 + 1000000));
        result.push_back(target.bernoulli(0.37));
        for (auto v : target.randbytes(19)) result.push_back(v);
        result.push_back(target.choice(values));
        for (auto v : target.choices(values, 10)) result.push_back(v);
        for (auto v : target.choices(values, 10, weights)) result.push_back(v);
        for (auto v : target.sample(values, 4)) result.push_back(v);
        for (auto v : target.shuffled(values)) result.push_back(v);
        for (auto v : target.permutation(11)) result.push_back(v);
        for (auto v : target.randint_vec(11, 0, 100)) result.push_back(v);
        for (auto v : target.distinct(11, 0, 100)) result.push_back(v);
        for (auto v : target.randstr(11, "abc")) result.push_back(v);
        const auto [left, right] = target.randpair(-50, 100);
        result.push_back(left);
        result.push_back(right);
        for (auto v : target.composition(100, 10)) result.push_back(v);
        for (auto [u, v] : target.tree(20)) result.push_back(u * 20 + v);
        for (bool directed : {false, true})
            for (auto [u, v] : target.graph(20, 30, directed)) result.push_back(u * 20 + v);
        return result;
    };
    const auto state = rnd.getstate();
    const auto expected = exercise(rnd);
    rnd.setstate(state);
    assert(exercise(rnd) == expected);
    auto copied = rnd;
    assert(exercise(copied) == exercise(rnd));
    rnd.seed(12345);
    sora::RandomGenerator<Engine> same(12345);
    assert(exercise(rnd) == exercise(same));

    rnd.seed();
    assert(0 <= rnd.randint(0, 100) && rnd.randint(0, 100) <= 100);
    sora::RandomGenerator<Engine> non_fixed;
    const auto before_zero_bits = non_fixed.getstate();
    assert(non_fixed.getrandbits(0) == 0);
    assert(non_fixed.getstate() == before_zero_bits);
}

void check_numbers(Random& rnd) {
    constexpr auto lo = std::numeric_limits<std::int64_t>::min();
    constexpr auto hi = std::numeric_limits<std::int64_t>::max();
    constexpr auto umax = std::numeric_limits<std::uint64_t>::max();
    std::set<int> all;
    bool upper_half = false, lower_half = false;
    for (int iteration = 0; iteration < 10000; ++iteration) {
        const auto value = rnd.randint(lo, hi);
        upper_half |= value >= 0;
        lower_half |= value < 0;
        assert(rnd.randint(lo, lo) == lo);
        assert(rnd.randint(hi, hi) == hi);
        assert(rnd.randint(umax, umax) == umax);
        assert(rnd.randint(-3, 2LL) >= -3 && rnd.randint(-3, 2LL) <= 2);
        assert(rnd.randint(static_cast<signed char>(-128), static_cast<signed char>(127)) >= -128);
        assert(rnd.randint(char16_t{3}, char16_t{8}) <= 8);
        all.insert(rnd.randint(-2, 2));
        assert(rnd.randrange(1) == 0);
        const int range = rnd.randrange(-20, 30);
        assert(-20 <= range && range < 30);
        const auto whole = rnd.randrange(lo, hi);
        assert(lo <= whole && whole < hi);
        assert(rnd.randrange(umax - 1, umax) == umax - 1);
        const double p = rnd.random();
        assert(0 <= p && p < 1);
        const double q = rnd.uniform(-1e300, 1e300);
        assert(std::isfinite(q) && -1e300 <= q && q < 1e300);
        const double maximum = std::numeric_limits<double>::max();
        const double huge = rnd.uniform(-maximum, maximum);
        assert(std::isfinite(huge) && -maximum <= huge && huge < maximum);
        assert(rnd.uniform(4.5, 4.5) == 4.5);
        assert(rnd.uniform(1.0, std::nextafter(1.0, 2.0)) == 1.0);
        assert(!rnd.bernoulli(0));
        assert(rnd.bernoulli(1));
    }
    assert(upper_half && lower_half && all.size() == 5);

    for (int a = -8; a <= 8; ++a) {
        for (int b = -8; b <= 8; ++b) {
            for (int step = -8; step <= 8; ++step) {
                if (step == 0 || (step > 0 ? a >= b : a <= b)) continue;
                std::set<int> expected, actual;
                for (int x = a; step > 0 ? x < b : x > b; x += step) expected.insert(x);
                for (int iteration = 0; iteration < 1000; ++iteration) {
                    const int x = rnd.randrange(a, b, step);
                    assert(expected.contains(x));
                    actual.insert(x);
                }
                assert(actual == expected);
            }
        }
    }
    for (int iteration = 0; iteration < 1000; ++iteration) {
        const auto x = rnd.randrange(hi, lo, lo);
        assert(x == hi || x == -1);
        const auto y = rnd.randrange(lo, hi, hi);
        assert(y == lo || y == -1 || y == hi - 1);
        const auto u = rnd.randrange(std::uint64_t{0}, umax, umax);
        assert(u == 0);
    }
    for (int k = 0; k <= 64; ++k) {
        std::uint64_t bits = 0;
        for (int iteration = 0; iteration < 100; ++iteration) {
            const auto value = rnd.getrandbits(k);
            if (k < 64) assert(value < (std::uint64_t{1} << k));
            bits |= value;
        }
        assert(bits == (k == 64 ? umax : (std::uint64_t{1} << k) - 1));
    }
    assert(rnd.randbytes(0).empty());
    for (std::size_t n : {1, 7, 8, 9, 16, 1000}) assert(rnd.randbytes(n).size() == n);
    const auto bytes = rnd.randbytes(10000);
    assert(std::set<std::uint8_t>(bytes.begin(), bytes.end()).size() == 256);
}

void check_sequences(Random& rnd) {
    const std::vector<int> empty;
    assert(rnd.choices(empty, 0).empty());
    assert(rnd.choices(empty, 0, std::vector<double>{}).empty());
    assert(rnd.sample(empty, 0).empty());
    assert(rnd.shuffled(empty).empty());
    std::vector<int> mutable_empty;
    rnd.shuffle(mutable_empty);
    const int singleton[]{42};
    assert(rnd.choice(singleton) == 42);
    assert(rnd.choices(singleton, 10) == std::vector<int>(10, 42));
    assert(rnd.sample(std::vector<int>{7, 7, 7}, 3) == std::vector<int>(3, 7));
    std::vector<bool> flags{true, false, true};
    assert(rnd.sample(flags, 3).size() == 3);
    assert(rnd.choice(std::vector<std::string>{"temporary"}) == "temporary");

    for (int n = 1; n <= 40; ++n) {
        std::vector<int> values(n);
        std::iota(values.begin(), values.end(), 0);
        for (int k = 0; k <= n; ++k) {
            const auto samples = rnd.sample(values, k);
            assert(samples.size() == static_cast<std::size_t>(k));
            check_unique(samples);
            for (int v : samples) assert(0 <= v && v < n);
        }
        const auto copy = values;
        auto shuffled = rnd.shuffled(values);
        std::ranges::sort(shuffled);
        assert(shuffled == values && values == copy);
        rnd.shuffle(values);
        std::ranges::sort(values);
        assert(values == copy);
    }
    // 要素数に比例する走査や確保がないことを、巨大な仮想範囲で確認する。
    const auto huge = std::views::iota(std::int64_t{0}, std::int64_t{1000000000000});
    const auto samples = rnd.sample(huge, 10);
    check_unique(samples);
    for (auto x : samples) assert(0 <= x && x < 1000000000000LL);
    const std::array<int, 4> array{10, 20, 30, 40};
    const std::span<const int> span(array);
    assert(rnd.sample(span, 4).size() == 4);
    const std::string text = "abc";
    assert(text.find(rnd.choice(text)) != std::string::npos);
    auto non_common = std::ranges::subrange(std::counted_iterator(array.begin(), 4), std::default_sentinel);
    assert(rnd.sample(non_common, 4).size() == 4);

    const std::array<int, 4> values{0, 1, 2, 3};
    const std::array<double, 4> weights{0, 0, 1, 0};
    assert(rnd.choices(values, 1000, weights) == std::vector<int>(1000, 2));
    const auto weighted = rnd.choices(values, 20000, std::array<double, 4>{1, 2, 0, 7});
    const auto a = std::ranges::count(weighted, 0);
    const auto b = std::ranges::count(weighted, 1);
    const auto c = std::ranges::count(weighted, 3);
    assert(std::ranges::count(weighted, 2) == 0);
    assert(1500 < a && a < 2500 && 3300 < b && b < 4700 && 13000 < c && c < 15000);
}

void check_generators(Random& rnd) {
    assert(rnd.permutation(0).empty());
    assert(rnd.randint_vec(0, -1, 1).empty());
    assert(rnd.distinct(0, 0, 0).empty());
    assert(rnd.randstr(0, "").empty());
    assert(rnd.randstr(100, "x") == std::string(100, 'x'));
    assert(rnd.randpair(0, 1) == std::make_pair(0, 1));
    for (int n = 1; n <= 50; ++n) {
        auto permutation = rnd.permutation(n);
        std::ranges::sort(permutation);
        for (int i = 0; i < n; ++i) assert(permutation[i] == i);
        const auto values = rnd.randint_vec(n, -123, 456LL);
        assert(values.size() == static_cast<std::size_t>(n));
        for (auto v : values) assert(-123 <= v && v <= 456);
        const auto text = rnd.randstr(n, "abc");
        assert(text.size() == static_cast<std::size_t>(n));
        for (auto c : text) assert(c == 'a' || c == 'b' || c == 'c');
        for (int k = 0; k <= n; ++k) {
            const auto distinct = rnd.distinct(k, -n, -1);
            check_unique(distinct);
            assert(distinct.size() == static_cast<std::size_t>(k));
            for (int v : distinct) assert(-n <= v && v <= -1);
        }
    }
    const auto unsigned_values = rnd.distinct(10000, 0ULL, ULLONG_MAX);
    check_unique(unsigned_values);
    assert(*std::ranges::max_element(unsigned_values) > ULLONG_MAX / 2);
    const auto signed_values = rnd.distinct(10000, LLONG_MIN, LLONG_MAX);
    check_unique(signed_values);
    assert(*std::ranges::min_element(signed_values) < 0);
    assert(*std::ranges::max_element(signed_values) > 0);
    auto near_limit = rnd.distinct(5, ULLONG_MAX - 4, ULLONG_MAX);
    std::ranges::sort(near_limit);
    for (int i = 0; i < 5; ++i) assert(near_limit[i] == ULLONG_MAX - 4 + i);

    assert(rnd.composition(0, 0).empty());
    assert(rnd.composition(LLONG_MAX, 1) == std::vector<std::int64_t>{LLONG_MAX});
    for (int n = 1; n <= 50; ++n) {
        for (int k = 1; k <= n; ++k) {
            const auto parts = rnd.composition(n, k);
            assert(parts.size() == static_cast<std::size_t>(k));
            assert(std::accumulate(parts.begin(), parts.end(), std::int64_t{0}) == n);
            for (auto value : parts) assert(value > 0);
        }
    }
    const auto large = rnd.composition(LLONG_MAX, 100);
    assert(std::accumulate(large.begin(), large.end(), std::int64_t{0}) == LLONG_MAX);
}

void check_tree(const std::vector<std::pair<int, int>>& edges, int n) {
    assert(edges.size() == static_cast<std::size_t>(std::max(0, n - 1)));
    std::vector<int> parent(n);
    std::iota(parent.begin(), parent.end(), 0);
    const auto root = [&](int v) {
        while (v != parent[v]) v = parent[v];
        return v;
    };
    for (auto [u, v] : edges) {
        assert(0 <= u && u < v && v < n);
        assert(root(u) != root(v));
        parent[root(u)] = root(v);
    }
}

void check_graphs(Random& rnd) {
    assert(rnd.tree(0).empty());
    assert(rnd.tree(1).empty());
    for (int n = 2; n <= 100; ++n)
        for (int iteration = 0; iteration < 20; ++iteration) check_tree(rnd.tree(n), n);
    check_tree(rnd.tree(100000), 100000);
    for (bool directed : {false, true}) {
        assert(rnd.graph(0, 0, directed).empty());
        assert(rnd.graph(1, 0, directed).empty());
        for (int n = 2; n <= 15; ++n) {
            const int capacity = n * (n - 1) / (directed ? 1 : 2);
            for (int m = 0; m <= capacity; ++m) {
                const auto edges = rnd.graph(n, m, directed);
                assert(edges.size() == static_cast<std::size_t>(m));
                check_unique(edges);
                for (auto [u, v] : edges) {
                    assert(0 <= u && u < n && 0 <= v && v < n && u != v);
                    if (!directed) assert(u < v);
                }
            }
        }
        const auto sparse = rnd.graph(INT_MAX, 100, directed);
        check_unique(sparse);
        for (auto [u, v] : sparse) assert(0 <= u && u < INT_MAX && 0 <= v && v < INT_MAX && u != v);
    }
    // 固定seedで小さい出力の頻度を確認する。乱数品質の統計的な保証ではない。
    std::map<std::vector<std::pair<int, int>>, int> trees, graphs;
    std::map<std::vector<std::int64_t>, int> compositions;
    std::map<std::pair<int, int>, int> pairs, samples;
    for (int iteration = 0; iteration < 32000; ++iteration) {
        auto edges = rnd.tree(4);
        std::ranges::sort(edges);
        ++trees[edges];
        auto graph = rnd.graph(4, 2);
        std::ranges::sort(graph);
        ++graphs[graph];
        ++compositions[rnd.composition(5, 3)];
        ++pairs[rnd.randpair(0, 3)];
        auto sample = rnd.sample(std::array<int, 3>{0, 1, 2}, 2);
        ++samples[{sample[0], sample[1]}];
    }
    assert(trees.size() == 16 && graphs.size() == 15 && compositions.size() == 6 && pairs.size() == 6 && samples.size() == 6);
    for (const auto& [key, count] : trees) assert(1500 < count && count < 2500);
    for (const auto& [key, count] : graphs) assert(1600 < count && count < 2700);
    for (const auto& [key, count] : compositions) assert(4500 < count && count < 6200);
    for (const auto& [key, count] : pairs) assert(4500 < count && count < 6200);
    for (const auto& [key, count] : samples) assert(4500 < count && count < 6200);
}

int main() {
    check_state<std::mt19937_64>();
    check_state<std::mt19937>();
    check_state<std::minstd_rand>();
    Random rnd(20261117);
    check_numbers(rnd);
    check_sequences(rnd);
    check_generators(rnd);
    check_graphs(rnd);
}
