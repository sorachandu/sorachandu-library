#include <bits/stdc++.h>
using namespace std;
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

ull h,w;
// 場外判定
    auto outof = [&](ull y, ull x){
        return (y >= h || x >= w);
    };
    // 4方向の近傍 右,下,左,上
    const int dy[4] = {0, 1, 0, -1};
    const int dx[4] = {1, 0, -1, 0};

    // 8方向の近傍 右,右下,下,左下,左,左上,上,右上
    const int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    const int dx[8] = {1, 1, 0, -1, -1, -1, 0, 1};


// 1次元と2次元の相互変換
    auto convert_1Dto2D = [&](ull idx){
        ull y = idx/w;
        ull x = idx%w;
        return make_pair(y,x);
    };
    auto convert_2Dto1D = [&](ull y, ull x){
        return y*w+x;
    };

// 二分探索による、浮動小数点型を介さないsqrt
// 制約：0 <= x <= LLONG_MAX
ll ll_sqrt(ll x){
    assert(0 <= x);
    ll ok = 0, ng = x/2+2;
    while(abs(ok-ng) > 1){
        ll mid = (ok+ng)/2;
        if(x/mid < mid) ng = mid;
        else ok = mid;
    }
    return ok;
}

// 座標圧縮
template<typename T> inline vector<uint> compression(vector<T> &a){
    int n = a.size();
    vector<uint> res(n);
    vector b = a;
    sort(all(b));
    b.erase(unique(all(b)),b.end());
    rep(i,0,n){
        auto iter = lower_bound(all(b),a[i]);
        res[i] = iter - b.begin();
    }
    return res;
}

// 浮動小数点型を介さず、aを底とした対数関数 log_a(x)以上の最小の整数を返す
// 制約: 0 < x <= LLONG_MAX, 1 < a <= LLONG_MAX
ll ll_log(ll x, ll a = 2){
    assert(x > 0 && a > 1);
    ll res = 0;
    while(x > 1){
        x = (x+a-1)/a;
        res++;
    }
    return res;
}

// ランレングス圧縮
// 制約: 空でないstd::string
inline vector<pair<char,ll>> RunLengthEncoding(const string& s){
    assert(!s.empty());
    int n = s.size();
    vector<pair<char,ll>> res;
    char c = s.front();
    ll cnt = 1;
    rep(i,1,n){
        if(c==s[i]) cnt++;
        else{
            res.emplace_back(c, cnt);
            cnt = 1;
            c = s[i];
        }
    }
    res.emplace_back(c, cnt);
    return res;
}

// 回文判定
bool is_palindrome(string s){
    bool isok = true;
    int n = s.size();
    rep(i,0,n/2){
        if(s[i] != s[n-i-1]){
            isok = false;
            break;
        }
    }
    return isok;
}

// 高速な冪乗計算(x^n), 計算中及び答えは1e9+7で割った余り
const ll MOD = 1e9 + 7;
ll Pow(ll x, ll n, ll mod = MOD){
    ll res = 1;
    while(n > 0){
        if(n&1) res = res*x % mod;
        x = x*x %mod;
        n >>= 1;
    }
    return res;
}

// 素因数分解
vector<pll> prime_fact(ll k) {
    vector<pll> v;
    for (ll i = 2; i * i <= k; i++) {
        ll cnt = 0;
        while (k % i == 0) {
            cnt++;
            k /= i;
        }

        if (cnt != 0) v.push_back({i, cnt});
    }
    if (k != 1) v.push_back({k, 1});
    return v;
}

// 負の数にも対応した剰余演算
ll modulo(ll val, ll m) {
    ll res = val % m;
    if (res < 0) res += m;
    return res;
}

// めぐる式二分探索
ll bisearch(vector<ll>& v, ll key) {
    ll ng = -1;
    ll ok = (ll)v.size();

    while (abs(ok - ng) > 1) {
        ll mid = (ok + ng) / 2;
        if (v[mid] >= key)
            ok = mid;
        else
            ng = mid;
    }
    return ok;
}

class UnionFind {
   public:
    UnionFind() = default;

    // n 個の要素
    explicit UnionFind(size_t n) : m_parents(n), m_sizes(n, 1) {
        iota(m_parents.begin(), m_parents.end(), 0);
    }

    // i の root を返す
    int find(int i) {
        if (m_parents[i] == i) {
            return i;
        }
        // 経路圧縮
        return (m_parents[i] = find(m_parents[i]));
    }

    // a の木と b の木を統合
    void merge(int a, int b) {
        a = find(a);
        b = find(b);

        if (a != b) {
            // union by size (小さいほうが子になる）
            if (m_sizes[a] < m_sizes[b]) {
                swap(a, b);
            }

            m_sizes[a] += m_sizes[b];
            m_parents[b] = a;
        }
    }

    // a と b が同じ木に属すかを返す
    bool connected(int a, int b) { return (find(a) == find(b)); }

    // i が属するグループの要素数を返す
    int size(int i) { return m_sizes[find(i)]; }

   private:
    // m_parents[i] は i の 親,
    // root の場合は自身が親
    vector<int> m_parents;

    // グループの要素数 (root 用)
    // i が root のときのみ, m_sizes[i] はそのグループに属する要素数を表す
    vector<int> m_sizes;
};

// Xorshift, rand()より高品質かつ高速な乱数生成
unsigned int randInt() {
    static unsigned int tx = 123456789, ty = 362436069, tz = 521288629,
                        tw = 88675123;
    unsigned int tt = (tx ^ (tx << 11));
    tx = ty;
    ty = tz;
    tz = tw;
    return (tw = (tw ^ (tw >> 19)) ^ (tt ^ (tt >> 8)));
}

// 各桁の和を求める res+=をres++に変更すると桁数を求められる
ll digsum(ll n) {
    ll res = 0ll;
    while (n > 0) {
        res += n % 10;
        n /= 10;
    }
    return res;
}

// 拡張Euclidの互除法による逆元計算
ll modinv(ll a, ll m) {
    ll b = m, u = 1, v = 0;
    while (b) {
        ll t = a / b;
        a -= t * b;
        swap(a, b);
        u -= t * v;
        swap(u, v);
    }
    u %= m;
    if (u < 0) u += m;
    return u;
}

// 素数判定
bool IsPrime(ll num) {
    if (num < 2)
        return false;
    else if (num == 2)
        return true;
    else if (num % 2 == 0)
        return false;  // 偶数はあらかじめ除く

    double sqrtNum = sqrt(num);
    for (int i = 3; i <= sqrtNum; i += 2) {
        if (num % i == 0) {
            // 素数ではない
            return false;
        }
    }

    // 素数である
    return true;
}

// nをbase_number進数に変換する
string to_oct(ll n, int base_number) {
    string s;
    while (n) {
        s = to_string(n % base_number) + s;
        n /= base_number;
    }
    return s;
}

// 二点間の距離を求める
double getDistance(ll x, ll y) { return sqrt((x * x) + (y * y)); }
// p.s. hypot(double x, double y) というSTLで同じことができる
