#pragma once

#include <iostream>
#include <stack>
#include <vector>
#include <cmath>

template <typename T>
struct Pool {
    std::stack<T*> block;

    Pool(size_t k_chunks, size_t size_of_one_chunk) {
        for (int i = 0; i < k_chunks; ++i) {
            T* a = new T[size_of_one_chunk];
            block.push(a);
        }
    }

    ~Pool() {
        while (!block.empty()) {
            delete[] block.top();
            block.pop();
        }
    }
};

template <typename T, size_t chunks_in_each_pool = 100, size_t kPools = 10>
class CPoolAllocator {
public:
    using value_type = T;
    using pointer = T*;
    template <typename U>
    struct rebind {
        using other = CPoolAllocator<U, chunks_in_each_pool, kPools>;
    };

    CPoolAllocator() {
        for (int i = 0; i < kPools; ++i) {
            pools_.template emplace_back(Pool<value_type>(chunks_in_each_pool, pow(2, i)));
        }
    }

    ~CPoolAllocator() {
        pools_.clear();
    }

    pointer allocate(size_t n) {
        Pool<value_type> current_pool = pools_[std::ceil(log2(n))];
        if (current_pool.block.empty()) {
            throw std::bad_alloc();
        }
        pointer ans = current_pool.block.top();
        if (ans == nullptr) {
            throw std::bad_alloc();
        }
        current_pool.block.pop();
        return ans;
    }

    void deallocate(pointer p, size_t n) {
        Pool<value_type> current_pool = pools_[std::ceil(log2(n))];
        current_pool.block.push(p);
    }

private:
    std::vector<Pool<value_type>> pools_;
};