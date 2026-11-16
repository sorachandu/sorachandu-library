#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <limits>
#include <numeric>
#include <random>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace sora {

// boolと128bit整数を除く、64bit以下の標準整数型。
template<class T>
concept RandomInteger = std::integral<T> && !std::same_as<T, bool> && sizeof(T) <= 8;

// 値をコピーして抽出できる、サイズ付きランダムアクセス範囲。
template<class R>
concept RandomSequence = std::ranges::random_access_range<R> &&
    std::ranges::sized_range<R> &&
    std::constructible_from<std::ranges::range_value_t<R>, std::ranges::range_reference_t<R>>;

// Python風の乱数生成・テストケース生成。固定seedは同一環境・同じ呼び出し順で再現可能。
// Engineは標準の乱数エンジンと同様にコピー、seed(整数)、seed(seed_seq)を提供すること。
// 不正な引数はassertで検出する。shuffle以外は入力を変更しない。
// 以下の計算量はエンジンの1回の生成・固定seedの設定・状態コピーをO(1)とする。
template<std::uniform_random_bit_generator Engine = std::mt19937_64>
class RandomGenerator {
public:
    using engine_type = Engine;
    using result_type = typename Engine::result_type;
    using state_type = Engine;

    // 非固定seedで初期化する。時間はstd::random_deviceに依存する。
    RandomGenerator() { seed(); }

    // 固定seedで初期化する。O(1)。
    explicit RandomGenerator(result_type value) { seed(value); }

    // 固定seedで再初期化する。O(1)。
    void seed(result_type value) { engine_.seed(value); }

    // std::random_device由来のseedで再初期化する。時間は乱数源に依存する。
    void seed() {
        std::random_device source;
        std::array<std::random_device::result_type, 8> values;
        for (auto& value : values) value = source();
        std::seed_seq sequence(values.begin(), values.end());
        engine_.seed(sequence);
    }

    // エンジンの状態をコピーして取得・復元する。各O(1)。
    [[nodiscard]] state_type getstate() const { return engine_; }
    void setstate(const state_type& state) { engine_ = state; }

    // エンジンの生の乱数を返す。ならしO(1)。min/maxと合わせてURBGとして利用可能。
    result_type operator()() { return engine_(); }
    static constexpr result_type min() { return Engine::min(); }
    static constexpr result_type max() { return Engine::max(); }

    // 閉区間[a, b]の一様整数。符号の有無が同じ型の組合せを受け付ける。期待O(1)。
    template<RandomInteger A, RandomInteger B>
        requires (std::is_signed_v<A> == std::is_signed_v<B>)
    std::common_type_t<A, B> randint(A a, B b) {
        using T = std::common_type_t<A, B>;
        assert(static_cast<T>(a) <= static_cast<T>(b));
        using D = std::conditional_t<std::is_signed_v<T>, long long, unsigned long long>;
        return static_cast<T>(std::uniform_int_distribution<D>(a, b)(engine_));
    }

    // [0, b) / [a, b)の一様整数。空区間は不可。期待O(1)。
    template<RandomInteger T>
    T randrange(T b) {
        assert(b > 0);
        return static_cast<T>(randint(T{0}, static_cast<T>(b - 1)));
    }

    template<RandomInteger A, RandomInteger B>
        requires (std::is_signed_v<A> == std::is_signed_v<B>)
    std::common_type_t<A, B> randrange(A a, B b) {
        using T = std::common_type_t<A, B>;
        assert(static_cast<T>(a) < static_cast<T>(b));
        return randint(static_cast<T>(a), static_cast<T>(static_cast<T>(b) - 1));
    }

    // a, a+step, ... からbを含まない方向へ一様に選ぶ。負のstepも可。期待O(1)。
    // step != 0かつ候補が1個以上必要。3引数の符号の有無はそろえる。
    template<RandomInteger A, RandomInteger B, RandomInteger S>
        requires (std::is_signed_v<A> == std::is_signed_v<B> &&
                  std::is_signed_v<A> == std::is_signed_v<S>)
    std::common_type_t<A, B, S> randrange(A a, B b, S step) {
        using T = std::common_type_t<A, B, S>;
        const __int128 start = a, stop = b, stride = step;
        assert(stride != 0);
        assert((stride > 0 && start < stop) || (stride < 0 && start > stop));
        const __int128 distance = stride > 0 ? stop - start : start - stop;
        const __int128 width = stride > 0 ? stride : -stride;
        const auto count = static_cast<std::uint64_t>((distance - 1) / width + 1);
        const auto index = randint(std::uint64_t{0}, count - 1);
        return static_cast<T>(start + stride * static_cast<__int128>(index));
    }

    // [0, 1)の一様実数。期待O(1)。
    double random() {
        const double value = std::generate_canonical<double, std::numeric_limits<double>::digits>(engine_);
        return value < 1.0 ? value : std::nextafter(1.0, 0.0);
    }

    // 有限のa <= bに対して[a, b)の一様実数。a == bならa。期待O(1)。
    double uniform(double a, double b) {
        assert(std::isfinite(a) && std::isfinite(b) && a <= b);
        if (a == b) return a;
        const double value = std::lerp(a, b, random());
        return value < b ? std::max(a, value) : std::nextafter(b, a);
    }

    // 確率pでtrue。有限の0 <= p <= 1が必要。期待O(1)。
    bool bernoulli(double p) {
        assert(std::isfinite(p) && 0.0 <= p && p <= 1.0);
        return std::bernoulli_distribution(p)(engine_);
    }

    // 下位kビットが一様な整数。0 <= k <= 64。k == 0なら0。期待O(1)。
    std::uint64_t getrandbits(int k) {
        assert(0 <= k && k <= 64);
        if (k == 0) return 0;
        const auto value = randint(std::uint64_t{0}, std::numeric_limits<std::uint64_t>::max());
        return value >> (64 - k);
    }

    // 一様なnバイトを返す。期待時間・出力空間O(n)。
    std::vector<std::uint8_t> randbytes(std::size_t n) {
        std::vector<std::uint8_t> result(n);
        std::uint64_t bits = 0;
        for (std::size_t i = 0; i < n; ++i) {
            if (i % 8 == 0) bits = getrandbits(64);
            result[i] = static_cast<std::uint8_t>(bits);
            bits >>= 8;
        }
        return result;
    }

    // 非空範囲から1要素を値で返す。期待O(1)、要素のコピーはO(1)とする。
    template<RandomSequence R>
    std::ranges::range_value_t<R> choice(R&& range) {
        const auto n = std::ranges::size(range);
        assert(n > 0);
        return at(range, randint(std::uint64_t{0}, static_cast<std::uint64_t>(n) - 1));
    }

    // 重複を許してk個抽出する。空範囲はk == 0のときだけ可。期待時間・出力空間O(k)。
    template<RandomSequence R>
    std::vector<std::ranges::range_value_t<R>> choices(R&& range, std::size_t k) {
        assert(k == 0 || std::ranges::size(range) > 0);
        std::vector<std::ranges::range_value_t<R>> result;
        result.reserve(k);
        for (std::size_t i = 0; i < k; ++i) result.push_back(choice(range));
        return result;
    }

    // 重み付き復元抽出。重みは要素と同数で、有限・非負、非空なら総和が正かつ有限。
    // 累積和と二分探索を使う。期待時間O(N + k log N)、N == 1ならO(k)。出力以外の空間O(N)。
    template<RandomSequence R, std::ranges::input_range W>
        requires std::ranges::sized_range<W> &&
                 std::convertible_to<std::ranges::range_reference_t<W>, double>
    std::vector<std::ranges::range_value_t<R>> choices(R&& range, std::size_t k, W&& weights) {
        const auto n = std::ranges::size(range);
        assert(n == std::ranges::size(weights));
        assert(k == 0 || n > 0);
        std::vector<double> prefix;
        prefix.reserve(n);
        double total = 0;
        for (auto&& weight : weights) {
            const double value = static_cast<double>(weight);
            assert(std::isfinite(value) && value >= 0);
            total += value;
            prefix.push_back(total);
        }
        assert(n == 0 || (std::isfinite(total) && total > 0));
        std::vector<std::ranges::range_value_t<R>> result;
        result.reserve(k);
        for (std::size_t i = 0; i < k; ++i) {
            const auto index = std::upper_bound(prefix.begin(), prefix.end(), uniform(0, total)) - prefix.begin();
            result.push_back(at(range, static_cast<std::uint64_t>(index)));
        }
        return result;
    }

    // 異なる位置からk個をランダムな順序で抽出する。0 <= k <= N。
    // 疎なFisher–Yates法で期待時間・追加空間O(k)。入力の走査・変更は行わない。
    template<RandomSequence R>
    std::vector<std::ranges::range_value_t<R>> sample(R&& range, std::size_t k) {
        const auto n = std::ranges::size(range);
        assert(k <= n);
        std::vector<std::ranges::range_value_t<R>> result;
        result.reserve(k);
        if (k == 0) return result;
        for (auto index : sample_indices(k, static_cast<std::uint64_t>(n) - 1))
            result.push_back(at(range, index));
        return result;
    }

    // 入力を破壊的にシャッフルする。期待時間O(N)、追加空間O(1)。空範囲も可。
    template<std::ranges::random_access_range R>
        requires std::ranges::sized_range<R> && std::permutable<std::ranges::iterator_t<R>>
    void shuffle(R&& range) { std::ranges::shuffle(range, engine_); }

    // シャッフル済みのvectorを返す。期待時間・出力空間O(N)。入力は変更しない。
    template<RandomSequence R>
        requires std::permutable<typename std::vector<std::ranges::range_value_t<R>>::iterator>
    std::vector<std::ranges::range_value_t<R>> shuffled(R&& range) {
        std::vector<std::ranges::range_value_t<R>> result;
        result.reserve(std::ranges::size(range));
        for (auto&& value : range) result.push_back(value);
        shuffle(result);
        return result;
    }

    // [0, n)の一様な順列。n >= 0。期待時間・出力空間O(n)。
    std::vector<int> permutation(int n) {
        assert(n >= 0);
        std::vector<int> result(n);
        std::iota(result.begin(), result.end(), 0);
        shuffle(result);
        return result;
    }

    // 閉区間[a, b]の一様整数をn個返す。期待時間・出力空間O(n)。
    template<RandomInteger A, RandomInteger B>
        requires (std::is_signed_v<A> == std::is_signed_v<B>)
    std::vector<std::common_type_t<A, B>> randint_vec(std::size_t n, A a, B b) {
        assert(static_cast<__int128>(a) <= static_cast<__int128>(b));
        std::vector<std::common_type_t<A, B>> result;
        result.reserve(n);
        for (std::size_t i = 0; i < n; ++i) result.push_back(randint(a, b));
        return result;
    }

    // 閉区間[a, b]から相異なるn整数をランダムな順序で返す。a <= b、n <= b-a+1。
    // 64bit整数の全域も扱える。期待時間・追加空間O(n)。
    template<RandomInteger A, RandomInteger B>
        requires (std::is_signed_v<A> == std::is_signed_v<B>)
    std::vector<std::common_type_t<A, B>> distinct(std::size_t n, A a, B b) {
        using T = std::common_type_t<A, B>;
        const __int128 first = a, last = b;
        assert(first <= last);
        assert(static_cast<unsigned __int128>(n) <= static_cast<unsigned __int128>(last - first) + 1);
        std::vector<T> result;
        result.reserve(n);
        const auto maximum = static_cast<std::uint64_t>(last - first);
        for (auto index : sample_indices(n, maximum))
            result.push_back(static_cast<T>(first + static_cast<__int128>(index)));
        return result;
    }

    // alphabetの位置を一様に選び、長さnの文字列を返す。n > 0なら非空が必要。
    // 重複文字も別の候補と数える。期待時間・出力空間O(n)。
    std::string randstr(std::size_t n, std::string_view alphabet = "abcdefghijklmnopqrstuvwxyz") {
        assert(n == 0 || !alphabet.empty());
        std::string result(n, '\0');
        for (char& value : result) value = choice(alphabet);
        return result;
    }

    // a <= l < r <= bとなる整数の組を一様に返す。a < b。期待時間・空間O(1)。
    template<RandomInteger A, RandomInteger B>
        requires (std::is_signed_v<A> == std::is_signed_v<B>)
    std::pair<std::common_type_t<A, B>, std::common_type_t<A, B>> randpair(A a, B b) {
        assert(static_cast<__int128>(a) < static_cast<__int128>(b));
        auto values = distinct(2, a, b);
        if (values[0] > values[1]) std::swap(values[0], values[1]);
        return {values[0], values[1]};
    }

    // 和がnのk個の正整数からなる順序付き組を一様に返す。n >= 0、1 <= k <= n。
    // (n, k) == (0, 0)は空列を返す。期待時間O(k log k)、追加空間O(k)。
    std::vector<std::int64_t> composition(std::int64_t n, std::size_t k) {
        assert(n >= 0);
        if (k == 0) {
            assert(n == 0);
            return {};
        }
        assert(k <= static_cast<std::uint64_t>(n));
        if (k == 1) return {n};
        auto cuts = distinct(k - 1, std::int64_t{1}, n - 1);
        std::sort(cuts.begin(), cuts.end());
        std::vector<std::int64_t> result;
        result.reserve(k);
        std::int64_t previous = 0;
        for (auto cut : cuts) {
            result.push_back(cut - previous);
            previous = cut;
        }
        result.push_back(n - previous);
        return result;
    }

    // [0, n)を頂点とするラベル付き木の辺集合を一様に生成する。n >= 0。
    // Prüfer列を線形時間で復号する。期待時間・追加空間O(n)。n <= 1なら空。
    // 各辺はu < vに正規化する。辺の列の順序は一様とは限らない。
    std::vector<std::pair<int, int>> tree(int n) {
        assert(n >= 0);
        if (n <= 1) return {};
        const auto code = randint_vec(static_cast<std::size_t>(n - 2), 0, n - 1);
        std::vector<int> degree(n, 1);
        for (int v : code) ++degree[v];
        int pointer = 0;
        while (degree[pointer] != 1) ++pointer;
        int leaf = pointer;
        std::vector<std::pair<int, int>> result;
        result.reserve(n - 1);
        for (int v : code) {
            result.emplace_back(std::min(leaf, v), std::max(leaf, v));
            degree[leaf] = 0;
            if (--degree[v] == 1 && v < pointer) {
                leaf = v;
            } else {
                do { ++pointer; } while (degree[pointer] != 1);
                leaf = pointer;
            }
        }
        int other = 0;
        while (other == leaf || degree[other] != 1) ++other;
        result.emplace_back(std::min(leaf, other), std::max(leaf, other));
        return result;
    }

    // [0, n)を頂点とする単純グラフのm辺を生成する。n >= 0、mは最大辺数以下。
    // 既定は無向(u < v)、directed == trueなら有向。連結性は保証しない。
    // 各辺集合を一様に選び、辺もランダムな順序で返す。自己ループ・多重辺なし。
    // 期待時間は無向O(m log n)、有向O(m)。追加空間O(m)。m == 0なら空。
    std::vector<std::pair<int, int>> graph(int n, std::size_t m, bool directed = false) {
        assert(n >= 0);
        const auto vertices = static_cast<std::uint64_t>(n);
        const auto capacity = vertices * (vertices == 0 ? 0 : vertices - 1) / (directed ? 1 : 2);
        assert(m <= capacity);
        std::vector<std::pair<int, int>> result;
        result.reserve(m);
        if (m == 0) return result;
        for (auto index : sample_indices(m, capacity - 1)) {
            if (directed) {
                const int u = static_cast<int>(index / (vertices - 1));
                int v = static_cast<int>(index % (vertices - 1));
                if (v >= u) ++v;
                result.emplace_back(u, v);
            } else {
                // 大きい端点vごとにv本を割り当て、三角数で区間を特定する。
                std::uint64_t low = 1, high = vertices;
                while (high - low > 1) {
                    const auto mid = low + (high - low) / 2;
                    if (mid * (mid - 1) / 2 <= index) low = mid;
                    else high = mid;
                }
                result.emplace_back(static_cast<int>(index - low * (low - 1) / 2), static_cast<int>(low));
            }
        }
        return result;
    }

private:
    Engine engine_;

    template<RandomSequence R>
    static decltype(auto) at(R&& range, std::uint64_t index) {
        return std::ranges::begin(range)[static_cast<std::ranges::range_difference_t<R>>(index)];
    }

    // 仮想配列[0, maximum]の末尾を取り除く。変更した位置だけを保存する。
    // maximum == UINT64_MAXも有効。呼出元でk <= maximum+1を確認する。
    std::vector<std::uint64_t> sample_indices(std::size_t k, std::uint64_t maximum) {
        std::vector<std::uint64_t> result;
        result.reserve(k);
        std::unordered_map<std::uint64_t, std::uint64_t> moved;
        moved.reserve(k);
        const auto value_at = [&](std::uint64_t index) {
            const auto it = moved.find(index);
            return it == moved.end() ? index : it->second;
        };
        for (std::size_t i = 0; i < k; ++i) {
            const auto last = maximum - static_cast<std::uint64_t>(i);
            const auto index = randint(std::uint64_t{0}, last);
            result.push_back(value_at(index));
            if (index != last) moved[index] = value_at(last);
            moved.erase(last);
        }
        return result;
    }
};

}  // namespace sora
