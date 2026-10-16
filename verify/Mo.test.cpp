// verification-helper: IGNORE
#define PROBLEM "https://atcoder.jp/contests/abc242/tasks/abc242_g"

#include <algorithm>
#include <iostream>
#include <vector>

#include "../data_structure/Mo.hpp"

int main(){
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);
    int N;
    std::cin>>N;
    std::vector<int> A(N);
    for(auto &e:A) std::cin>>e;
    int Q;
    std::cin>>Q;
    sora::Mo mo(N);
    for(int i=0;i<Q;i++){
        int l,r;
        std::cin>>l>>r;
        mo.add(l-1,r);
    }
    int res{};
    std::vector<int> ans(Q);
    std::vector<int> cnt(N + 1);
    auto Add=[&](int k){
        if((cnt[A[k]]++)&1) res++;
    };
    auto Del=[&](int k){
        if((--cnt[A[k]])&1) res--;
    };
    auto Rem=[&](int q){
        ans[q]=res;
    };
    mo.build(Add,Del,Rem);
    for(auto e:ans) std::cout<<e<<"\n";
}
