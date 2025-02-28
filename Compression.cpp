/***
 座標圧縮
 座標圧縮したいvector<T>を渡す operator[]かgetで圧縮後の値にアクセス
 構築 O(NlogN), 値取得 O(1)
***/
template<typename T> struct Compression{
    int n;
    vector<T> val;
    vector<T> res;
    vector<int> id;
    Compression(const vector<T> &a) : n((int)a.size()), val(a), res(a), id(n) {
        sort(res.begin(),res.end());
        res.erase(unique(res.begin(),res.end()),res.end());
        for(int i=0;i<n;i++) id[i]=lower_bound(res.begin(),res.end(),val[i])-res.begin();
    }

    const int &operator[](int k) const {
        return id[k];
    }

    inline int get(int k) const {
        return id[k];
    }

    inline T getRealVal(int k) const {
        return val[k];
    }
};