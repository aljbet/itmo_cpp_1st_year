#include <iostream>
#include <fstream>
#include <deque>

bool is_stable(std::deque<std::deque<uint64_t>> heap) {
    bool flag = true;
    for (int i = 0; i < heap.size(); i++) {
        for (int j = 0; j < heap[i].size(); j++) {
            if (heap[i][j] > 3) {
                flag = false;
                break;
            }
        }
    }
    return flag;
}

void sandheap(std::deque<std::deque<uint64_t>>& heap) {
    int flag_north = 0;
    int flag_west = 0;
    bool flag_south = false;
    bool flag_east = false;
    std::deque<std::deque<uint64_t>> new_heap;
    for (int i = 0; i < heap.size(); i++) {
        std::deque<uint64_t> temp;
        for (int j = 0; j < heap[i].size(); j++) {
            temp.push_back(0);
        }
        new_heap.push_back(temp);
    }
    for (int i = 0; i < heap.size(); i++) {
        for (int j = 0; j < heap[i].size(); j++) {
            if ((heap)[i][j] >= 4) {
                new_heap[i + flag_north][j + flag_west] += ((heap)[i][j] - 4);
                if (i == 0 && flag_north == 0) {
                    std::deque<uint64_t> temp;
                    for (int m = 0; m < new_heap[i + flag_north].size(); m++) {
                        temp.push_back(0);
                    }
                    new_heap.push_front(temp);
                    flag_north = 1;
                }
                new_heap[i + flag_north - 1][j + flag_west]++;
                if (i == (heap).size() - 1 && !flag_south) {
                    std::deque<uint64_t> temp;
                    for (int m = 0; m < new_heap[i].size(); m++) {
                        temp.push_back(0);
                    }
                    new_heap.push_back(temp);
                    flag_south = true;
                }
                new_heap[i + flag_north + 1][j + flag_west]++;
                if (j == 0 && flag_west == 0) {
                    for (int m = 0; m < new_heap.size(); m++) {
                        new_heap[m].push_front(0);
                    }
                    flag_west = 1;
                }
                new_heap[i + flag_north][j + flag_west - 1]++;
                if (j == (heap)[i].size() - 1 && !flag_east) {
                    for (int m = 0; m < new_heap.size(); m++) {
                        new_heap[m].push_back(0);
                    }
                    flag_east = true;
                }
                new_heap[i + flag_north][j + flag_west + 1]++;
            } else {
                new_heap[i + flag_north][j + flag_west] += (heap)[i][j];
            }
        }
    }
    heap = new_heap;
}