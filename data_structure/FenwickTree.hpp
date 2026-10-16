#pragma once

#include <vector>
#include <cassert>

namespace sora{

template<typename T>
class FenwickTree{
private:
    int n_;
    std::vector<T> data_;

public:
    // O(N)
    explicit FenwickTree(int n) : n_(n), data_(n+1,T{}) {}

    // a[p] += x : O(logN)
    void add(int p,T x){
        assert(0<=p and p<n_);
        for(++p; p<=n_; p+=p & -p){
            data_[p]+=x;
        }
    }

    // [0,r)のsum : O(logN)
    T sum(int r) const {
        assert(0<=r and r<=n_);
        T res{};
        for(; r>0; r-=r & -r){
            res+=data_[r];
        }
        return res;
    }

    // [l,r)のsum : O(logN)
    T sum(int l,int r) const {
        assert(0<=l and l<=r and r<=n_);
        return sum(r)-sum(l);
    }
};

} // namespace sora