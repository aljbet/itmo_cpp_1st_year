#include "number.h"

uint2022_t from_uint(uint32_t i) {
    uint2022_t answer;
    answer.num[0] = i % BASE;
    answer.num[1] = i / BASE;
    return answer;
}

uint2022_t from_string(const char* buff) {
    uint2022_t answer;
    std::string temporary_string;
    uint32_t last_cell = (strlen(buff) - 1) / 9;
    uint32_t last_cell_element = strlen(buff) % 9;
    if (last_cell_element == 0) {
        last_cell_element = 9;
    }
    int j = 0;
    while (j < last_cell_element) {
        temporary_string += buff[j];
        j++;
    }
    if (!temporary_string.empty()) {
        answer.num[last_cell] = std::stoi(temporary_string);
        temporary_string = "";
    }
    for (int i = 0; i < last_cell; i++) {
        for (int k = 0; k < 9; k++) {
            temporary_string += buff[j];
            j++;
        }
        answer.num[last_cell - i - 1] = std::stoi(temporary_string);
        temporary_string = "";
    }

    return answer;
}

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t answer;
    for (int i = 0; i < LENGTH; i++) {
        answer.num[i] += (lhs.num[i] + rhs.num[i]) % BASE;
        answer.num[i + 1] += (lhs.num[i] + rhs.num[i]) / BASE;
    }

    return answer;
}

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t answer;
    if (!(rhs > lhs)) {
        for (int i = 0; i < LENGTH; i++) {
            if (lhs.num[i] < rhs.num[i]) {
                answer.num[i] += BASE;
                answer.num[i + 1]--;
            }
            answer.num[i] += (lhs.num[i] - rhs.num[i]);
        }
    }

    return answer;
}

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t answer;
    long long current_answer;
    long long remainder;
    int len1 = LENGTH - 1;
    int len2 = LENGTH - 1;
    while (!lhs.num[len1]) {
        len1--;
    }
    while (!rhs.num[len2]) {
        len2--;
    }
    for (int i = 0; i <= len2; i++) {
        for (int j = 0; j <= len1; j++) {
            current_answer = (long long) lhs.num[j] * (long long) rhs.num[i];
            remainder = current_answer + answer.num[i + j];
            answer.num[i + j] = (current_answer + answer.num[i + j]) % BASE;
            answer.num[i + j + 1] += remainder / BASE;
        }
    }

    return answer;
}

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t answer;
    if ((lhs * rhs) == from_uint(0) or rhs > lhs) {
        return from_uint(0);
    }
    int last_cell = LENGTH - 1;
    while (!lhs.num[last_cell]) {
        last_cell--;
    }
    int k_numbers = 9 * last_cell + std::to_string(lhs.num[last_cell]).size();
    uint2022_t temporary_divisor;
    for (int i = k_numbers - 1; i >= 0; i--) {
        temporary_divisor =
                temporary_divisor * from_uint(10) + from_uint((int) (lhs.num[i / 9] / (pow(10, i % 9))) % 10);
        int left = 0;
        int right = BASE;
        int mid;
        int x;
        while (left <= right) {
            mid = (left + right) / 2;
            if (rhs * from_uint(mid) > temporary_divisor) {
                right = mid - 1;
            } else {
                x = mid;
                left = mid + 1;
            }
        }
        answer.num[i / 9] += x * pow(10, i % 9);
        temporary_divisor = temporary_divisor - rhs * from_uint(x);
    }

    return answer;
}

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs) {
    bool equality = true;
    for (int i = 0; i < LENGTH; i++) {
        if (lhs.num[i] != rhs.num[i]) {
            equality = false;
            break;
        }
    }
    return equality;
}

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs) {
    return !(lhs == rhs);
}

bool operator>(const uint2022_t& lhs, const uint2022_t& rhs) {
    int last_cell_left = LENGTH - 1;
    while (!lhs.num[last_cell_left]) {
        last_cell_left--;
    }
    int last_cell_right = LENGTH - 1;
    while (!rhs.num[last_cell_right]) {
        last_cell_right--;
    }
    if (last_cell_left > last_cell_right) {
        return true;
    } else if (last_cell_left < last_cell_right) {
        return false;
    } else {
        while (last_cell_right >= 0) {
            if (lhs.num[last_cell_left] > rhs.num[last_cell_right]) {
                return true;
            } else if (lhs.num[last_cell_left] < rhs.num[last_cell_right]) {
                return false;
            }
            last_cell_right--;
            last_cell_left--;
        }
    }
}

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value) {
    bool empty_list = true;
    for (int k = 0; k < LENGTH; k++) {
        if (value.num[k] != 0) {
            empty_list = false;
            break;
        }
    }
    if (empty_list) {
        stream << 0 << std::endl;
    } else {
        int i = LENGTH - 1;
        while (!value.num[i]) {
            i--;
        }
        stream.fill('0');
        stream << value.num[i];
        for (int j = i - 1; j >= 0; j--) {
            stream << std::setw(9) << value.num[j];
        }
    }
    return stream;
}
