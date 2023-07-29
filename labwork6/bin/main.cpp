#include <iostream>
#include "lib/parser.h"

using namespace omfl;

int main() {
    std::string data = R"(
        key1 = 100500
        key2 = -22
        key3 = +28)";

    const auto root = parse(data);

    std::cout << root.valid() << std::endl;

    std::cout << root.Get("key1").IsInt() << std::endl;
    std::cout << root.Get("key2").IsInt() << std::endl;
    std::cout << root.Get("key3").IsInt() << std::endl;

    std::cout << root.Get("key1").AsInt() << std::endl;
    std::cout << root.Get("key2").AsInt() << std::endl;
    std::cout << root.Get("key3").AsInt() << std::endl;
    return 0;
}
