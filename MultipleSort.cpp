#include<bits/stdc++.h>
using namespace std;
// Example usage:
// aの降順にソートしてbを合わせる
// vector<pair<int,int>> v{a,b}をsortするのと等価
// multipleSort(less(),a,b);

// multipleSort(greater(),a,b);
// vector index=multipleSort(less(),u,v,w);



// This function sorts multiple vectors based on the first vector
// and returns the indices of the sorted order.
// Note: First argument is a comparison function.
template <typename Compare, typename... Vectors>
vector<size_t> multipleSort(Compare comp = Compare(), Vectors&... vectors) {
    const size_t size = std::get<0>(std::tie(vectors...)).size();
    ((void)std::initializer_list<int>{(vectors.size() == size ? 0 : 
        throw std::invalid_argument("Vectors must have the same size"))...});

    std::vector<size_t> indices(size);
    std::iota(indices.begin(), indices.end(), 0);

    std::sort(indices.begin(), indices.end(), [&](size_t i, size_t j) {
        return comp(std::tie(vectors[i]...), std::tie(vectors[j]...));
    });

    auto reorder = [&](auto& vec) {
        auto temp=vec;
        for (size_t i = 0; i < size; ++i) {
            vec[i] = temp[indices[i]];
        }
    };
    (reorder(vectors), ...);
    return indices;
}