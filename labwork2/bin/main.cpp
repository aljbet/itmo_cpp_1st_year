#include <lib/number.h>

#include <iostream>

int main() {
    uint2022_t value1 = from_string("342352132123286444232");
    uint2022_t value2 = from_string("13487645634990");
    uint2022_t v = value1 + value2;
    std::cout << v << std::endl;

    return 0;
}
