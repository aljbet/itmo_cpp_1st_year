#include "id3v2.h"

std::string make_string(int size, std::ifstream& file) {
    std::string ans;
    char ch;
    for (int i = 0; i < size; i++) {
        file >> ch;
        ans.push_back(ch);
    }
    return ans;
}

std::vector<uint8_t> make_vector(uint32_t size, std::ifstream& file) {
    std::vector<uint8_t> ans;
    uint8_t ch;
    for (int i = 0; i < size; i++) {
        file >> ch;
        ans.push_back(ch);
    }
    return ans;
}

void make_binary_data(int& i, std::ifstream& file, std::vector<uint8_t> vec, uint32_t size) {
    char ch;
    while (i < size - 1) {
        file >> ch;
        vec.push_back(ch);
        i++;
    }
}

void go_till_0(int& i, std::ifstream& file, std::string& str) {
    char ch;
    while (true) {
        file >> ch;
        if (ch == '\0') {
            i++;
            break;
        }
        str.push_back(ch);
        i++;
    }
}

void go_till_0(std::ifstream& file, std::string& str) {
    char ch;
    while (true) {
        file >> ch;
        if (ch == '\0') {
            break;
        }
        str.push_back(ch);
    }
}

void go_till_0(std::ifstream& file, std::vector<uint8_t>& vec) {
    uint8_t ch;
    while (true) {
        file >> ch;
        if (ch == '\0') {
            break;
        }
        vec.push_back(ch);
    }
}

uint32_t size_count(std::ifstream& file) {
    uint32_t ans = 0;
    uint8_t ch;
    for (int i = 0; i < 4; i++) {
        file >> ch;
        ans |= (ch << (7 * (3 - i)));
    }
    return ans;
}

void FrameHeader::make_flags(std::ifstream& file) {
    uint8_t ch1, ch2;
    file >> ch1 >> ch2;
    flags_ = ((ch1 << 8) | ch2);
}

void etco::work_with_file(std::ifstream& file) {
    file >> time_stamp_format_;
    for (int i = 1; i < header_.size_; i += 2) {
        file >> type_of_event_[i / 2];
        file >> time_stamp_[i / 2];
        std::cout << type_of_event_[i / 2] << ' ' << time_stamp_[i / 2];
    }
    std::cout << '\n';
}

void seek::work_with_file(std::ifstream& file) {
    make_offset(file);
    std::cout << offset_ << '\n';
}

void seek::make_offset(std::ifstream& file) {
    char ch;
    for (int i = 0; i < 4; i++) {
        file >> ch;
        offset_ |= (ch << 8 * (3 - i));
    }
}

void rbuf::work_with_file(std::ifstream& file) {
    uint8_t ch1, ch2, ch3;
    file >> ch1 >> ch2 >> ch3;
    buffer_size_ = (ch1 << 16) | (ch2 << 8) | ch3;
    file >> info_flag_;
    make_offset(file);
}

void owne::work_with_file(std::ifstream& file) {
    file >> encoding_;
    int i = 1;
    go_till_0(i, file, price_);
    go_till_0(i, file, date_);
    go_till_0(i, file, seller_);
    std::cout << price_ << ' ' << date_ << ' ' << seller_ << '\n';
}

void comr::work_with_file(std::ifstream& file) {
    file >> encoding_;
    int i = 1;
    go_till_0(i, file, price_);
    go_till_0(i, file, date_);
    go_till_0(i, file, contact_url_);
    file >> received_as_;
    go_till_0(i, file, seller_);
    go_till_0(i, file, description_);
    go_till_0(i, file, picture_);

    std::cout << price_ << ' ' << date_ << ' ' << contact_url_ << ' ' << seller_ << ' ' << description_ << ' '
              << picture_ << '\n';
}

void mcdi::work_with_file(std::ifstream& file) {
    int i = 0;
    make_binary_data(i, file, binary_data_, header_.size_);
    for (uint8_t a: binary_data_) {
        std::cout << a << ' ';
    }
    std::cout << '\n';
}

void ufid_priv::make_owner_id(int& i, std::ifstream& file) {
    go_till_0(i, file, owner_id_);
}

void ufid_priv::work_with_file(std::ifstream& file) {
    int i = 0;
    make_owner_id(i, file);
    std::cout << owner_id_ << ' ';
    make_binary_data(i, file, binary_data_, header_.size_);
    for (uint8_t a: binary_data_) {
        std::cout << a << ' ';
    }
    std::cout << '\n';
}

void encr_grid::work_with_file(std::ifstream& file) {
    int i = 0;
    make_owner_id(i, file);
    file >> symbol_;
    std::cout << owner_id_ << ' ';
    make_binary_data(i, file, binary_data_, header_.size_);
    for (uint8_t a: binary_data_) {
        std::cout << a << ' ';
    }
    std::cout << '\n';
}

void sytc_poss::work_with_file(std::ifstream& file) {
    int i = 1;
    file >> time_stamp_format_;
    make_binary_data(i, file, binary_data_, header_.size_);
    for (uint8_t a: binary_data_) {
        std::cout << a << ' ';
    }
    std::cout << '\n';
}

void pcnt::work_with_file(std::ifstream& file) {
    counter_ = make_vector(header_.size_, file);
    for (uint8_t a: counter_) {
        std::cout << a << ' ';
    }
    std::cout << '\n';
}

void popm::make_email(int& i, std::ifstream& file) {
    go_till_0(i, file, email_);
}

void popm::work_with_file(std::ifstream& file) {
    int i = 0;
    make_email(i, file);
    std::cout << email_ << ' ';
    file >> rating_;
    i++;
    std::cout << rating_ << ' ';
    counter_ = make_vector(header_.size_ - i, file);
    for (uint8_t a: counter_) {
        std::cout << a << ' ';
    }
    std::cout << '\n';
}

void url::make_info(int& i, std::ifstream& file) {
    char ch;
    std::string cur_str;
    while (i < header_.size_ - 1) {
        file >> ch;
        if (ch == '\0') {
            info_.push_back(cur_str);
            cur_str.clear();
        } else {
            cur_str.push_back(ch);
        }
        i++;
    }
    info_.push_back(cur_str);
}

void url::work_with_file(std::ifstream& file) {
    int i = 0;
    header_.size_++;
    make_info(i, file);
    for (const std::string& str: info_) {
        std::cout << str << ' ';
    }
    std::cout << '\n';
}

void rva2::work_with_file(std::ifstream& file) {
    file >> type_of_channel_;
    uint8_t ch1, ch2;
    file >> ch1 >> ch2;
    volume_adjustment_ = (ch1 << 8) | ch2;
    file >> bits_representing_peak_;
    go_till_0(file, peak_volume_);
}

void equ2::work_with_file(std::ifstream& file) {
    file >> interpolation_method_;
    go_till_0(file, info_.front());
    uint8_t ch1, ch2;
    file >> ch1 >> ch2;
    frequency_ = (ch1 << 8) | ch2;
    file >> ch1 >> ch2;
    volume_adjustment_ = (ch1 << 8) | ch2;
    std::cout << frequency_ << ' ' << volume_adjustment_;
}

void link::work_with_file(std::ifstream& file) {
    char ch;
    for (int i = 0; i < 4; i++) {
        file >> ch;
        cur_frame_id_ |= (ch << 8 * (3 - i));
    }
    int i = 4;
    go_till_0(i, file, url_link_);
    make_info(i, file);
    std::cout << cur_frame_id_ << '\t' << url_link_ << '\t';
    for (std::string& a: info_) {
        std::cout << a << ' ';
    }
    std::cout << '\n';
}

void TextFrame::work_with_file(std::ifstream& file) {
    int i = 0;
    file >> encoding_;
    make_info(i, file);
    for (std::string& str: info_) {
        std::cout << str << ' ';
    }
    std::cout << '\n';
}

void user::work_with_file(std::ifstream& file) {
    int i = 3;
    file >> encoding_;
    language_ = make_string(i, file);
    make_info(i, file);
    for (const std::string& str: info_) {
        std::cout << str << ' ';
    }
    std::cout << '\n';
}

void sylt::make_info(std::ifstream& file) {
    go_till_0(file, info_.front());
}

void sylt::work_with_file(std::ifstream& file) {
    file >> encoding_;
    language_ = make_string(kLanguageStringSize, file);
    file >> time_stamp_format_;
    file >> content_type_;
    make_info(file);
    for (const std::string& str: info_) {
        std::cout << str << ' ';
    }
    std::cout << '\n';
}

void xxx::make_description(int& i, std::ifstream& file) {
    go_till_0(i, file, description_);
}

void xxx::work_with_file(std::ifstream& file) {
    file >> encoding_;
    int i = 0;
    make_description(i, file);
    make_info(i, file);
    for (const std::string& str: info_) {
        std::cout << str << ' ';
    }
    std::cout << '\n';
}

void comm_uslt::work_with_file(std::ifstream& file) {
    int i = 3;
    file >> encoding_;
    language_ = make_string(i, file);
    make_description(i, file);
    make_info(i, file);
    for (const std::string& str: info_) {
        std::cout << str << ' ';
    }
    std::cout << '\n';
}