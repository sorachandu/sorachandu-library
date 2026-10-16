#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <numeric>
#include <random>
#include <set>
#include <string>
#include <vector>

#include "../data_structure/Compression.hpp"
#include "../data_structure/Doubling.hpp"
#include "../data_structure/PotentialUF.hpp"
#include "../graph/LowLink.hpp"
#include "../graph/LowestCommonAncestor.hpp"
#include "../math/Eratosthenes.hpp"
#include "../math/Matrix.hpp"
#include "../math/Pow.hpp"
#include "../string/RollingHash.hpp"
#include "../string/RunLengthEncoding.hpp"
#include "../string/Trie.hpp"
#include "../utility/RangeRandom.hpp"
#include "../utility/Timer.hpp"

// ヘッダ移行に伴う境界ケースと、単純解との比較を行うローカル検証。
int main() {
    std::mt19937 rng(12345);
    {
        sora::Compression<int> empty(std::vector<int>{});
        assert(empty.size() == 0 && empty.get_from_val(1) == 0);
        for (int n = 1; n < 50; ++n) {
            std::vector<int> a(n);
            for (auto& x : a) x = static_cast<int>(rng() % 11) - 5;
            sora::Compression<int> cmp(a);
            std::vector<int> values(a);
            std::sort(values.begin(), values.end());
            values.erase(std::unique(values.begin(), values.end()), values.end());
            assert(cmp.size() == static_cast<int>(values.size()));
            for (int i = 0; i < n; ++i) assert(values[cmp.get(i)] == a[i] && cmp[i] == cmp.get(i));
            for (int x = -7; x <= 7; ++x)
                assert(cmp.get_from_val(x) == std::lower_bound(values.begin(), values.end(), x) - values.begin());
        }
    }
    {
        sora::Doubling<int> empty(std::vector<int>{}, 0);
        sora::Doubling<int> zero(std::vector<int>{0}, 0);
        assert(zero.query(0, 0) == 0);
        for (int n = 1; n < 30; ++n) {
            std::vector<int> next(n);
            for (int& x : next) x = rng() % n;
            sora::Doubling<int> db(next, 200);
            for (int v = 0; v < n; ++v) {
                int expected = v;
                for (int k = 0; k <= 200; ++k) {
                    assert(db.query(v, k) == expected);
                    expected = next[expected];
                }
            }
        }
        sora::Doubling<unsigned int> cycle(std::vector<unsigned int>{1, 0}, ~0ULL);
        assert(cycle.query(0, ~0ULL) == 1);
    }
    {
        for (int n = 0; n <= 200; ++n) {
            auto sieve = sora::eratosthenes(n);
            std::vector<long long> expected;
            for (int x = 0; x <= n; ++x) {
                bool prime = x >= 2;
                for (int d = 2; d * d <= x; ++d) if (x % d == 0) prime = false;
                assert(sieve[x] == prime);
                if (prime) expected.push_back(x);
            }
            assert(sora::get_primes(n) == expected);
        }
        assert(sora::pow(0, 0) == 1);
        assert(sora::pow(-1, 1000000000000LL) == 1);
        assert(sora::pow(-2, 5) == -32);
        assert(sora::pow(2, 62) == (1LL << 62));
    }
    {
        sora::Matrix<long long> empty;
        assert(empty.width() == 0 && empty.height() == 0 && empty.determinant() == 1);
        const std::vector<std::vector<long long>> values{{1, 1}, {1, 0}};
        const sora::Matrix<long long> fib(values);
        auto power = fib ^ 10;
        assert(power[0][0] == 89 && power[0][1] == 55 && power[1][1] == 34);
        auto self = fib;
        self *= self;
        assert(self.a == (fib * fib).a);
        assert(((fib + fib) - fib).a == fib.a);
        assert((fib ^ 0).a == sora::Matrix<long long>::identity(2).a);
        sora::Matrix<double> mat(std::vector<std::vector<double>>{{0, 2}, {3, 4}});
        assert(std::abs(mat.determinant() + 6) < 1e-9);
        assert(mat[0][0] == 0 && mat[0][1] == 2);
        sora::Matrix<double> singular(std::vector<std::vector<double>>{{1, 2}, {2, 4}});
        assert(singular.determinant() == 0);
        sora::Matrix<int> a(2, 3), b(3, 4);
        assert((a * b).height() == 2 && (a * b).width() == 4);
    }
    {
        assert(sora::run_length_encoding(std::string{}).empty());
        assert(sora::run_length_encoding(std::vector<int>{}).empty());
        for (int n = 1; n < 100; ++n) {
            std::string str;
            std::vector<int> values;
            for (int i = 0; i < n; ++i) { int x = rng() % 4; values.push_back(x); str += char('a' + x); }
            std::string restored;
            for (auto [c, count] : sora::run_length_encoding(str)) restored.append(count, c);
            assert(str == restored);
            std::vector<int> decoded;
            for (auto [x, count] : sora::run_length_encoding(values)) decoded.insert(decoded.end(), count, x);
            assert(decoded == values);
        }
    }
    {
        sora::Trie trie;
        std::multiset<std::string> model;
        for (int t = 0; t < 300; ++t) {
            std::string str;
            for (unsigned int i = 0, len = rng() % 5; i < len; ++i) str += char('a' + rng() % 3);
            if (t % 3 == 0) { trie.insert(str); model.insert(str); }
            else if (t % 3 == 1) {
                auto it = model.find(str);
                assert(trie.erase(str) == (it != model.end()));
                if (it != model.end()) model.erase(it);
            } else assert(trie.erase_all(str) == static_cast<int>(model.erase(str)));
            assert(trie.size() == static_cast<int>(model.size()) && trie.empty() == model.empty());
            assert(trie.count_word(str) == static_cast<int>(model.count(str)));
            int prefixes = 0;
            for (const auto& word : model) if (word.starts_with(str)) ++prefixes;
            assert(trie.count_prefix(str) == prefixes);
            assert(trie.count_less(str) == std::distance(model.begin(), model.lower_bound(str)));
            assert(trie.count_less_equal(str) == std::distance(model.begin(), model.upper_bound(str)));
            const auto lower = model.lower_bound(str), upper = model.upper_bound(str);
            assert(trie.lower_bound(str) == (lower == model.end() ? std::nullopt : std::optional<std::string>(*lower)));
            assert(trie.upper_bound(str) == (upper == model.end() ? std::nullopt : std::optional<std::string>(*upper)));
            int k = 0;
            for (const auto& word : model) assert(trie.kth(k++) == word);
            assert(!trie.kth(-1) && !trie.kth(k));
        }
    }
    {
        sora::RollingHash empty;
        assert(empty.hash() == 0 && empty.hash(0, 0) == 0 && empty.size() == 0);
        std::string str;
        for (int i = 0; i < 100; ++i) str += static_cast<char>(rng() % 256);
        sora::RollingHash hash(str);
        for (int l = 0; l <= 100; ++l) for (int r = l; r <= 100; ++r) {
            sora::RollingHash part(str.substr(l, r - l));
            assert(hash.hash(l, r) == part.hash());
            assert(hash.concat(hash.hash(0, l), hash.hash(l, r), r - l) == hash.hash(0, r));
        }
    }
    {
        sora::PotentialUF<long long> uf(30);
        std::vector<int> label(30);
        std::iota(label.begin(), label.end(), 0);
        for (int t = 0; t < 100; ++t) {
            int x = rng() % 30, y = rng() % 30;
            int lx = label[x], ly = label[y];
            assert(uf.merge(x, y, 3LL * (y - x)) == (lx != ly));
            for (int& id : label) if (id == ly) id = lx;
            for (int a = 0; a < 30; ++a) for (int b = 0; b < 30; ++b) {
                assert(uf.same(a, b) == (label[a] == label[b]));
                if (uf.same(a, b)) assert(uf.diff(a, b) == 3LL * (b - a));
            }
        }
        auto groups = uf.groups();
        int total = 0;
        for (const auto& group : groups) { total += group.size(); for (int v : group) assert(uf.same(v, group[0])); }
        assert(total == 30);
        assert(sora::PotentialUF<int>(0).groups().empty());
    }
    {
        for (int n = 1; n <= 30; ++n) {
            std::vector<std::vector<int>> g(n);
            for (int v = 1; v < n; ++v) { int p = rng() % v; g[p].push_back(v); g[v].push_back(p); }
            sora::LowestCommonAncestor tree(g);
            for (int root : {0, n - 1}) {
                tree.build(root);
                std::vector<int> parent(n, -1), depth(n);
                auto dfs = [&](auto&& self, int v, int p) -> void {
                    parent[v] = p;
                    for (int w : g[v]) if (w != p) { depth[w] = depth[v] + 1; self(self, w, v); }
                };
                dfs(dfs, root, -1);
                for (int v = 0; v < n; ++v) {
                    assert(tree.get_parent(v) == parent[v] && tree.get_depth(v) == depth[v]);
                    for (int w = 0; w < n; ++w) {
                        int a = v, b = w;
                        while (depth[a] > depth[b]) a = parent[a];
                        while (depth[b] > depth[a]) b = parent[b];
                        while (a != b) { a = parent[a]; b = parent[b]; }
                        assert(tree.lca(v, w) == a);
                        assert(tree.get_distance(v, w) == depth[v] + depth[w] - 2 * depth[a]);
                    }
                }
            }
        }
        sora::LowestCommonAncestor tree(2);
        tree.add_edge(0, 1, 0); tree.build();
        assert(tree.get_parent_edge_id(1) == 0 && tree.is_on_path(0, 1, 1));
        assert(tree.closest_on_path(0, 0, 1) == 0);
    }
    {
        for (int t = 0; t < 100; ++t) {
            int n = rng() % 10;
            std::vector<std::vector<int>> g(n);
            std::vector<std::pair<int, int>> edges;
            for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v) if (rng() % 3 == 0) {
                g[u].push_back(v); g[v].push_back(u); edges.emplace_back(u, v);
            }
            auto components = [&](int removed, std::pair<int, int> edge) {
                std::vector<bool> used(n);
                int count = 0;
                auto dfs = [&](auto&& self, int v) -> void {
                    used[v] = true;
                    for (int w : g[v]) if (w != removed && !used[w] && std::minmax(v, w) != std::minmax(edge.first, edge.second)) self(self, w);
                };
                for (int v = 0; v < n; ++v) if (v != removed && !used[v]) { ++count; dfs(dfs, v); }
                return count;
            };
            const int base = components(-1, {-1, -1});
            std::vector<int> aps;
            std::vector<std::pair<int, int>> bridges;
            for (int v = 0; v < n; ++v) if (components(v, {-1, -1}) > base) aps.push_back(v);
            for (auto edge : edges) if (components(-1, edge) > base) bridges.push_back(edge);
            sora::LowLink low(g);
            std::sort(low.aps.begin(), low.aps.end());
            std::sort(low.bridges.begin(), low.bridges.end());
            assert(low.aps == aps && low.bridges == bridges);
        }
    }
    {
        sora::RangeRandom random(-3, 7);
        for (int i = 0; i < 1000; ++i) { auto x = random(); assert(-3 <= x && x <= 7); }
        random.set_range(5, 5);
        assert(random.gen() == 5 && random.get_range() == std::make_pair(std::int64_t{5}, std::int64_t{5}));
        sora::Timer timer;
        auto start = timer.count();
        assert(start >= 0 && timer.count() >= start && timer.is_under(3600));
    }
}
