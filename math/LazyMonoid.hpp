#pragma once

#include <concepts>

#include "Monoid.hpp"

namespace sora {

// モノイドに、値への作用mapping、作用の合成composition、恒等作用idを加えた構文を検査する。
// composition(f, g)はgの後にfを適用する。数学的な法則は利用者が保証する。
template <typename M>
concept LazyMonoid = Monoid<M> && requires(typename M::F f, typename M::F g, typename M::T x) {
    { M::mapping(f, x) } -> std::convertible_to<typename M::T>;
    { M::composition(f, g) } -> std::convertible_to<typename M::F>;
    { M::id() } -> std::convertible_to<typename M::F>;
};

} // namespace sora
