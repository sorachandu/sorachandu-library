#pragma once

#include <bit>
#include <cassert>
#include <cstddef>
#include <vector>

namespace sora{

// 点加算・点代入・区間和・境界探索。T{}を零元とする加法可換群を使う。
// Tのコピー・加減算をO(1)として空間O(N)、更新・問い合わせO(log N)。
template<typename T>
class FenwickTree{
private:
    int n_;
    std::vector<T> data_;

public:
    // 長さnの零配列を構築する。n >= 0。時間・空間O(N)。
    explicit FenwickTree(int n) : n_(n) {
        assert(n>=0);
        data_.assign(static_cast<std::size_t>(n)+1,T{});
    }

    // a[p] += x : O(logN)
    void add(int p,T x){
        assert(0<=p and p<n_);
        for(unsigned int i=static_cast<unsigned int>(p)+1; i<=static_cast<unsigned int>(n_); i+=i & -i){
            data_[i]+=x;
        }
    }

    // a[p]を返す。sum(p,p+1)と等価。0 <= p < N。時間O(log N)。
    T get(int p) const {
        assert(0<=p and p<n_);
        T result=data_[p+1];
        const int left=(p+1) & p;
        for(int r=p; r>left; r-=r & -r) result=result-data_[r];
        return result;
    }

    // a[p]をxに置き換える。0 <= p < N。時間O(log N)。
    void set(int p,T x){
        assert(0<=p and p<n_);
        add(p,x-get(p));
    }

    // [0,r)の和を返す : O(logN)
    T sum(int r) const {
        assert(0<=r and r<=n_);
        T res{};
        for(; r>0; r-=r & -r){
            res+=data_[r];
        }
        return res;
    }

    // [l,r)の和を返す : O(logN)
    T sum(int l,int r) const {
        assert(0<=l and l<=r and r<=n_);
        return sum(r)-sum(l);
    }

    // f(sum(l,r))がtrueとなる最大のrを返す。0 <= l <= N。
    // f(T{}) == true、fは同じ引数に同じ値を返し、区間を右へ伸ばすとtrueからfalseへ単調に変わること。
    // fの評価をO(1)として時間O(log N)。l == NならNを返す。
    template<class F>
    int find_last(int l,F f) const {
        assert(0<=l and l<=n_);
        assert(f(T{}));
        if(l==n_) return n_;
        const T base=sum(l);
        T prefix{};
        int p=0;
        for(int step=static_cast<int>(std::bit_floor(static_cast<unsigned int>(n_))); step>0; step/=2){
            if(step>n_-p) continue;
            const int next=p+step;
            T candidate=prefix;
            candidate+=data_[next];
            // lより手前は無条件に進み、述語には[l, next)の和だけを渡す。
            if(next<l || f(candidate-base)){
                p=next;
                prefix=candidate;
            }
        }
        return p;
    }

    // 関数ポインタをテンプレート引数で指定するfind_last。条件・計算量は上と同じ。
    template<bool (*f)(T)>
    int find_last(int l) const { return find_last(l,f); }

    // f(sum(l,r))がtrueとなる最小のlを返す。0 <= r <= N。
    // f(T{}) == true、fは同じ引数に同じ値を返し、区間を左へ伸ばすとtrueからfalseへ単調に変わること。
    // fの評価をO(1)として時間O(log N)。r == 0なら0を返す。
    template<class F>
    int find_first(int r,F f) const {
        assert(0<=r and r<=n_);
        assert(f(T{}));
        if(r==0) return 0;
        const T total=sum(r);
        if(f(total)) return 0;
        T prefix{};
        int p=0;
        for(int step=static_cast<int>(std::bit_floor(static_cast<unsigned int>(r))); step>0; step/=2){
            if(step>r-p) continue;
            const int next=p+step;
            T candidate=prefix;
            candidate+=data_[next];
            // 述語がfalseになる最後の左端を探す。rでは零元になるので必ずtrue。
            if(!f(total-candidate)){
                p=next;
                prefix=candidate;
            }
        }
        return p+1;
    }

    // 関数ポインタをテンプレート引数で指定するfind_first。条件・計算量は上と同じ。
    template<bool (*f)(T)>
    int find_first(int r) const { return find_first(r,f); }
};

} // namespace sora
