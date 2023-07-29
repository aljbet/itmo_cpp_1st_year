#include "lib/CCircularBuffer.h"

int main() {
    std::initializer_list<int> a = {2, 3, 9, 2022};
    std::cout << *a.begin() << ' ' << *(a.end() - 1) << ' ' << a.end() - a.begin() << '\n';
    CCircularBuffer<int> buff({1, 2});
    bool b = buff.empty();
    std::cout << b;
}