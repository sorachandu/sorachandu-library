#pragma once

#include <cassert>
#include <ostream>
#include <utility>
#include <vector>

namespace sora {

// ei1333さんのライブラリを移植。出典: https://ei1333.github.io/luzhiled/snippets/math/matrix.html
// 行列演算。T の四則演算を O(1) とする。入力はコピーして保持する。
// 空行列は 0×0 として扱う。高さ0で幅が正の行列は表現できない。
template<class T>
struct Matrix {
    std::vector<std::vector<T>> a;
    inline static const T unit=1;

    // 0×0 行列。時間・空間 O(1)。
    Matrix() {}

    // N×M の零行列。時間・空間 O(NM)。N,M >= 0。
    Matrix(int n, int m) : a(n, std::vector<T>(m, 0)) {}

    // N×N の零行列。時間・空間 O(N^2)。
    Matrix(int n) : a(n, std::vector<T>(n, 0)) {};

    // 長方形の配列をコピーする。時間・空間 O(NM)。
    Matrix(const std::vector<std::vector<T>>& v) : a(v) {};

    // 高さを返す。時間 O(1)。
    int height() const {
        return (a.size());
    }

    // 幅を返す。時間 O(1)。
    int width() const {
        return a.empty() ? 0 : static_cast<int>(a[0].size());
    }

    // 行への参照を返す。時間 O(1)。
    inline const std::vector<T> &operator[](int k) const {
        return (a.at(k));
    }

    // 行への参照を返す。時間 O(1)。
    inline std::vector<T> &operator[](int k) {
        return (a.at(k));
    }

    // 単位行列。時間・空間 O(N^2)。
    static Matrix identity(int n) {
        Matrix mat(n);
        for(int i = 0; i < n; i++) mat[i][i] = unit;
        return (mat);
    }

    // 時間 O(NM)、追加空間 O(1)。
    Matrix &operator+=(const Matrix &other) {
        int n = height(), m = width();
        assert(n == other.height() && m == other.width());
        for(int i = 0; i < n; i++)
            for(int j = 0; j < m; j++)
               (*this)[i][j] += other[i][j];
        return (*this);
    }

    // 時間 O(NM)、追加空間 O(1)。
    Matrix &operator-=(const Matrix &other) {
        int n = height(), m = width();
        assert(n == other.height() && m == other.width());
        for(int i = 0; i < n; i++)
            for(int j = 0; j < m; j++)
                (*this)[i][j] -= other[i][j];
        return (*this);
    }

    // N×P と P×M の積。時間 O(NPM)、追加空間 O(NM)。
    Matrix &operator*=(const Matrix &other) {
        int n = height(), m = other.width(), p = width();
        assert(p == other.height());
        std::vector<std::vector<T>> product(n, std::vector<T>(m, 0));
        for(int i = 0; i < n; i++)
            for(int j = 0; j < m; j++)
            for(int k = 0; k < p; k++)
                product[i][j] = (product[i][j] + ((*this)[i][k] * other[k][j]));
        a.swap(product);
        return (*this);
    }

    // 正方行列の非負整数乗。時間 O(N^3 log(k+1))、追加空間 O(N^2)。
    Matrix &operator^=(long long k) {
        assert(k >= 0 && height() == width());
        Matrix other = Matrix::identity(height());
        while(k > 0) {
            if(k & 1) other *= *this;
            k >>= 1LL;
            if (k) *this *= *this;
        }
        a.swap(other.a);
        return (*this);
    }

    // 時間・追加空間 O(NM)。
    Matrix operator+(const Matrix &other) const {
        return (Matrix(*this) += other);
    }

    // 時間・追加空間 O(NM)。
    Matrix operator-(const Matrix &other) const {
        return (Matrix(*this) -= other);
    }

    // N×P と P×M の積。時間 O(NPM)、追加空間 O(NM + NP)。
    Matrix operator*(const Matrix &other) const {
        return (Matrix(*this) *= other);
    }

    // 正方行列の非負整数乗。時間 O(N^3 log(k+1))、追加空間 O(N^2)。
    Matrix operator^(const long long k) const {
        return (Matrix(*this) ^= k);
    }

    // 行列を出力する。時間 O(NM)。
    friend std::ostream &operator<<(std::ostream &os, Matrix &p) {
        int n = p.height(), m = p.width();
        for(int i = 0; i < n; i++) {
            os << "[";
            for(int j = 0; j < m; j++) {
                os << p[i][j] << (j + 1 == m ? "]\n" : ",");
            }
        }
        return (os);
    }

    // 行列を出力する。時間 O(NM)。
    friend std::ostream &operator<<(std::ostream &os, Matrix &&p) {
        int n = p.height(), m = p.width();
        for(int i = 0; i < n; i++) {
            os << "[";
            for(int j = 0; j < m; j++) {
                os << p[i][j] << (j + 1 == m ? "]\n" : ",");
            }
        }
        return (os);
    }


    // 体上の除算を要求する（整数除算は不可）。入力は変更しない。時間 O(N^3)、空間 O(N^2)。
    T determinant() const {
        Matrix other(*this);
        assert(width() == height());
        T ret = 1;
        for(int i = 0; i < width(); i++) {
            int idx = -1;
            for(int j = i; j < width(); j++) {
                if(other[j][i] != 0) idx = j;
            }
            if(idx == -1) return (0);
            if(i != idx) {
                ret *= -1;
                std::swap(other[i], other[idx]);
            }
            ret *= other[i][i];
            T vv = other[i][i];
            for(int j = 0; j < width(); j++) {
                other[i][j] /= vv;
            }
            for(int j = i + 1; j < width(); j++) {
                T factor = other[j][i];
                for(int k = 0; k < width(); k++) {
                    other[j][k] -= other[i][k] * factor;
                }
            }
        }
        return (ret);
    }
};

} // namespace sora
