#include <functional>
#include <lib/ArgParser.h>

#include <iostream>
#include <numeric>

struct Options {
    bool sum = false;
    bool mult = false;
};

using namespace ArgumentParser;

int main(int argc, char** argv) {
    Options opt;
    std::vector<int> values;
    ArgParser parser("my parser");

    parser.AddIntArgument("N").MultiValue(1).Positional().StoreValues(values);
    parser.AddFlag("sum", "add args").StoreValue(opt.sum);
    parser.AddFlag("mult", "multiply args").StoreValue(opt.mult);
    parser.AddHelp('h', "help", "Program accumulate arguments");

    if (!parser.Parse(argc, argv)) {
        std::cout << "Wrong argument" << std::endl;
        parser.HelpDescription();
        return 1;
    }

    if (parser.Help()) {
        parser.HelpDescription();
        return 0;
    }

    if (opt.sum) {
        std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 0) << std::endl;
    } else if (opt.mult) {
        std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 1, std::multiplies<int>())
                  << std::endl;
    } else {
        std::cout << "No one options had chosen" << std::endl;
        parser.HelpDescription();
        return 1;
    }

    return 0;

}
