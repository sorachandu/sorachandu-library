#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <iterator>
#include <vector>

#include "../math/Monoid.hpp"

namespace sora {

// モノイドの静的区間積。演算・コピーをO(1)として構築時間・空間O(N log N)、取得O(1)。
// 参考: https://info.atcoder.jp/entry/algorithm_lectures/disjoint_sparse_table
// https://noshi91.hatenablog.com/entry/2023/04/07/165310
template <Monoid M>
class DisjointSparseTable{
    using Type=typename M::T;
    std::vector<std::vector<Type>> t;

    int size() const { return std::ssize(t[0])-2; }

public:
    DisjointSparseTable(const std::vector<Type> &v) : t(){
        const int n=std::ssize(v)+2;
        const int h=32-std::countl_zero((unsigned int)n-1);
        t.assign(h,std::vector<Type>(n,M::e()));
        for(int k=1;k<h;k++){
            auto &s=t[k];
            const int w=1<<k;
            for(int i=w;i<n;i+=w*2){
                for(int j=i-1;j>i-w;j--)
                    s[j-1]=M::op(v[j-1],s[j]);
                const int m=std::min(i+w-1,n-1);
                for(int j=i;j<m;j++)
                    s[j+1]=M::op(s[j],v[j-1]);
            }
        }
    }

    // 要素を取得する。時間 O(1)。
    Type get(int p) const {
        assert(0<=p and p<size());
        return prod(p,p+1);
    }

    // 半開区間 [l, r) の積。時間 O(1)、空区間では単位元を返す。
    Type prod(int l,int r) const {
        assert(0<=l and l<=r and r<=size());
        r++;
        const auto &s=t[31-std::countl_zero((unsigned int)l^r)];
        return M::op(s[l],s[r]);
    }
};

} // namespace sora
