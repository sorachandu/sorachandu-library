#include<bits/stdc++.h>
using namespace std;

/*//--------------------------------------------------------
Mo's Algorithm
配列の要素が不変かつクエリが先読み可能かつ、
区間を左右に動かしたときの差分が高速に計算できるなら使えるかも
O(α N sqrt(Q)) ただし区間差分更新一回の計算量をO(α)とした場合
Refer: https://ei1333.hateblo.jp/entry/2017/09/11/211011
*///--------------------------------------------------------
class Mo{
    int N;
    std::vector<std::pair<int,int>> LR;

public:
    explicit Mo(int N): N(N) {}

    // Add query [l,r)
    void add(int l,int r){
        LR.emplace_back(l,r);
    }

    template<typename AL,typename AR,typename DL,typename DR,typename REM>
    void build(const AL &add_left, const AR &add_right, const DR &del_left, const DL &del_right,const REM &rem){
        int Q=std::ssize(LR);
        int bs=N/std::min<int>(N,sqrt(Q));
        std::vector<int> ord(Q);
        std::iota(ord.begin(),ord.end(),0);
        std::sort(ord.begin(),ord.end(),[&](int a,int b){
            int ablock=LR[a].first/bs, bblock=LR[b].first/bs;
            if(ablock!=bblock) return ablock<bblock;
            return (ablock&1)?LR[a].second>LR[b].second:LR[a].second<LR[b].second;
        });
        int L=0,R=0;
        for(auto idx:ord){
            while(L>LR[idx].first) add_left(--L);
            while(R<LR[idx].second) add_right(R++);
            while(L<LR[idx].first) del_left(L++);
            while(R>LR[idx].second) del_right(--R);
            rem(idx);
        }
    }

    template<typename A,typename D,typename R>
    void build(const A &add,const D &del,const R &rem){
        build(add,add,del,del,rem);
    }
};




/*//--------------------------------------------------------
Usage: ABC242-G https://atcoder.jp/contests/abc242/submissions/77097828
*///--------------------------------------------------------
int main(){
    cin.tie(nullptr)->ios::sync_with_stdio(false);
    int N;
    cin>>N;
    vector<int> A(N);
    for(auto &e:A) cin>>e;
    int Q;
    cin>>Q;
    Mo mo(N);
    for(int i=0;i<Q;i++){
        int l,r;
        cin>>l>>r;
        mo.add(l-1,r);
    }
    int res{};
    vector<int> ans(Q);
    vector<int> cnt(N);
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
    for(auto e:ans) cout<<e<<"\n";
}