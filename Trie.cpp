#include<bits/stdc++.h>
using namespace std;
/*//--------------------------------------------------------
たたき台　改造して使う場面が多いと思う
*///--------------------------------------------------------



struct Node{
    int pass=0; // このノードを通る文字列数
    int end=0;  // このノードで終わる文字列数
    array<Node*,26> nxt{};
    Node()=default;
};

struct Trie{
    Node* root;
    Trie() : root(new Node()) {}

    void insert(const string &s){
        Node *cur=root;
        cur->pass++;
        for(char c:s){
            int d=c-'a';
            if(!cur->nxt[d]) cur->nxt[d]=new Node();
            cur=cur->nxt[d];
            cur->pass++;
        }
        cur->end++;
    }


    // 文字列sが何回挿入されたか O(|S|)
    int countWord(const string &s) const {
        const Node* cur=root;
        for(char c:s){
            int d=c-'a';
            if(!cur->nxt[d]) return 0;
            cur=cur->nxt[d];
        }
        return cur->end;
    }

    // 文字列pをprefixとする文字列の個数 O(|p|)
    int countPrefix(const string &p) const {
        const Node* cur=root;
        for(char c:p){
            int d=c-'a';
            if(!cur->nxt[d]) return 0;
            cur=cur->nxt[d];
        }
        return cur->pass;
    }
};