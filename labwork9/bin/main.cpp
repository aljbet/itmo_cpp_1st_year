#include <iostream>
#include "lib/CPoolAllocator.h"
#include <chrono>

int main() {
    std::vector<int> my_vec;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; ++i) {
        my_vec.insert(my_vec.end(), i);
        auto cur = std::chrono::high_resolution_clock::now();
        auto cur_ans = std::chrono::duration_cast<std::chrono::milliseconds>(cur - start).count();
        std::cout << cur_ans << '\n';
    }
}