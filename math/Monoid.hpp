#pragma once

#include <concepts>

namespace sora {

// M::T を台集合、M::op を結合的な演算、M::e() を単位元とする。
// 結合則と単位元の法則は利用者が保証する（Conceptは構文のみを検査する）。
template <typename M>
concept Monoid = requires(typename M::T a, typename M::T b) {
    { M::op(a, b) } -> std::convertible_to<typename M::T>;
    { M::e() } -> std::convertible_to<typename M::T>;
};

} // namespace sora
