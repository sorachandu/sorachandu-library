#pragma once

#include <type_traits>

#include <atcoder/modint>

#include "Combination.hpp"

namespace sora {
namespace detail {

// ACLのstatic_modintまたはdynamic_modintだけを受け入れる型判定。
template <typename T>
struct IsAtCoderModInt : std::false_type {};

// ACLの任意の素数modに対応する静的modintの判定。
template <int Mod, std::enable_if_t<(1 <= Mod)>* Ptr>
struct IsAtCoderModInt<atcoder::static_modint<Mod, Ptr>> : std::true_type {};

// ACLの動的modintの判定。
template <int Id>
struct IsAtCoderModInt<atcoder::dynamic_modint<Id>> : std::true_type {};

} // namespace detail

// ACL向けの別名。計算はCombinationと共有し、構築O(N)、各クエリO(1)、空間O(N)。
// modは素数、0 <= N < mod。動的modintは構築後にmodを変更しないこと。
template <typename ModInt>
    requires detail::IsAtCoderModInt<ModInt>::value
using AtCoderCombination = Combination<ModInt>;

} // namespace sora
