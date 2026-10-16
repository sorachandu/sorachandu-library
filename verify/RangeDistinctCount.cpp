// verification-helper: IGNORE
#define PROBLEM "https://atcoder.jp/contests/abc174/tasks/abc174_f"

#include <iostream>
#include <vector>

#include "../data_structure/RangeDistinctCount.hpp"

int main(){
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);
    int N,Q;
    std::cin>>N>>Q;
    std::vector<int> C(N);
    for(int i=0;i<N;i++) std::cin>>C[i],C[i]--;
    sora::RangeDistinctCount dc(C);
    while(Q--){
        int l,r;
        std::cin>>l>>r;
        l--;
        std::cout<<dc.query(l,r,0,N)<<"\n";
    }
}
