struct frac{
    long long num,den;
    int8_t state;
    frac(){
        num=den=0;
        state=-1;
    }
    frac(long long a) : frac(a,1) {}
    frac(long long a,long long b) : num(a), den(b){
        if(b==0){
            // 分母=0
            state=0;
            num=1;
            den=0;
        }else{
            state=1;
            normalize();
        }
    };

    inline void normalize(){
        assert(state==1);
        if(den<0) num=-num,den=-den;
        long long g=gcd(std::abs(num),std::abs(den));
        num/=g;
        den/=g;
    }

    inline pair<long long,long long> get_pair() const {
        return {num,den};
    }
    inline long long get_num() const {
        return num;
    }
    inline long long get_den() const {
        return den;
    }

    // is 分母==0 or 未初期化?
    inline bool isNaN() const {
        return state!=1;
    }

    // 任意のnum,denを超過する適当な定数を渡して1次元化
    // オーバーフローとかに気を付けて
    inline long long convert1D(long long VAL) const {
        return num*VAL+den;
    }

    // double値への変換
    double to_double() const {
        if(isNaN()) return std::numeric_limits<double>::quiet_NaN();
        return (double)num / den;
    }
    
    // 絶対値
    frac abs() const {
        if(isNaN()) return *this;
        return frac(std::abs(num), den);
    }

    // (乗法における) 逆元
    frac inv() const {
        if(isNaN()) return *this;
        return frac(den, num);
    }
    
    // 符号反転
    frac operator-() const {
        if(isNaN()) return *this;
        return frac(-num, den);
    }

    // 四則演算
    frac operator+(const frac& other) const {
        if(isNaN() || other.isNaN()) return frac(1, 0);
        return frac(num * other.den + den * other.num, den * other.den);
    }
    
    frac operator-(const frac& other) const {
        if(isNaN() || other.isNaN()) return frac(1, 0);
        return frac(num * other.den - den * other.num, den * other.den);
    }
    
    frac operator*(const frac& other) const {
        if(isNaN() || other.isNaN()) return frac(1, 0);
        return frac(num * other.num, den * other.den);
    }
    
    frac operator/(const frac& other) const {
        if(isNaN() || other.isNaN() || other.num == 0) return frac(1, 0);
        return frac(num * other.den, den * other.num);
    }
    
    // 比較演算子
    bool operator<(const frac& other) const {
        if(isNaN() || other.isNaN()) return false;
        return num * other.den < den * other.num;
    }
    
    bool operator==(const frac& other) const {
        if(isNaN() && other.isNaN()) return true;
        if(isNaN() || other.isNaN()) return false;
        return num * other.den == den * other.num;
    }
    
    bool operator!=(const frac& other) const { return !(*this == other); }
    bool operator<=(const frac& other) const { return *this < other || *this == other; }
    bool operator>(const frac& other) const { return !(*this <= other); }
    bool operator>=(const frac& other) const { return !(*this < other); }
};

std::ostream& operator<<(std::ostream& os, const frac& f) {
    if(f.state == -1) {
        os << "Unnitialized";
    } else if(f.state == 0){
        os << "NaN";
    } else if(f.den == 1) {
        os << f.num;
    } else {
        os << f.num << "/" << f.den;
    }
    return os;
}

struct FracHash {
    size_t operator()(const frac& f) const {
        if(f.isNaN()) return 0;
        auto h1 = std::hash<long long>{}(f.num);
        auto h2 = std::hash<long long>{}(f.den);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15LLU + (h1 << 12) + (h1 >> 4));
    }
};