#define PROBLEM "https://judge.yosupo.jp/problem/zalgorithm"

#include <iostream>
#include <string>
#include <vector>

#include "../string/RollingHashMonoid.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string s;
    std::cin >> s;
    using M = sora::RollingHashMonoid;
    const int n = static_cast<int>(s.size());
    std::vector<M::T> prefix(n + 1, M::e());
    for (int i = 0; i < n; ++i)
        prefix[i + 1] = M::op(prefix[i], M::from_char(s[i]));
    // prefix[i]に先頭len文字を連結した値とprefix[i+len]を比較する。
    // 二分探索で各接尾辞のLCPを求める。時間O(N log N)、空間O(N)。
    for (int i = 0; i < n; ++i) {
        int low = 0, high = n - i + 1;
        while (high - low > 1) {
            const int mid = (low + high) / 2;
            if (M::op(prefix[i], prefix[mid]) == prefix[i + mid]) low = mid;
            else high = mid;
        }
        std::cout << low << (i + 1 == n ? '\n' : ' ');
    }
}
