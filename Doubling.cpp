/***
N要素,各要素からの遷移先が一意に定まる,遷移は最大K回 の条件のもとダブリングをする
構築は時間, 空間計算量ともにO(NlogK) クエリはθ(logK)
各要素から1回遷移したN要素の配列とKを渡すこと なおKのデフォルト値は2^60
***/
template<typename T> class Doubling{
    ull K;
    uint n, N;
    vector<vector<T>> dp;
    
    public:
        Doubling(vector<T> v, ull k = 1ull<<60) :
            K(k), n(uint(v.size())), N(bit_width(K)){
                dp.resize(N);
                rep(i,0,N) dp[i].resize(n);
                dp[0] = v;
                rep(i,1,N){
                    rep(j,0,n){
                        dp[i][j] = dp[i-1][dp[i-1][j]];
                    }
                }
            }
        
        // 要素pからk回遷移した要素を返す
        T query(T p, ull k){
            assert(0 <= p && p < n && k <= K);
            rep(i,0,N){
                if(1ull<<i & k) p = dp[i][p];
            }
            return p;
        }
};