// 場外判定
    auto outof = [&](ull y, ull x){
        return (y >= H || x >= W);
    };
    // 4方向の近傍 右,下,左,上
    const int dy[4] = {0, 1, 0, -1};
    const int dx[4] = {1, 0, -1, 0};

    // 8方向の近傍 右,右下,下,左下,左,左上,上,右上
    const int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    const int dx[8] = {1, 1, 0, -1, -1, -1, 0, 1};


// 1次元と2次元の相互変換
    auto convert_1Dto2D = [&](ull idx){
        ull y = idx/W;
        ull x = idx%W;
        return make_pair(y,x);
    };
    auto convert_2Dto1D = [&](ull y, ull x){
        return y*W+x;
    };