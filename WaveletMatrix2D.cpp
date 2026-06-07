#include<bits/stdc++.h>
using namespace std;


/*//--------------------------------------------------------
Implemented by Gemini 3.1 Pro
区間[l,r)の中の a<=A_i<b を満たす値の種類数を O(log(σ)log(N)) で求められる
*///--------------------------------------------------------
// 5e5くらいまで対応
const int D_VAL=19; // 配列A_iの最大値
const int D_PRE=19; // 配列preの最大値 = Aの配列長

// 1. 簡潔ビットベクトル
struct BitVector {
    int length, blocks;
    vector<uint64_t> bit;
    vector<int> rui;

    BitVector() : length(0), blocks(0) {}
    BitVector(int n) {
        length = n;
        blocks = (n + 63) / 64;
        bit.assign(blocks, 0ULL);
        rui.assign(blocks + 1, 0);
    }
    void set(int pos) { bit[pos / 64] |= (1ULL << (pos % 64)); }
    void build() {
        for (int i = 0; i < blocks; ++i) {
            rui[i + 1] = rui[i] + __builtin_popcountll(bit[i]);
        }
    }
    int rank1(int pos) const {
        if (pos <= 0) return 0;
        if (pos > length) pos = length;
        int b = pos / 64;
        int rem = pos % 64;
        uint64_t mask = (rem == 0) ? 0 : ((1ULL << rem) - 1);
        return rui[b] + __builtin_popcountll(bit[b] & mask);
    }
    int rank0(int pos) const { return pos - rank1(pos); }
    int count1() const { return rui[blocks]; }
};

// 2. 内側の Wavelet Matrix (pre配列の処理用)
struct InnerWM {
    int n;
    BitVector B[D_PRE];
    int zeros[D_PRE];

    InnerWM() : n(0) {}
    InnerWM(vector<int> v) {
        n = v.size();
        if (n == 0) return;
        vector<int> cur = v;
        for (int lvl = 0; lvl < D_PRE; ++lvl) {
            int d = D_PRE - 1 - lvl;
            B[lvl] = BitVector(n);
            for (int i = 0; i < n; ++i) if ((cur[i] >> d) & 1) B[lvl].set(i);
            B[lvl].build();
            zeros[lvl] = n - B[lvl].count1();

            vector<int> next_v(n);
            int idx0 = 0, idx1 = zeros[lvl];
            for (int i = 0; i < n; ++i) {
                if ((cur[i] >> d) & 1) next_v[idx1++] = cur[i];
                else next_v[idx0++] = cur[i];
            }
            cur = next_v;
        }
    }

    int count_less_than(int l, int r, int val) const {
        if (l >= r || val <= 0) return 0;
        int ans = 0;
        for (int lvl = 0; lvl < D_PRE; ++lvl) {
            if (l >= r) break;
            int d = D_PRE - 1 - lvl;
            int l0 = B[lvl].rank0(l), r0 = B[lvl].rank0(r);
            int l1 = B[lvl].rank1(l) + zeros[lvl], r1 = B[lvl].rank1(r) + zeros[lvl];

            if ((val >> d) & 1) {
                ans += (r0 - l0);
                l = l1; r = r1;
            } else {
                l = l0; r = r0;
            }
        }
        return ans;
    }
};

// 3. 外側の Wavelet Matrix (A配列の処理と全体の制御)
struct WaveletMatrix2D {
    int n;
    BitVector B[D_VAL];
    int zeros[D_VAL];
    InnerWM inner_WM[D_VAL + 1];

    WaveletMatrix2D(vector<int> A, vector<int> pre) {
        n = A.size();
        vector<int> cur_A = A;
        vector<int> cur_pre = pre;

        for (int lvl = 0; lvl < D_VAL; ++lvl) {
            int d = D_VAL - 1 - lvl;
            inner_WM[lvl] = InnerWM(cur_pre); 

            B[lvl] = BitVector(n);
            for (int i = 0; i < n; ++i) if ((cur_A[i] >> d) & 1) B[lvl].set(i);
            B[lvl].build();
            zeros[lvl] = n - B[lvl].count1();

            vector<int> next_A(n), next_pre(n);
            int idx0 = 0, idx1 = zeros[lvl];
            for (int i = 0; i < n; ++i) {
                if ((cur_A[i] >> d) & 1) {
                    next_A[idx1] = cur_A[i];
                    next_pre[idx1] = cur_pre[i];
                    idx1++;
                } else {
                    next_A[idx0] = cur_A[i];
                    next_pre[idx0] = cur_pre[i];
                    idx0++;
                }
            }
            cur_A = next_A;
            cur_pre = next_pre;
        }
        inner_WM[D_VAL] = InnerWM(cur_pre); 
    }

    // [l, r) 内で A_i < val かつ pre_i < limit を満たす個数
    int count_prefix(int l, int r, int val, int limit) const {
        if (l >= r || val <= 0) return 0;
        int L = l, R = r, ans = 0;

        for (int lvl = 0; lvl < D_VAL; ++lvl) {
            if (L >= R) break;
            int d = D_VAL - 1 - lvl;
            
            int L0 = B[lvl].rank0(L), R0 = B[lvl].rank0(R);
            int L1 = B[lvl].rank1(L) + zeros[lvl], R1 = B[lvl].rank1(R) + zeros[lvl];

            if ((val >> d) & 1) {
                // 0側に分岐する要素はすべて val 未満。それらの pre 条件を inner_WM で判定
                ans += inner_WM[lvl + 1].count_less_than(L0, R0, limit);
                L = L1; R = R1;
            } else {
                L = L0; R = R0;
            }
        }
        return ans;
    }

    // [l, r) 内で a <= A_i < b かつ初回出現のものの個数
    int query(int l, int r, int a, int b) const {
        // pre配列は構築時に +1 されているため、条件は pre < l + 1 となる
        int limit = l + 1;
        return count_prefix(l, r, b, limit) - count_prefix(l, r, a, limit);
    }
};




/*//--------------------------------------------------------
以下使用例　ABC174-F https://atcoder.jp/contests/abc174/submissions/76509432
*///--------------------------------------------------------
int main(){
    cin.tie(nullptr)->ios::sync_with_stdio(false);
    int N,Q;
    cin>>N>>Q;
    vector<int> C(N);
    for(int i=0;i<N;i++) cin>>C[i],C[i]--;
    vector<int> pre(N); // pre[i]:=C[i]と同一の要素が出た前回のindex 1-indexed
    vector<int> _last(N); // 補助配列
    for(int i=0;i<N;i++){
        pre[i]=_last[C[i]];
        _last[C[i]]=i+1;
    }
    WaveletMatrix2D wm(C,pre);
    while(Q--){
        int l,r;
        cin>>l>>r;
        l--;
        cout<<wm.query(l,r,0,N)<<"\n";
    }
}