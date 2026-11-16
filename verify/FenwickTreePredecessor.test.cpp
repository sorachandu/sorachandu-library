#define PROBLEM "https://judge.yosupo.jp/problem/predecessor_problem"

#include <iostream>
#include <string>

#include "../data_structure/FenwickTree.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, q;
    std::string initial;
    std::cin >> n >> q >> initial;
    sora::FenwickTree<int> tree(n);
    for (int i = 0; i < n; ++i)
        if (initial[i] == '1') tree.add(i, 1);
    while (q--) {
        int type, p;
        std::cin >> type >> p;
        if (type == 0) tree.set(p, 1);
        else if (type == 1) tree.set(p, 0);
        else if (type == 2) std::cout << tree.get(p) << '\n';
        else if (type == 3) {
            const int next = tree.find_last(p, [](int count) { return count == 0; });
            std::cout << (next == n ? -1 : next) << '\n';
        } else {
            const int prev = tree.find_first(p + 1, [](int count) { return count == 0; }) - 1;
            std::cout << prev << '\n';
        }
    }
}
