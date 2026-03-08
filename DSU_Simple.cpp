/*//--------------------------------------------------------
代表元に何らかの性質を持たせたい、
代表元の決定方法に規則を持たせたいとき用
経路圧縮はするけどUnion by Sizeはしない、再帰のやつ
*///--------------------------------------------------------
class DSU{
    vector<int> par,siz;
    
    public:
        DSU() : par(),siz() {}
        DSU(int n) : par(n,-1),siz(n,1) {}
        
        int root(int v){
            return (~par[v] ? par[v]=root(par[v]) : v);
        }

        // u側に、vを併合
        void merge(int u,int v){
            u=root(u);
            v=root(v);
            if(u!=v){
                par[v]=u;
                siz[u]+=siz[v];
            }
        }
        
        bool same(int u,int v){ return root(u)==root(v); }
};