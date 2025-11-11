/***
  ポテンシャル付きUnionFind
  Abel := 閉じた加法と減法が定義されている型 (多分…)
  refer: https://qiita.com/drken/items/cce6fc5c579051e64fab 
***/
template<typename Abel> class PotentialUF{
    vector<int> par;
    vector<int> rank;
    vector<Abel> diff_potential;

    inline void init(int n, Abel SUM_UNITY){
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
};