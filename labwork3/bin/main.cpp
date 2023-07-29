#include "../lib/sand_heap.h"
#include "../lib/to_bmp.h"

#include <iostream>
#include <fstream>
#include <cstring>

int main(int argc, char** argv) {
    uint16_t length;
    uint16_t width;
    char* filename_from;
    char* directory;
    uint16_t max_iter;
    uint16_t frequency;
    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--lines") == 0) {
            length = std::strtol(argv[i + 1], nullptr, 10);
        } else if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--width") == 0) {
            width = std::strtol(argv[i + 1], nullptr, 10);
        } else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) {
            filename_from = argv[i + 1];
        } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
            directory = argv[i + 1];
        } else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--max-iter") == 0) {
            max_iter = std::strtol(argv[i + 1], nullptr, 10);
        } else {
            frequency = std::strtol(argv[i + 1], nullptr, 10);
        }
    }
    std::deque<std::deque<uint64_t>> heap;
    for (int i = 0; i < length; i++) {
        std::deque<uint64_t> temp;
        for (int j = 0; j < width; j++) {
            temp.push_back(0);
        }
        heap.push_back(temp);
    }
    int x;
    int y;
    uint64_t k;
    std::ifstream file1;
    file1.open(filename_from);
    while (file1 >> x) {
        file1 >> y >> k;
        heap[y - 1][x - 1] = k;
    }
    int k_iter = 0;
    int n = 0;
    while (!is_stable(heap) && k_iter <= max_iter) {
        sandheap(heap);
        k_iter++;
        if (frequency && k_iter % frequency == 0) {
            n++;
            std::string filename_to = directory + std::to_string(n) + ".bmp";
            std::ofstream file_bmp;
            file_bmp.open(filename_to);
            to_bmp(heap, file_bmp);
            file_bmp.close();
        }
    }
    if (frequency == 0) {
        n++;
        std::string filename_to = directory + std::to_string(n) + ".bmp";
        std::ofstream file_bmp;
        file_bmp.open(filename_to);
        to_bmp(heap, file_bmp);
        file_bmp.close();
    }
    return 0;
}