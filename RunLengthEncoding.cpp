#include<bits/stdc++.h>
using namespace std;

// ランレングス圧縮
// 制約: 空でないstd::string
vector<pair<char,long long>> const RunLengthEncoding(const string& s){
    assert(!s.empty());
    const int n = ssize(s);
    vector<pair<char,long long>> res;
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

// 配列に対するRLE
template<typename T> vector<pair<T,long long>> const RunLengthEncoding(const vector<T>& v){
    assert(!v.empty());
    const int n=ssize(v);
    vector<pair<T,long long>> res;
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