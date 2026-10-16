#pragma once

#include <algorithm>
#include <iterator>
#include <vector>

namespace sora {

// 配列の座標圧縮。入力は変更しない。比較・コピーをO(1)として構築O(N log N)、空間O(N)。
template<typename T> struct Compression{
    int n;
    std::vector<T> cmp;
    std::vector<int> id;
    Compression(const std::vector<T> &a) : n(std::ssize(a)), cmp(a), id(n) {
        std::sort(cmp.begin(),cmp.end());
        cmp.erase(std::unique(cmp.begin(),cmp.end()),cmp.end());
        for(int i=0;i<n;i++) id[i]=std::lower_bound(cmp.begin(),cmp.end(),a[i])-cmp.begin();
    }

    // 圧縮後の添字を取得。時間 O(1)。
    const int &operator[](int k) const {
        return id[k];
    }

    // 圧縮後の添字を取得。時間 O(1)。
    int get(int k) const {
        return id[k];
    }

    // 異なる値の個数。時間 O(1)。
    int size() const {
        return std::ssize(cmp);
    }

    // val 以上の最初の値の順位（未登録値も可）。時間 O(log N)。
    int get_from_val(const T& val) const {
        return std::lower_bound(cmp.begin(),cmp.end(),val)-cmp.begin();
    }
};

} // namespace sora
