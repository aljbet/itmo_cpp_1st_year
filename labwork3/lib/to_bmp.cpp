#include <iostream>
#include <fstream>
#include <deque>
#include "to_bmp.h"

void add16(uint16_t a, std::ofstream& file_bmp) {
    file_bmp << (uint8_t) a;
    file_bmp << (uint8_t) (a >> 8);
}

void add32(uint32_t a, std::ofstream& file_bmp) {
    file_bmp << (uint8_t) a;
    file_bmp << (uint8_t) (a >> 8);
    file_bmp << (uint8_t) (a >> 16);
    file_bmp << (uint8_t) (a >> 24);
}

void to_bmp(std::deque<std::deque<uint64_t>>& heap, std::ofstream& file_bmp) {
    BITMAPINFOHEADER bmp_info_header;
    bmp_info_header.biSize = info_header_size;
    bmp_info_header.biWidth = (signed int) heap[0].size();
    bmp_info_header.biHeight = (signed int) heap.size();
    bmp_info_header.biPlanes = 1;
    bmp_info_header.biBitCount = 8;
    bmp_info_header.biCompression = 0x0000;
    bmp_info_header.biSizeImage = bmp_info_header.biWidth * bmp_info_header.biHeight;
    bmp_info_header.biXPelsPerMeter = 100;
    bmp_info_header.biYPelsPerMeter = 100;
    bmp_info_header.biClrUsed = 5;
    bmp_info_header.biClrImportant = 0;

    BITMAPFILEHEADER bmp_file_header;
    bmp_file_header.bfType = 0x4d42;
    bmp_file_header.bfSize = file_header_size + info_header_size + palette_size + bmp_info_header.biSizeImage;
    bmp_file_header.bfReserved1 = 0;
    bmp_file_header.bfReserved2 = 0;
    bmp_file_header.bfoffBits = file_header_size + info_header_size + palette_size;

    add16(bmp_file_header.bfType, file_bmp);
    add32(bmp_file_header.bfSize, file_bmp);
    add16(bmp_file_header.bfReserved1, file_bmp);
    add16(bmp_file_header.bfReserved2, file_bmp);
    add32(bmp_file_header.bfoffBits, file_bmp);

    add32(bmp_info_header.biSize, file_bmp);
    add32(bmp_info_header.biWidth, file_bmp);
    add32(bmp_info_header.biHeight, file_bmp);
    add16(bmp_info_header.biPlanes, file_bmp);
    add16(bmp_info_header.biBitCount, file_bmp);
    add32(bmp_info_header.biCompression, file_bmp);
    add32(bmp_info_header.biSizeImage, file_bmp);
    add32(bmp_info_header.biXPelsPerMeter, file_bmp);
    add32(bmp_info_header.biYPelsPerMeter, file_bmp);
    add32(bmp_info_header.biClrUsed, file_bmp);
    add32(bmp_info_header.biClrImportant, file_bmp);

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            file_bmp << RGBQUAD[i][j];
        }
    }
    for (int i = heap.size() - 1; i >= 0; i--) {
        for (int j = 0; j < heap[i].size(); j++) {
            if (heap[i][j] < 4) {
                uint8_t a = (uint8_t) heap[i][j];
                file_bmp << a;
            } else {
                uint8_t a = (uint8_t) 4;
                file_bmp << a;
            }
        }
        for (size_t j = 0; (j + heap[0].size()) % 4 != 0; j++) {
            uint8_t a = (uint8_t) 0;
            file_bmp << a;
        }
    }
}