#pragma once

#include <algorithm>
#include <numeric>
#include <utility>
#include <vector>

namespace sora {

// 加法可換群のポテンシャル差を管理するUnion-Find。群演算はO(1)、空間O(N)。
// 参考: https://qiita.com/drken/items/cce6fc5c579051e64fab
template<typename Abel> class PotentialUF{
    int n;
    std::vector<int> par;
    std::vector<int> rank;
    std::vector<Abel> diff_potential;

    inline void init(int n, Abel sum_unity){
        this->n=n;
        par.resize(n);
        std::iota(par.begin(),par.end(),0);
        rank.resize(n,0);
        diff_potential.resize(n,sum_unity);
    }
    
    public:
        // N 要素を初期化。時間・空間 O(N)。
        PotentialUF(int n=1, Abel sum_unity=0){
            init(n,sum_unity);
        }

        // 代表元を返す。償却 O(α(N))。
        inline int root(int x){
            if(par[x]==x) return x;
            int r=root(par[x]);
            diff_potential[x]+=diff_potential[par[x]];
            return par[x]=r;
        }

        // 代表元からのポテンシャル差。償却 O(α(N))。
        inline Abel potential(int x){
            root(x);
            return diff_potential[x];
        }

        // 同じ成分か判定。償却 O(α(N))。
        inline bool same(int x,int y){
            return root(x)==root(y);
        }

        // potential(y) - potential(x) = p を課す。同じ成分なら矛盾の有無によらず false。
        // 制約を追加。償却 O(α(N))。
        inline bool merge(int x,int y,Abel p){
            p+=potential(x);
            p-=potential(y);
            x=root(x);
            y=root(y);
            if(x==y) return false;
            if(rank[x]<rank[y]) std::swap(x,y), p=-p;
            if(rank[x]==rank[y]) ++rank[x];
            par[y]=x;
            diff_potential[y]=p;
            return true;
        }

        // potential(y) - potential(x) を返す。同じ成分であること。償却 O(α(N))。
        inline Abel diff(int x,int y){
            return potential(y) - potential(x);
        }

        // 連結成分を列挙。時間 O(N α(N))、追加空間 O(N)。
        std::vector<std::vector<int>> groups(){
            std::vector<int> leader_buf(n),group_size(n);
            for(int i=0;i<n;i++){
                leader_buf[i]=root(i);
                group_size[leader_buf[i]]++;
            }
            std::vector<std::vector<int>> result(n);
            for(int i=0;i<n;i++) result[i].reserve(group_size[i]);
            for(int i=0;i<n;i++) result[leader_buf[i]].emplace_back(i);
            result.erase(std::remove_if(result.begin(),result.end(),
                                    [&](const std::vector<int> &v){return v.empty(); }),
                         result.end());
            return result;
        }
};

} // namespace sora
