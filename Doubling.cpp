/***
閉じた単射の遷移関数,遷移は最大K回 の条件のもとDoubling
構築は時間, 空間計算量ともにO(NlogK) クエリはθ(logK)
各要素から1回遷移したN要素の配列とKを渡すこと なおKのデフォルト値は2^60
重み付きにする場合はコメントアウトを外してよしなに
https://atcoder.jp/contests/abc241/submissions/74233007
https://atcoder.jp/contests/abc438/submissions/74246549
***/
template<typename T> class Doubling{
    using ull=unsigned long long;
    ull K;
    uint n, N;
    vector<vector<T>> dp;
    // vector<vector<T>> sum;
    
public:
    Doubling() : K(0), n(0), N(0) {}
    Doubling(vector<T> v, ull k = 1ull<<60) :
        K(k), n(uint(v.size())), N(bit_width(K)){
            dp.resize(N);
            for(uint i=0;i<N;i++) dp[i].resize(n);
            dp[0] = v;
            // sum.resize(N);
            // for(uint i=0;i<N;i++) sum[i].resize(n);
            // for(uint i=0;i<n;i++) sum[0][i]=初期重み値
            for(uint i=1;i<N;i++){
                for(uint j=0;j<n;j++){
                    dp[i][j] = dp[i-1][dp[i-1][j]];
                    // sum[i][j] = sum[i-1][j]+sum[i-1][dp[i-1][j]];
                }
            }
        }
    
    // 要素pからk回遷移した要素を返す
    T query(T p, ull k){
        assert(0 <= p && p < n && k <= K);
        for(uint i=0;i<N;i++){
            if(1ull<<i & k){
                // res+=sum[i][p];
                p = dp[i][p];
            }
        }
        return p;
    }
};