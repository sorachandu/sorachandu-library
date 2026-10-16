#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

#include<iostream>
#include"../data_structure/FenwickTree.hpp"

int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N,Q;
    std::cin>>N>>Q;
    sora::FenwickTree<long long> BIT(N);
    for(int i=0;i<N;i++){
        long long x;
        std::cin>>x;
        BIT.add(i,x);
    }
    while(Q--){
        int type;
        std::cin>>type;
        if(type==0){
            int p;
            long long x;
            std::cin>>p>>x;
            BIT.add(p,x);
        }else{
            int l,r;
            std::cin>>l>>r;
            std::cout<<BIT.sum(l,r)<<"\n";
        }
    }
}
