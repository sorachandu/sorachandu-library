#include <bits/stdc++.h>
using namespace std;
#pragma region templates
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

[[maybe_unused]] const int inf = 1073741823;
[[maybe_unused]] const ll infl = 1LL << 60;

// std::pair
template<typename T1, typename T2>
std::ostream &operator<< (std::ostream &os, std::pair<T1,T2> p){
    os << "{" << p.first << "," << p.second << "}";
    return os;
}
// std::vector
template<typename T> std::ostream &operator<< (std::ostream &os, const std::vector<T> &v){
    os << "[";
    for(auto iter=v.begin();iter!=v.end();) os << *iter << (++iter!=v.end()?", ":"");
    os << "]";
    return os;
}
// std::stack
template<typename T> std::ostream &operator<< (std::ostream &os, std::stack<T> st){
    os << "[";
    while(!st.empty()){ T e=st.top(); st.pop(); os << e << (!st.empty()?", ":""); }
    os << "]";
    return os;
}
// std::queue
template<typename T> std::ostream &operator<< (std::ostream &os, std::queue<T> q){
    os << "[";
    while(!q.empty()){ T e=q.front(); q.pop(); os << e << (!q.empty()?", ":""); }
    os << "]";
    return os;
}
// std::priority_queue
template<typename T> std::ostream &operator<< (std::ostream &os, std::priority_queue<T> pq){
    os << "[";
    while(!pq.empty()){ T e=pq.top(); pq.pop(); os << e << (!pq.empty()?", ":""); }
    os << "]";
    return os;
}
// minpq
template<typename T> std::ostream &operator<< (std::ostream &os, minpq<T> pq){
    os << "[";
    while(!pq.empty()){ T e=pq.top(); pq.pop(); os << e << (!pq.empty()?", ":""); }
    os << "]";
    return os;
}
// std::deque
template<typename T> std::ostream &operator<< (std::ostream &os, const std::deque<T> &dq){
    os << "[";
    for(auto iter=dq.begin();iter!=dq.end();) os << *iter << (++iter!=dq.end()?", ":"");
    os << "]";
    return os;
}
// std::array
template<typename T, size_t N> std::ostream &operator<< (std::ostream &os, const std::array<T,N> &arr){
    os << "[";
    for(size_t i=0;i<N;i++) os << arr[i] << (i+1!=N?", ":"");
    os << "]";
    return os;
}
// std::set
template<typename T> std::ostream &operator<< (std::ostream &os, const std::set<T> &st){
    os << "{";
    for(auto iter=st.begin();iter!=st.end();) os << *iter << (++iter!=st.end()?", ":"");
    os << "}";
    return os;
}
// std::map
template<typename T1, typename T2> std::ostream &operator<< (std::ostream &os, const std::map<T1,T2> &mp){
    os << "{";
    for(auto iter=mp.begin();iter!=mp.end();) os << iter->first << ":" << iter->second << (++iter!=mp.end()?", ":"");
    os << "}";
    return os;
}
// std::unordered_set
template<typename T> std::ostream &operator<< (std::ostream &os, const std::unordered_set<T> &st){
    os << "{";
    for(auto iter=st.begin();iter!=st.end();) os << *iter << (++iter!=st.end()?", ":"");
    os << "}";
    return os;
}
// std::multiset
template<typename T> std::ostream &operator<< (std::ostream &os, const std::multiset<T> &st){
    os << "{";
    for(auto iter=st.begin();iter!=st.end();) os << *iter << (++iter!=st.end()?", ":"");
    os << "}";
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

// Pythonのenumerateみたいなやつ　[index,value]を範囲for文に提供
template<typename T> inline vector<pair<int,T>> enumerate(const vector<T> &v){
    vector<pair<int,T>> res(ssize(v));
    for(int i=0;i<ssize(v);i++) res[i]={i,v[i]};
    return res;
}
inline vector<pair<int,char>> enumerate(const string &s){
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