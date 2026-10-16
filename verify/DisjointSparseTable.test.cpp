#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"

#include <algorithm>
#include <iostream>
#include <vector>

#include "../data_structure/DisjointSparseTable.hpp"

struct IntMin{
    using T=int;
    static T op(T a,T b){ return std::min(a,b); }      
    static T e(){ return 1<<30; }
};

int main(){
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);
    int N,Q;
    std::cin>>N>>Q;
    std::vector<int> A(N);
    for(auto &e:A) std::cin>>e;
    sora::DisjointSparseTable<IntMin> DST(A);
    while(Q--){
        int l,r;
        std::cin>>l>>r;
        std::cout<<DST.prod(l,r)<<"\n";
    }
}
