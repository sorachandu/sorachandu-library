#include <bits/stdc++.h>
using namespace std;
struct Init { Init() { ios::sync_with_stdio(0); cin.tie(0); cout << setprecision(13); } }init;

using ll = long long;
using ull = unsigned long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;
template<typename T> using minpq=priority_queue<T,vector<T>,greater<T>>;

#define rep(i, x, limit) for(int i=(x); i< (limit); ++i)
#define REP(i, x, limit) for(int i=(x); i<=(limit); ++i)
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define el '\n'
#define spa ' '
#define Yes cout<<"Yes"<<el
#define No  cout<<"No" <<el
#define YES cout<<"YES"<<el
#define NO  cout<<"NO" <<el
#define END(x) cout<<(x)<<el, exit(0)
#define debug(x) cerr<<#x<<" = "<<x<<el

const int inf = 1073741823;
const ll infl = 1LL << 60;
const string ABC = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string abc = "abcdefghijklmnopqrstuvwxyz";

template<typename T1, typename T2>
std::ostream &operator<< (std::ostream &os, std::pair<T1,T2> p){
    os << "{" << p.first << "," << p.second << "}";
    return os;
}

template<typename T1,typename T2> inline bool chmin(T1 &a,T2 b){return a>b?a=b,true:false;}
template<typename T1,typename T2> inline bool chmax(T1 &a,T2 b){return a<b?a=b,true:false;}

// a^bを返す オーバーフローに注意
inline ll Pow(ll a,ll b){
    assert(b>=0);
    if(a==0 and b==0) return 1;
    if(a==1) return 1;
    if(a==-1) return (b&1)?-1:1;
    ll res=1;
    while(b){
        if(b&1) res*=a;
        b>>=1;
        if(b) a*=a;
    }
    return res;
}

// 配列の要素を空白区切りで出力 第二引数をtrueにすると改行区切り
template<typename T> inline void print_vec(const vector<T> &v, bool split_line=false) {
    if(v.empty()){
        cout << "This vector is empty." << el;
        return;
    }
    constexpr bool isValue = is_integral<T>::value;
    for (int i = 0; i < (int)v.size(); i++) {
        if constexpr(isValue){
            if((v[i]==inf) || (v[i]==infl)) cout << 'x' << " \n"[split_line || i+1==(int)v.size()];
            else cout << v[i] << " \n"[split_line || i+1==(int)v.size()];
        }else cout << v[i] << " \n"[split_line || i+1==(int)v.size()];
    }
}

// Pythonのenumerateみたいなやつ　[index,value]を範囲for文に提供
template<typename T> inline vector<pair<int,T>> enumerate(const vector<T> &v){
    vector<pair<int,T>> res(ssize(v));
    for(int i=0;i<ssize(v);i++){
        res[i]={i,v[i]};
    }
    return res;
}

// This function sorts multiple vectors based on the first vector
// and returns the indices of the sorted order.
// Note: First argument is a comparison function.
template <typename Compare, typename... Vectors>
vector<size_t> multipleSort(Compare comp = Compare(), Vectors&... vectors) {
    const size_t size = std::get<0>(std::tie(vectors...)).size();
    ((void)std::initializer_list<int>{(vectors.size() == size ? 0 : 
        throw std::invalid_argument("Vectors must have the same size"))...});

    std::vector<size_t> indices(size);
    std::iota(indices.begin(), indices.end(), 0);

    std::sort(indices.begin(), indices.end(), [&](size_t i, size_t j) {
        return comp(std::get<0>(std::tie(vectors...))[i], std::get<0>(std::tie(vectors...))[j]);
    });

    auto reorder = [&](auto& vec) {
        auto temp=vec;
        for (size_t i = 0; i < size; ++i) {
            vec[i] = temp[indices[i]];
        }
    };
    (reorder(vectors), ...);
    return indices;
}

int main(){
    
}