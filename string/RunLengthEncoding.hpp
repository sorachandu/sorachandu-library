#pragma once

#include <iterator>
#include <string>
#include <utility>
#include <vector>

namespace sora {

// ランレングス圧縮
// 時間 O(N)、出力空間 O(N)。入力は変更しない。空入力の出力は空。
inline std::vector<std::pair<char,long long>> run_length_encoding(const std::string& s){
    if (s.empty()) return {};
    const int n = std::ssize(s);
    std::vector<std::pair<char,long long>> res;
    char c = s.front();
    long long cnt = 1;
    for(int i=1;i<n;i++){
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

// 配列に対するRLE。比較・コピーを O(1) として時間・出力空間 O(N)。入力は変更しない。
template<typename T> std::vector<std::pair<T,long long>> run_length_encoding(const std::vector<T>& v){
    if (v.empty()) return {};
    const int n=std::ssize(v);
    std::vector<std::pair<T,long long>> res;
    T e=v.front();
    long long cnt=1;
    for(int i=1;i<n;i++){
        if(e==v[i]) cnt++;
        else{
            res.emplace_back(e,cnt);
            cnt=1;
            e=v[i];
        }
    }
    res.emplace_back(e,cnt);
    return res;
}

} // namespace sora
