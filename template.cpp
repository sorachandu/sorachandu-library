#if !__INCLUDE_LEVEL__
#include __FILE__

int main(){
    
}

#else
#include <bits/stdc++.h>
using namespace std;
struct Init { Init() { ios::sync_with_stdio(0); cin.tie(0); cout << setprecision(13); } }init;

using ll = long long;
using ull = unsigned long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;
template<typename T> using minpq=priority_queue<T,vector<T>,greater<T>>;

#define rep(i, x, limit) for (int i = (x); i < (limit); i++)
#define REP(i, x, limit) for (int i = (x); i <= (limit); i++)
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()
#define el '\n'
#define spa ' '
#define Yes cout << "Yes" << el
#define No cout << "No" << el
#define YES cout << "YES" << el
#define NO cout << "NO" << el
#define eps (1e-10)
#define Equals(a,b) (fabs((a) - (b)) < eps )
#define debug(x) cerr << #x << " = " << x << el

const double pi = 3.141592653589793238;
const int inf = 1073741823;
const ll infl = 1LL << 60;
const string ABC = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string abc = "abcdefghijklmnopqrstuvwxyz";

template<typename T1, typename T2>
std::ostream &operator<< (std::ostream &os, std::pair<T1,T2> p){
    os << "{" << p.first << "," << p.second << "}";
    return os;
}

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

template<typename T1, typename T2> inline void print_vec(const vector<pair<T1,T2>> &v, bool split_line=false){
    if(v.empty()){
        cout << "This vector is empty." << el;
        return;
    }
    for(int i = 0; i < (int)v.size(); i++){
        cout << '{';
        auto [a,b] = v[i];
        constexpr pair<bool,bool> isValue = {is_integral<T1>::value, is_integral<T2>::value};
        if constexpr(isValue.first){
            if(a==inf || a==infl) cout << "x,";
            else cout << a << ",";
        }else cout << a << ",";
        if constexpr(isValue.second){
            if(b==inf || b==infl) cout << "x,";
            else cout << b;
        }else cout << b;
        cout << "}" << " \n"[split_line || i+1==(int)v.size()];
    }
}

template<typename T1, typename T2> inline bool chmax(T1 &a, T2 b) {
    bool compare = a < b;
    if(compare) a = b;
    return compare;
}
template<typename T1, typename T2> inline bool chmin(T1 &a, T2 b) {
    bool compare = a > b;
    if(compare) a = b;
    return compare;
}

// std::chronoを利用した時間計測用クラス
class Timer{
    chrono::system_clock::time_point start;
    public:
        Timer() : start(chrono::system_clock::now()) {}
    
        double count(){
            chrono::duration<double> Time_ = chrono::system_clock::now() - start;
            return Time_.count();
        }

        bool is_under(double x){
            return (this -> count()) < x;
        }
};

// std::uniform_int_distributionを利用した一様乱数生成クラス
class Random_Gen{
    random_device seed_gen;
    mt19937 engine;
    uniform_int_distribution<int64_t> dist;
    public:
        // Constructor [l,r]で生成する値の範囲を指定
        Random_Gen() : engine(seed_gen()) {}
        Random_Gen(int64_t l, int64_t r) : engine(seed_gen()), dist(l,r) {}
        
        // 現在の生成する値の範囲をstd::pairで返す
        pair<int64_t,int64_t> get_range(){
            return make_pair(dist.min(),dist.max());
        }
        // 生成する値の範囲を[l,r]に変更する
        void set_range(int64_t l, int64_t r){
            uniform_int_distribution<int64_t>::param_type Param(l,r);
            dist.param(Param);
        }
        // [l,r]内の一様分布の整数を返す
        int64_t gen(){
            return dist(engine);
        }
        int64_t operator()(){ return gen(); }
};

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
        return comp(std::tie(vectors[i]...), std::tie(vectors[j]...));
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

#endif