#define PROBLEM "https://judge.yosupo.jp/problem/range_kth_smallest"

#include <iostream>
#include <vector>

#include "../data_structure/WaveletMatrix.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, q;
    std::cin >> n >> q;
    std::vector<int> a(n);
    for (int& x : a) std::cin >> x;
    const sora::WaveletMatrix<int> wm(a);
    while (q--) {
        int l, r, k;
        std::cin >> l >> r >> k;
        std::cout << wm.kth_smallest(l, r, k) << '\n';
    }
}
