#include<bits/stdc++.h>
using namespace std;
// 行列ライブラリ　四則演算(除法は含まない)を愚直に計算するくらい
template<typename T> struct Matrix{
    private:
        const int _n,_m;
        vector<vector<T>> _data;
    
    public:
        Matrix(int n, int m, T e) : Matrix(vector<vector<T>>(n,vector<T>(m,e))) {}
        explicit Matrix(const vector<vector<T>>& mat) : _data(mat), _n((int)mat.size()), _m((int)mat[0].size()) {}

        template<typename S> inline bool isSameSize(const Matrix<S>& rhs) const {
            return (_n == rhs._n && _m == rhs._m);
        }

        Matrix<T> operator-() const{
            vector data = _data;
            for(auto &v : data){
                for(auto &x : v) x = -x;
            }            
            return Matrix(data);
        }

        Matrix<T> operator+(const Matrix<T>& rhs) const {
            assert(isSameSize(rhs));
            vector data = _data;
            for(int i = 0; i < _n; i++){
                for(int j = 0; j < _m; j++){
                    data[i][j] += rhs._data[i][j];
                }
            }
            return Matrix(data);
        }

        Matrix<T> operator-(const Matrix<T>& rhs) const {
            assert(isSameSize(rhs));
            vector data = _data;
            for(int i = 0; i < _n; i++){
                for(int j = 0; j < _m; j++){
                    data[i][j] -= rhs._data[i][j];
                }
            }
            return Matrix(data);
        }

        Matrix<T> operator*(const T& rhs) const {
            vector data = _data;
            for(auto &v : data){
                for(auto &x : v) x *= rhs;
            }
            return Matrix(data);
        }

        Matrix<T> operator*(const Matrix<T>& rhs) const {
            assert(_m == rhs._n);
            vector<vector<T>> data(_n,vector<T>(rhs._m));
            for(int i = 0; i < _n; i++){
                for(int j = 0; j < rhs._m; j++){
                    T res = 0;
                    for(int k = 0; k < _m; k++){
                        res += _data[i][k]*rhs._data[k][j];
                    }
                    data[i][j] = res;
                }
            }
            return Matrix(data);
        }

        /*Matrix<T> operator^(const int n) const {
            Matrix Mat(_data);
            Matrix rhs(_data);
            for(int i = 1; i < n; i++){
                Mat = Matrix(Mat * rhs);
            }
            return Mat;
        }*/

        void Print(){
            for(auto &v : _data){
                for(auto x = v.begin(); x < v.end(); x++){
                    cout << *x << " \n"[x+1==v.end()];
                }
            }
        }
};
template<typename T> inline Matrix<T> IdentityMatrix(int n){
    vector<vector<T>> data(n,vector<T>(n,0));
    for(int i = 0; i < n; i++){
        data[i][i] = 1;
    }
    return Matrix(data);
}

int main(){
    
}