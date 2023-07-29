#ifndef LABWORK_3_ALJBET_TO_BMP_H
#define LABWORK_3_ALJBET_TO_BMP_H

#endif //LABWORK_3_ALJBET_TO_BMP_H

const int file_header_size = 14;
const int info_header_size = 40;
const int palette_size = 20;

struct BITMAPFILEHEADER {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfoffBits;
};

struct BITMAPINFOHEADER {
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

static uint8_t RGBQUAD[5][4]{
        {255, 255, 255, 0},
        {0,   255, 0,   0},
        {255, 0,   255, 0},
        {0, 255, 255,   0},
        {0, 0, 0, 0},
};

void to_bmp(std::deque<std::deque<uint64_t>> &heap, std::ofstream &file_bmp);
