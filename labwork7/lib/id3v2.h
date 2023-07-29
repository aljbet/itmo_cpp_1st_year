#pragma once

#include <fstream>
#include <iostream>
#include <vector>

const uint8_t kHeaderVersionSize = 2;
const uint8_t kExtendedHeaderSize = 4;
const uint8_t kFooterSize = 10;
const uint8_t kFrameIDSize = 4;
const uint8_t kLanguageStringSize = 3;

std::string make_string(int size, std::ifstream& file);

std::vector<uint8_t> make_vector(uint32_t size, std::ifstream& file);

void make_binary_data(int& i, std::ifstream& file, std::vector<uint8_t> vec, uint32_t size);

void go_till_0(int& i, std::ifstream& file, std::string& str);

void go_till_0(std::ifstream& file, std::string& str);

void go_till_0(std::ifstream& file, std::vector<uint8_t>& vec);

uint32_t size_count(std::ifstream& file);

struct FrameHeader {
    std::string frame_id_; //4 chars
    uint32_t size_{}; //4 7-bit bytes
    uint16_t flags_{};

    void make_flags(std::ifstream& file);
};

class etco {
private:
    FrameHeader header_;
    uint8_t time_stamp_format_{};
    std::vector<uint8_t> type_of_event_;
    std::vector<uint8_t> time_stamp_;

public:
    explicit etco(FrameHeader f) {
        header_ = std::move(f);
    }

    void work_with_file(std::ifstream& file);
};

class seek {
protected:
    FrameHeader header_;
    uint32_t offset_ = 0;

    void make_offset(std::ifstream& file);

public:
    explicit seek(FrameHeader f) {
        header_ = std::move(f);
    }

    virtual void work_with_file(std::ifstream& file);
};

class rbuf : public seek {
private:
    uint32_t buffer_size_{};
    uint8_t info_flag_{};

public:
    explicit rbuf(const FrameHeader& f) : seek(f) {}

    void work_with_file(std::ifstream& file) override;
};

class owne {
protected:
    FrameHeader header_;
    uint8_t encoding_{};
    std::string price_;
    std::string date_;
    std::string seller_;

public:
    explicit owne(FrameHeader f) {
        header_ = std::move(f);
    }

    virtual void work_with_file(std::ifstream& file);
};

class comr : public owne {
private:
    std::string contact_url_;
    uint8_t received_as_{};
    std::string description_;
    std::string picture_;
    std::vector<uint8_t> binary_data_;

public:
    explicit comr(const FrameHeader& f) : owne(f) {}

    void work_with_file(std::ifstream& file) override;
};

class mcdi {
protected:
    FrameHeader header_;
    std::vector<uint8_t> binary_data_;

public:
    explicit mcdi(FrameHeader f) {
        header_ = std::move(f);
    }

    virtual void work_with_file(std::ifstream& file);
};

class ufid_priv : public mcdi {
protected:
    std::string owner_id_;

    void make_owner_id(int& i, std::ifstream& file);

public:
    explicit ufid_priv(const FrameHeader& f) : mcdi(f) {}

    void work_with_file(std::ifstream& file) override;
};

class encr_grid : public ufid_priv {
protected:
    uint8_t symbol_{};

public:
    explicit encr_grid(const FrameHeader& f) : ufid_priv(f) {}

    void work_with_file(std::ifstream& file) override;
};

class sytc_poss : public mcdi {
private:
    uint8_t time_stamp_format_{};

public:
    explicit sytc_poss(const FrameHeader& f) : mcdi(f) {}

    void work_with_file(std::ifstream& file) override;
};

class pcnt {
protected:
    FrameHeader header_;
    std::vector<uint8_t> counter_;

public:
    explicit pcnt(FrameHeader f) {
        header_ = std::move(f);
    }

    virtual void work_with_file(std::ifstream& file);
};

class popm : public pcnt {
private:
    std::string email_;
    uint8_t rating_ = 0;

public:
    explicit popm(const FrameHeader& f) : pcnt(f) {}

    void make_email(int& i, std::ifstream& file);

    void work_with_file(std::ifstream& file) override;
};

class url {
protected:
    std::vector<std::string> info_;
    FrameHeader header_;

    void make_info(int& i, std::ifstream& file);

public:
    explicit url(FrameHeader f) {
        header_ = std::move(f);
    }

    virtual void work_with_file(std::ifstream& file);
};

class rva2 : public url {
private:
    uint8_t type_of_channel_{};
    uint16_t volume_adjustment_{};
    uint8_t bits_representing_peak_{};
    std::vector<uint8_t> peak_volume_;

public:
    explicit rva2(const FrameHeader& f) : url(f) {}

    void work_with_file(std::ifstream& file) override;
};

class equ2 : public url {
private:
    uint8_t interpolation_method_{};
    uint16_t frequency_{};
    uint16_t volume_adjustment_{};

public:
    explicit equ2(const FrameHeader& f) : url(f) {}

    void work_with_file(std::ifstream& file) override;
};

class link : public url {
private:
    uint32_t cur_frame_id_{};
    std::string url_link_;

public:
    explicit link(const FrameHeader& f) : url(f) {}

    void work_with_file(std::ifstream& file) override;
};

class TextFrame : public url {
protected:
    uint8_t encoding_{};

public:
    explicit TextFrame(const FrameHeader& f) : url(f) {}

    void work_with_file(std::ifstream& file) override;
};

class user : public TextFrame {
protected:
    std::string language_;

public:
    explicit user(const FrameHeader& f) : TextFrame(f) {}

    void work_with_file(std::ifstream& file) override;
};

class sylt : public user {
private:
    uint8_t time_stamp_format_ = 0;
    uint8_t content_type_ = 0;

    void make_info(std::ifstream& file);

public:
    explicit sylt(const FrameHeader& f) : user(f) {}

    void work_with_file(std::ifstream& file) override;
};

class xxx : public TextFrame {
protected:
    std::string description_;

    void make_description(int& i, std::ifstream& file);

public:
    explicit xxx(const FrameHeader& f) : TextFrame(f) {}

    void work_with_file(std::ifstream& file) override;
};

class comm_uslt : public xxx {
private:
    std::string language_;

public:
    explicit comm_uslt(const FrameHeader& f) : xxx(f) {}

    void work_with_file(std::ifstream& file) override;
};