#include "../lib/id3v2.h"

#include <bitset>

int main() {
    std::string filename;
    std::cin >> filename;
    std::ifstream file1(filename, std::ios::binary);
    std::noskipws(file1);
    if (!file1.is_open()) {
        throw std::invalid_argument("Can't open file");
    }
    std::string name;
    for (int i = 0; i < 3; i++) {
        char ch;
        file1 >> ch;
        name.push_back(ch);
    }
    if (name != "ID3") {
        throw std::invalid_argument("Wrong format");
    }
    uint8_t flags;
    file1.seekg(kHeaderVersionSize, std::ios::cur);
    file1 >> flags;
    uint32_t SIZE = size_count(file1); // размер всего файла
    if ((flags >> 4) & 1) {
        SIZE -= kFooterSize;
    }
    if ((flags >> 6) & 1) {
        file1.seekg(kExtendedHeaderSize); //skipping extended header size
        uint8_t number_of_flags;
        file1 >> number_of_flags;
        file1.seekg(number_of_flags); //skipping flags
    }
    while (file1.tellg() < SIZE) {
        FrameHeader f;
        f.frame_id_ = make_string(kFrameIDSize, file1);
        if (f.frame_id_[0] == '\0') {
            break;
        }
        f.size_ = size_count(file1);
        f.make_flags(file1);
        std::cout << f.frame_id_ << '\t';
        if (f.frame_id_ == "TXXX" || f.frame_id_ == "WXXX") {
            xxx current_frame(f);
            current_frame.work_with_file(file1);
        } else if (f.frame_id_ == "ETCO") {
            etco current_frame(f);
            current_frame.work_with_file(file1);
        } else if (f.frame_id_ == "MCDI") {
            mcdi current_frame(f);
            current_frame.work_with_file(file1);
        } else if (f.frame_id_ == "SYTC" || f.frame_id_ == "POSS") {
            sytc_poss current_frame(f);
            current_frame.work_with_file(file1);
        } else if (f.frame_id_ == "UFID" || f.frame_id_ == "PRIV") {
            ufid_priv current_frame(f);
            current_frame.work_with_file(file1);
        } else if (f.frame_id_ == "COMM" || f.frame_id_ == "USLT") {
            comm_uslt current_frame(f);
            current_frame.work_with_file(file1);
        } else if (f.frame_id_ == "EQU2") {
            equ2 current_frame(f);
            current_frame.work_with_file(file1);
        } else if (f.frame_id_ == "RVA2") {
            rva2 current_frame(f);
            current_frame.work_with_file(file1);
        } else if (f.frame_id_ == "LINK") {
            link current_frame(f);
            current_frame.work_with_file(file1);
        } else if (f.frame_id_ == "USER") {
            user current_frame(f);
            current_frame.work_with_file(file1);
        } else if (f.frame_id_ == "SYLT") {
            sylt current_frame(f);
            current_frame.work_with_file(file1);
        } else if (f.frame_id_ == "PCNT") {
            pcnt current_frame(f);
            current_frame.work_with_file(file1);
        } else if (f.frame_id_ == "POPM") {
            popm current_frame(f);
            current_frame.work_with_file(file1);
        } else if (f.frame_id_ == "SEEK") {
            seek current_frame(f);
            current_frame.work_with_file(file1);
        } else if (f.frame_id_ == "RBUF") {
            rbuf current_frame(f);
            current_frame.work_with_file(file1);
        } else if (f.frame_id_ == "OWNE") {
            owne current_frame(f);
            current_frame.work_with_file(file1);
        } else if (f.frame_id_ == "COMR") {
            comr current_frame(f);
            current_frame.work_with_file(file1);
        } else if (f.frame_id_ == "ENCR" || f.frame_id_ == "GRID") {
            encr_grid current_frame(f);
            current_frame.work_with_file(file1);
        } else if (f.frame_id_.front() == 'T') {
            TextFrame current_frame(f);
            current_frame.work_with_file(file1);
        } else if (f.frame_id_.front() == 'W') {
            url current_frame(f);
            current_frame.work_with_file(file1);
        } else {
            std::cout << '\n';
            file1.seekg(f.size_, std::ios::cur);
        }
    }
}