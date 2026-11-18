#include "../../string/RollingHashMonoid.hpp"
#include "../../data_structure/SegmentTree.hpp"

#include <cassert>
#include <limits>
#include <random>
#include <string>
#include <string_view>
#include <vector>

using M = sora::RollingHashMonoid;
static_assert(sora::Monoid<M>);
static_assert(M::e().hash == 0 && M::e().power == 1 && M::e().len == 0);

M::T reference(std::string_view s) {
    const auto base = M::from_char(0).power;
    M::T result;
    for (unsigned char c : s) {
        result.hash = (static_cast<__uint128_t>(result.hash) * base + c + 1) % M::mod;
        result.power = static_cast<__uint128_t>(result.power) * base % M::mod;
        ++result.len;
    }
    return result;
}

M::T fold(std::string_view s) {
    M::T result = M::e();
    for (unsigned char c : s) result = M::op(result, M::from_char(c));
    return result;
}

int main() {
    assert(M::T{} == M::e());
    assert(M::op(M::e(), M::e()) == M::e());
    const auto base = M::from_char(0).power;
    assert(257 <= base && base <= M::mod - 2);
    std::string bytes;
    for (int c = 0; c < 256; ++c) {
        const auto x = M::from_char(static_cast<unsigned char>(c));
        assert(x.hash == static_cast<M::u64>(c + 1));
        assert(x.power == base && x.len == 1);
        assert(M::op(M::e(), x) == x && M::op(x, M::e()) == x);
        bytes.push_back(static_cast<char>(c));
    }
    assert(fold(bytes) == reference(bytes));
    assert(fold("") == reference(""));

    std::mt19937_64 rng(192817);
    for (int iteration = 0; iteration < 5000; ++iteration) {
        std::string s(rng() % 100, '\0');
        for (char& c : s) c = static_cast<char>(rng() % 256);
        const std::string_view view(s);
        const auto i = rng() % (s.size() + 1);
        const auto j = i + rng() % (s.size() - i + 1);
        const auto a = fold(view.substr(0, i));
        const auto b = fold(view.substr(i, j - i));
        const auto c = fold(view.substr(j));
        assert(M::op(M::op(a, b), c) == M::op(a, M::op(b, c)));
        assert(M::op(M::op(a, b), c) == reference(s));
    }

    // 文字列を確保せずに長い連結の剰余演算も検証する。
    auto x = fold(bytes);
    while (x.len <= std::numeric_limits<std::size_t>::max() / 2) {
        const M::T expected{
            static_cast<M::u64>((static_cast<__uint128_t>(x.hash) * x.power + x.hash) % M::mod),
            static_cast<M::u64>(static_cast<__uint128_t>(x.power) * x.power % M::mod),
            x.len * 2};
        x = M::op(x, x);
        assert(x == expected);
    }

    for (int n : {0, 1, 2, 3, 17, 64, 129}) {
        std::string s(n, 'a');
        std::vector<M::T> values(n, M::from_char('a'));
        sora::SegmentTree<M> seg(values);
        sora::SegmentTree<M> other(n);
        assert(other.all_prod() == M::e());
        for (int iteration = 0; iteration < 1000; ++iteration) {
            if (n > 0) {
                const int p = static_cast<int>(rng() % n);
                s[p] = static_cast<char>(rng() % 256);
                seg.set(p, M::from_char(s[p]));
                other.set(p, M::from_char(s[p]));
                assert(seg.get(p) == reference(std::string_view(s).substr(p, 1)));
            }
            const int l = static_cast<int>(rng() % (n + 1));
            const int r = l + static_cast<int>(rng() % (n - l + 1));
            assert(seg.prod(l, r) == reference(std::string_view(s).substr(l, r - l)));
            assert(seg.all_prod() == reference(s));
        }
        for (int p = 0; p < n; ++p) other.set(p, seg.get(p));
        assert(other.all_prod() == seg.all_prod());
    }
}
