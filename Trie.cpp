struct Trie {
    struct Node {
        int pass = 0; // このノードをprefixとする文字列数（重複込み）
        int end  = 0; // このノードで終わる文字列数
        array<int, 26> nxt;

        Node() {
            nxt.fill(-1);
        }
    };

    vector<Node> t;

    Trie() : t(1) {}

    // Trie内の文字列数（重複込み） O(1)
    int size() const {
        return t[0].pass;
    }

    bool empty() const {
        return size() == 0;
    }

    // sを1個追加 O(|s|)
    void insert(const string& s) {
        int v = 0;
        t[v].pass++;

        for (char c : s) {
            int d = c - 'a';

            if (t[v].nxt[d] == -1) {
                t[v].nxt[d] = t.size();
                t.emplace_back();
            }

            v = t[v].nxt[d];
            t[v].pass++;
        }

        t[v].end++;
    }

    // sが何個存在するか O(|s|)
    int countWord(const string& s) const {
        int v = 0;

        for (char c : s) {
            int d = c - 'a';

            if (t[v].nxt[d] == -1)
                return 0;

            v = t[v].nxt[d];
        }

        return t[v].end;
    }

    // prefixがpである文字列数 O(|p|)
    int countPrefix(const string& p) const {
        int v = 0;

        for (char c : p) {
            int d = c - 'a';

            if (t[v].nxt[d] == -1)
                return 0;

            v = t[v].nxt[d];
        }

        return t[v].pass;
    }

    // sを1個削除
    // 存在しなければfalse
    // O(|s|)
    bool erase(const string& s) {
        if (countWord(s) == 0)
            return false;

        int v = 0;
        t[v].pass--;

        for (char c : s) {
            int d = c - 'a';
            v = t[v].nxt[d];
            t[v].pass--;
        }

        t[v].end--;
        return true;
    }

    // sをすべて削除
    // 削除した個数を返す
    // O(|s|)
    int eraseAll(const string& s) {
        int cnt = countWord(s);

        if (cnt == 0)
            return 0;

        int v = 0;
        t[v].pass -= cnt;

        for (char c : s) {
            int d = c - 'a';
            v = t[v].nxt[d];
            t[v].pass -= cnt;
        }

        t[v].end -= cnt;
        return cnt;
    }

    // sより辞書順で小さい文字列の個数（重複込み）
    // O(26|s|) = O(|s|)
    int countLess(const string& s) const {
        int v = 0;
        int res = 0;

        for (char c : s) {
            // 現在のprefixそのものが文字列なら、
            // sより短いので辞書順で前
            res += t[v].end;

            int d = c - 'a';

            // 次の文字がcより小さいものはすべてsより前
            for (int x = 0; x < d; x++) {
                int u = t[v].nxt[x];
                if (u != -1)
                    res += t[u].pass;
            }

            if (t[v].nxt[d] == -1)
                return res;

            v = t[v].nxt[d];
        }

        // s自身は含めない
        return res;
    }
    
    // s以下の文字列数
    // O(|s|)
    int countLessEqual(const string& s) const {
        return countLess(s) + countWord(s);
    }

    // 辞書順でk番目の文字列（0-indexed、重複込み）
    // 存在しなければnullopt
    // O(26 * 答えの長さ)
    optional<string> kth(int k) const {
        if (k < 0 || k >= size())
            return nullopt;

        int v = 0;
        string res;

        while (true) {
            // 現在のprefixそのものが先に並ぶ
            if (k < t[v].end)
                return res;

            k -= t[v].end;

            bool found = false;

            for (int d = 0; d < 26; d++) {
                int u = t[v].nxt[d];

                if (u == -1)
                    continue;

                if (k < t[u].pass) {
                    res += char('a' + d);
                    v = u;
                    found = true;
                    break;
                }

                k -= t[u].pass;
            }

            // k<size()なら本来ここには来ない
            if (!found)
                return nullopt;
        }
    }

    // s以上で辞書順最小の文字列
    // O(|s| + 26 * 答えの長さ)
    optional<string> lowerBound(const string& s) const {
        int k = countLess(s);
        return kth(k);
    }

    // sより辞書順で大きい最小の文字列
    // O(|s| + 26 * 答えの長さ)
    optional<string> upperBound(const string& s) const {
        int k = countLessEqual(s);
        return kth(k);
    }
};