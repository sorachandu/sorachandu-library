/*//--------------------------------------------------------
座標圧縮
座標圧縮したいvector<T>を渡す operator[]かgetで圧縮後の値にアクセス
構築 O(NlogN), 値取得 O(1)
*///--------------------------------------------------------
template<typename T> struct Compression{
    int n;
    std::vector<T> cmp;
    std::vector<int> id;
    Compression(const std::vector<T> &a) : n(std::ssize(a)), cmp(a), id(n) {
        std::sort(cmp.begin(),cmp.end());
        cmp.erase(std::unique(cmp.begin(),cmp.end()),cmp.end());
        for(int i=0;i<n;i++) id[i]=std::lower_bound(cmp.begin(),cmp.end(),a[i])-cmp.begin();
    }

    const int &operator[](int k) const {
        return id[k];
    }

    int get(int k) const {
        return id[k];
    }

    int size() const {
        return ssize(cmp);
    }

    int get_from_val(T val){
        return std::lower_bound(cmp.begin(),cmp.end(),val)-cmp.begin();
    }
};