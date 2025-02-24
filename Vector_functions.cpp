#include <bits/stdc++.h>
using namespace std;
const int inf = 1073741823;
#define rep(i, x, limit) for (int i = (int)x; i < (int)limit; i++)
#define REP(i, x, limit) for (int i = (int)x; i <= (int)limit; i++)
template<class T> inline bool chmax(T& a, T b) {
    if (a < b) {
        a = b;
        return true;
    }
    return false;
}
template<class T> inline bool chmin(T& a, T b) {
    if (a > b) {
        a = b;
        return true;
    }
    return false;
}

// 引数を直接いじるんじゃなくて、あくまでも操作後の配列を返り値として返すことに注意.

// {'.','#'} からなるようなvector<string>に対し,'#'を含まない(余分な)長方形領域を切り出したものを返す
// 制約: 引数は空配列でないこと O(H*W)
vector<string> trimming(const vector<string>& s){
    int a = inf, b = inf, c = 0, d = 0;
    int n = s.size();
    int m = s[0].size();
    rep(i,0,n){
        rep(j,0,m){
            if(s[i][j] == '#'){
                chmin(a,i);
                chmin(b,j);
                chmax(c,i);
                chmax(d,j);
            }
        }
    }
    vector<string> t;
    if(a == inf) t.resize(0);
    else{
        REP(i,a,c){
            string str;
            REP(j,b,d) str.push_back(s[i][j]);
            t.emplace_back(str);
        }
    }
    return t;
}
// 二次元配列<string>を反時計回りに90度だけ回転させたものを返す
// 制約: 引数は空配列でないこと ジャグ配列でないこと O(H*W)
vector<string> rotate_vec(const vector<string>& vec) {
    if (vec.empty()) return {};
    vector<string> rotated(vec[0].size(), string(vec.size(), ' '));
    for (int i = 0; i < (int)vec.size(); ++i) {
        for (int j = 0; j < (int)vec[i].size(); ++j) {
            rotated[j][(int)vec.size() - i - 1] = vec[i][j];
        }
    }
    return rotated;
}
// 二次元配列を反時計回りに90度だけ回転させたものを返す
// 制約: 引数は空配列でないこと ジャグ配列でないこと O(H*W)
template<typename T> inline vector<vector<T>> rotate_vec(const vector<vector<T>>& vec){
    if (vec.empty()) return {};
    vector<vector<T>> rotated(vec[0].size(), vector<T>(vec.size()));
    for (int i = 0; i < (int)vec.size(); ++i) {
        for (int j = 0; j < (int)vec[i].size(); ++j) {
            rotated[j][(int)vec.size() - i - 1] = vec[i][j];
        }
    }
    return rotated;
}