/***
  ポテンシャル付きUnionFind
  Abel := 閉じた加法と減法が定義されている型 (多分…)
  refer: https://qiita.com/drken/items/cce6fc5c579051e64fab 
***/
template<typename Abel> class PotentialUF{
    int n;
    vector<int> par;
    vector<int> rank;
    vector<Abel> diff_potential;

    inline void init(int n, Abel SUM_UNITY){
        this->n=n;
        par.resize(n);
        iota(par.begin(),par.end(),0);
        rank.resize(n,0);
        diff_potential.resize(n,SUM_UNITY);
    }
    
    public:
        PotentialUF(int n=1, Abel SUM_UNITY=0){
            init(n,SUM_UNITY);
        }

        inline int root(int x){
            if(par[x]==x) return x;
            int r=root(par[x]);
            diff_potential[x]+=diff_potential[par[x]];
            return par[x]=r;
        }

        inline Abel potential(int x){
            root(x);
            return diff_potential[x];
        }

        inline bool same(int x,int y){
            return root(x)==root(y);
        }

        inline bool merge(int x,int y,Abel p){
            p+=potential(x);
            p-=potential(y);
            x=root(x);
            y=root(y);
            if(x==y) return false;
            if(rank[x]<rank[y]) swap(x,y), p=-p;
            if(rank[x]==rank[y]) ++rank[x];
            par[y]=x;
            diff_potential[y]=p;
            return true;
        }

        inline Abel diff(int x,int y){
            return potential(y) - potential(x);
        }

        // refer to DSU on ACL
        vector<vector<int>> groups(){
            vector<int> leader_buf(n),group_size(n);
            for(int i=0;i<n;i++){
                leader_buf[i]=root(i);
                group_size[leader_buf[i]]++;
            }
            vector<vector<int>> result(n);
            for(int i=0;i<n;i++) result[i].reserve(group_size[i]);
            for(int i=0;i<n;i++) result[leader_buf[i]].emplace_back(i);
            result.erase(remove_if(result.begin(),result.end(),
                                    [&](const vector<int> &v){return v.empty(); }),
                         result.end());
            return result;
        }
};