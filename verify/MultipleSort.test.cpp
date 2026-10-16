// verification-helper: IGNORE
#include <algorithm>
#include <cassert>
#include <functional>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

#include "../utility/MultipleSort.hpp"

// 先頭配列だけでの比較と、元の添字に対応する並べ替えを検証する。
int main() {
    for (bool descending : {false, true}) {
        const std::vector<int> original{2, 1, 2, 0, 1};
        std::vector<int> keys(original), ids(keys.size());
        std::iota(ids.begin(), ids.end(), 0);
        std::vector<std::string> names{"zero", "one", "two", "three", "four"};
        const auto old_names = names;
        auto comp = [descending](int a, int b) { return descending ? a > b : a < b; };
        const auto order = sora::multiple_sort(comp, keys, ids, names);
        assert(std::is_sorted(keys.begin(), keys.end(), comp));
        for (std::size_t i = 0; i < order.size(); ++i) {
            assert(keys[i] == original[order[i]]);
            assert(ids[i] == static_cast<int>(order[i]));
            assert(names[i] == old_names[order[i]]);
        }
    }
    std::vector<int> empty, also_empty;
    assert(sora::multiple_sort(std::less<>{}, empty, also_empty).empty());
    std::vector<int> single{7};
    assert((sora::multiple_sort(std::less<>{}, single) == std::vector<std::size_t>{0}));
    std::vector<int> wrong_size{0, 1};
    bool threw = false;
    try { sora::multiple_sort(std::less<>{}, single, wrong_size); }
    catch (const std::invalid_argument&) { threw = true; }
    assert(threw && single[0] == 7 && (wrong_size == std::vector<int>{0, 1}));
}
