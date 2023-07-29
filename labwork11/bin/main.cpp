#include "../lib/funcs.h"
#include "../lib/xrange.h"
#include "../lib/zip.h"
#include <vector>
#include <list>

int main() {
    std::string str = "text";
    auto x = xrange(str, std::string("textababababababab"), std::string("ab"));
    std::string ans = str;
    for (auto& s: x) {
        std::cout << s;
    }
}