#pragma once

#include <algorithm>
#include <array>
#include <cinttypes>
#include <cmath>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>

const int BASE = 1e9; //the base of the number system
const int LENGTH = 68; //2^2022 - 609 symbols. 9 symbols in each array cell -> 68 cells

struct uint2022_t {
    uint32_t num[LENGTH]{0};
};

static_assert(sizeof(uint2022_t) <= 300, "Size of uint2022_t must be no higher than 300 bytes");

uint2022_t from_uint(uint32_t i);

uint2022_t from_string(const char* buff);

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator>(const uint2022_t& lhs, const uint2022_t& rhs);

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value);