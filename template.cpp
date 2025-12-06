#include <bits/stdc++.h>
using namespace std;
#pragma region templates
struct Init { Init() { ios::sync_with_stdio(0); cin.tie(0); cout << setprecision(13); } }init;

using ll  = long long;
using ull = unsigned long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;
using i128= __int128_t;
template<typename T> using minpq=priority_queue<T,vector<T>,greater<T>>;

#define rep(i, x, limit) for(int i=(x); i< (limit); ++i)
#define REP(i, x, limit) for(int i=(x); i<=(limit); ++i)
#define all(x) std::begin(x), std::end(x)
#define rall(x) std::rbegin(x), std::rend(x)
#define el '\n'
#define spa ' '
#define Yes cout<<"Yes"<<el
#define No  cout<<"No" <<el
#define YES cout<<"YES"<<el
#define NO  cout<<"NO" <<el
#define END(x) cout<<(x)<<el, exit(0)
#define debug(x) cerr<<#x<<" = "<<x<<el

[[maybe_unused]] const int inf = 1073741823;
[[maybe_unused]] const ll infl = 1LL << 60;

std::string i128_to_str(const __int128_t &target){
    std::string res_str;
    __uint128_t target_tmp = target<0 ? -target : target;
    do{
        res_str+=target_tmp%10+'0';
        target_tmp/=10;
    }while(target_tmp!=0);
    if(target<0) res_str+='-';
    std::reverse(std::begin(res_str),std::end(res_str));
    return res_str;
}

// is_pair
template<typename T> constexpr bool is_pair_v = false;
template<typename T, typename U> constexpr bool is_pair_v<std::pair<T,U>> = true;

// istreamable
#if __cplusplus >= 202002L
template<typename T> concept istreamable_v = requires(T a){ std::cin>>a; };
#else // earlier C++20
template<typename T, typename=void> constexpr bool istreamable_v = false;
template<typename T> constexpr bool istreamable_v<T, std::void_t<decltype(std::cin>>std::declval<T&>())>> = true;
#endif

// ostreamable
#if __cplusplus >= 202002L
template<typename T> concept ostreamable_v = requires(T a){ std::cout<<a; };
#else // earlier C++20
template<typename T, typename=void> constexpr bool ostreamable_v = false;
template<typename T> constexpr bool ostreamable_v<T, std::void_t<decltype(std::cout<<std::declval<T&>())>> = true;
#endif

// iterable
#if __cplusplus >= 202002L
#   if __has_include(<ranges>)
template<typename T> concept iterable_v = std::ranges::range<T>;
#   else // C++20, not has <ranges>
template<typename T> concept iterable_v = requires(T a){ std::begin(a); std::end(a); };
#   endif
#else // earlier C++20
template<typename T> constexpr bool iterable_v = std::is_same_v<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>;
#endif

// concepts for debug use only:
#if __cplusplus >= 202002L
// stack_like_v (std::stack, std::priority_queue)
template<typename T> concept stack_like_v = requires(T t){
    requires(
        std::same_as<decltype(t.top()), typename T::reference> ||
        std::same_as<decltype(t.top()), typename T::const_reference>
    );
    { t.pop() };
    { t.empty() } -> std::same_as<bool>;
};

// queue_like_v (std::queue)
template<typename T> concept queue_like_v = requires(T t){
    { t.front() } -> std::same_as<typename T::reference&>;
    { t.pop() };
    { t.empty() } -> std::same_as<bool>;
};
#endif

void input(){ return; }
template<typename T, typename... Rest>
void input(T &target, Rest&... rest){
    if constexpr(istreamable_v<T>) cin>>target;
    else if constexpr(iterable_v<T>) for(auto &e:target) input(e);
    else if constexpr(is_pair_v<T>){
        input(target.first);
        input(target.second);
    }else if constexpr(std::is_convertible_v<long long, T>){
        long long val;
        input(val);
        target=val;
    }else{
        cerr<<"Invalid Input: Unreadable variable detected"<<std::endl;
        assert(false);
    }
    input(rest...);
}

// output for debugging
template<typename T>
void write_value(const T &target){
    using V = std::decay_t<T>;
    if constexpr(ostreamable_v<V>){
        std::cout<<target;
    }else if constexpr(is_pair_v<V>){
        std::cout<<'{';
        write_value(target.first);
        std::cout<<',';
        write_value(target.second);
        std::cout<<'}';
    }else if constexpr(iterable_v<V>){
        std::cout<<'[';
        bool first=true;
        for(const auto &e:target){
            if(!first) std::cout<<", ";
            write_value(e);
            first=false;
        }
        std::cout<<']';
    }else if constexpr(std::is_convertible<V, __int128_t>::value){
        write_value(i128_to_str(target));
    }else if constexpr(std::is_convertible_v<V, long long>){
        write_value(static_cast<long long>(target));
    }else if constexpr(stack_like_v<V>){
        auto tmp=target;
        std::cout<<'[';
        bool first=true;
        while(!tmp.empty()){
            if(!first) std::cout<<", ";
            write_value(tmp.top());
            tmp.pop();
            first=false;
        }
        std::cout<<']';
    }else if constexpr(queue_like_v<V>){
        auto tmp=target;
        std::cout<<'[';
        bool first=true;
        while(!tmp.empty()){
            if(!first) std::cout<<", ";
            write_value(tmp.front());
            tmp.pop();
            first=false;
        }
        std::cout<<']';
    }
    else{
        std::cerr<<"Invalid Output: Unwritable variable detected"<<std::endl;
        assert(false);
    }
}

void output(){ std::cout<<'\n'; }
template<typename T, typename... Rest>
void output(const T &target, const Rest&... rest){
    write_value(target);
    if constexpr(sizeof...(rest)>0){
        std::cout<<' ';
        output(rest...);
    }else{
        std::cout<<'\n';
    }
}

template<typename T1,typename T2> bool chmin(T1 &a,T2 b){return a>b?a=b,true:false;}
template<typename T1,typename T2> bool chmax(T1 &a,T2 b){return a<b?a=b,true:false;}

// a^bを返す オーバーフローに注意
ll Pow(ll a,ll b){
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

// Pythonのenumerateみたいなやつ　[index,value]を範囲for文に提供
template<typename T> vector<pair<int,T>> enumerate(const vector<T> &v){
    vector<pair<int,T>> res(ssize(v));
    for(int i=0;i<ssize(v);i++) res[i]={i,v[i]};
    return res;
}
vector<pair<int,char>> enumerate(const string &s){
    vector<pair<int,char>> res(ssize(s));
    for(int i=0;i<ssize(s);i++) res[i]={i,s[i]};
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

#pragma endregion templates

int main(){
    
}