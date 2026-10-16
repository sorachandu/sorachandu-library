#include <cassert>
#include <type_traits>

#include "../math/AtCoderCombination.hpp"

// 外部ACLのincludeパスを指定して実行するローカル検証。
int main() {
    using Mint = atcoder::modint998244353;
    static_assert(std::is_same_v<sora::AtCoderCombination<Mint>, sora::Combination<Mint>>);
    static_assert(!sora::detail::IsAtCoderModInt<int>::value);
    const sora::AtCoderCombination<Mint> empty(0);
    assert(empty.comb(0, 0).val() == 1 && empty.fact(0).val() == 1);
    const sora::AtCoderCombination<Mint> comb(10);
    assert(comb.comb(10, 3).val() == 120 && comb.perm(10, 3).val() == 720);
    assert(comb.fact(10).val() == 3628800);
    using Dynamic = atcoder::dynamic_modint<123>;
    Dynamic::set_mod(101);
    const sora::AtCoderCombination<Dynamic> dynamic(100);
    assert(dynamic.comb(100, 50).val() == 1);
    assert(dynamic.fact(100).val() == 100);
    const sora::Combination<atcoder::static_modint<7>> small(6);
    assert(small.comb(6, 3).val() == 6);
}
