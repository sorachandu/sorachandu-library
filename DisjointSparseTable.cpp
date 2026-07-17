#include<bits/stdc++.h>
using namespace std;


/*//--------------------------------------------------------
↓↓↓↓↓↓ここからライブラリ本体↓↓↓↓↓↓
*///--------------------------------------------------------
template <typename M>
concept Monoid = requires {
    // 1. 台集合の型 M::T が存在すること
    typename M::T;
} && requires (typename M::T a, typename M::T b) {
    // 2. 二項演算 M::op(a, b) が存在し、戻り値型が M::T に変換可能であること
    { M::op(a, b) } -> std::convertible_to<typename M::T>;
    // 3. 単位元 M::e() が存在し、戻り値型が M::T に変換可能であること
    { M::e() } -> std::convertible_to<typename M::T>;
};

/*//--------------------------------------------------------
Disjoint Sparse Table (DST)
モノイドの区間積クエリを、前処理O(Nlog(N))、クエリあたりO(1)で処理
refer:
https://info.atcoder.jp/entry/algorithm_lectures/disjoint_sparse_table
https://noshi91.hatenablog.com/entry/2023/04/07/165310
*///--------------------------------------------------------
template <Monoid M>
class DisjointSparseTable{
    using Type=typename M::T;
    vector<vector<Type>> t;

    int size() const { return ssize(t[0])-2; }

public:
    DisjointSparseTable(const vector<Type> &v) : t(){
        const int n=ssize(v)+2;
        const int h=32-countl_zero((uint)n-1);
        t.assign(h,vector<Type>(n,M::e()));
        for(int k=1;k<h;k++){
            auto &s=t[k];
            const int w=1<<k;
            for(int i=w;i<n;i+=w*2){
                for(int j=i-1;j>i-w;j--)
                    s[j-1]=M::op(v[j-1],s[j]);
                const int m=min(i+w-1,n-1);
                for(int j=i;j<m;j++)
                    s[j+1]=M::op(s[j],v[j-1]);
            }
        }
    }

    Type get(int p) const {
        assert(0<=p and p<size());
        return prod(p,p+1);
    }

    Type prod(int l,int r) const {
        assert(0<=l and l<=r and r<=size());
        r++;
        const auto &s=t[31-countl_zero((uint)l^r)];
        return M::op(s[l],s[r]);
    }
};
/*//--------------------------------------------------------
↑↑↑↑↑↑ライブラリ本体終了↑↑↑↑↑↑
*///--------------------------------------------------------

/*//--------------------------------------------------------
使用例 https://judge.yosupo.jp/submission/386054
*///--------------------------------------------------------
struct IntMin{
    using T=int;
    static T op(T a,T b){ return min(a,b); }      
    static T e(){ return 1<<30; }
};

int main(){
    cin.tie(nullptr)->ios::sync_with_stdio(false);
    int N,Q;
    cin>>N>>Q;
    vector<int> A(N);
    for(auto &e:A) cin>>e;
    DisjointSparseTable<IntMin> DST(A);
    while(Q--){
        int l,r;
        cin>>l>>r;
        cout<<DST.prod(l,r)<<"\n";
    }
}