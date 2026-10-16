#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <vector>

namespace sora {

/*//--------------------------------------------------------
Implemented by Gemini 3.1 Pro (Reviewed GPT-6.1-sol)
区間[l,r)の中の a<=A_i<b を満たす値の種類数を O(log(σ)log(N)) で求められる
*///--------------------------------------------------------
// 5e5くらいまで対応
const int D_VAL=19; // 配列A_iの最大値
const int D_PRE=19; // 配列preの最大値 = Aの配列長

// 1. 簡潔ビットベクトル
struct BitVector {
    int length, blocks;
    std::vector<std::uint64_t> bit;
    std::vector<int> rui;

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
        std::uint64_t mask = (rem == 0) ? 0 : ((1ULL << rem) - 1);
        return rui[b] + __builtin_popcountll((rem == 0 ? 0ULL : bit[b] & mask));
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
    InnerWM(std::vector<int> v) {
        n = v.size();
        if (n == 0) return;
        std::vector<int> cur = v;
        for (int lvl = 0; lvl < D_PRE; ++lvl) {
            int d = D_PRE - 1 - lvl;
            B[lvl] = BitVector(n);
            for (int i = 0; i < n; ++i) if ((cur[i] >> d) & 1) B[lvl].set(i);
            B[lvl].build();
            zeros[lvl] = n - B[lvl].count1();

            std::vector<int> next_v(n);
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
        if (val >= (1 << D_PRE)) return r - l;
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

// 位置区間と値域を指定して、異なる値の個数を求める静的データ構造。
class RangeDistinctCount {
    int n;
    BitVector B[D_VAL];
    int zeros[D_VAL];
    InnerWM inner_WM[D_VAL + 1];

public:
    // 入力は変更しない。前提: N < 2^19、各値は [0, 2^19)。空配列も可。
    // 構築時間・空間 O(N D_VAL D_PRE + U)。U は最大値 + 1（空なら 0）。
    explicit RangeDistinctCount(const std::vector<int>& A) {
        assert(A.size() < (1U << D_PRE));
        n = static_cast<int>(A.size());
        std::vector<int> cur_A = A;
        int value_count = 0;
        for (int value : A) {
            assert(0 <= value && value < (1 << D_VAL));
            value_count = std::max(value_count, value + 1);
        }
        std::vector<int> cur_pre(n);
        {
            std::vector<int> last(value_count, 0);
            for (int i = 0; i < n; ++i) {
                cur_pre[i] = last[A[i]];
                last[A[i]] = i + 1;
            }
        }

        for (int lvl = 0; lvl < D_VAL; ++lvl) {
            int d = D_VAL - 1 - lvl;
            inner_WM[lvl] = InnerWM(cur_pre); 

            B[lvl] = BitVector(n);
            for (int i = 0; i < n; ++i) if ((cur_A[i] >> d) & 1) B[lvl].set(i);
            B[lvl].build();
            zeros[lvl] = n - B[lvl].count1();

            std::vector<int> next_A(n), next_pre(n);
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

private:
    // [l, r) 内で A_i < val かつ pre_i < limit を満たす個数。時間 O(D_VAL D_PRE)。
    int count_prefix(int l, int r, int val, int limit) const {
        if (l >= r || val <= 0) return 0;
        if (val >= (1 << D_VAL))
            return inner_WM[0].count_less_than(l, r, limit);
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

public:
    // [l, r) 内の a <= A_i < b を満たす値の種類数。時間 O(D_VAL D_PRE)。
    // 位置・値域とも半開区間。空区間・空の値域では 0 を返す。
    int query(int l, int r, int a, int b) const {
        assert(0 <= l && l <= r && r <= n);
        assert(a <= b);
        // pre配列は構築時に +1 されているため、条件は pre < l + 1 となる
        int limit = l + 1;
        return count_prefix(l, r, b, limit) - count_prefix(l, r, a, limit);
    }
};

} // namespace sora
