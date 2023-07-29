#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdint.h>


struct file_data {
    uint64_t k_lines = 1, k_words = 0, k_bytes = 0, k_chars = 0;
};

struct options {
    bool lines=false, words=false, bytes=false, chars=false;
};

void word_count(std::ifstream &file1, file_data * file_data1) {
    char c;
    bool word_flag=false;
    file1.seekg(0, std::ios::end);
    file_data1->k_bytes=file1.tellg();
    file1.seekg(0);
    while (file1.get(c)) {
        if (std::isprint(c)) {
            file_data1->k_chars++;
        }
        if (c=='\n') {
            file_data1->k_lines++;
            if (word_flag){
                file_data1->k_words++;
                word_flag=false;
            }
        }
        if (c==' ' and word_flag) {
            file_data1->k_words++;
            word_flag=false;
        }
        if (c!=' ' and c!='\n') {
            word_flag = true;
        }
    }
    if (std::isprint(c)) {
        file_data1->k_words++;
    }
}

int main(int argc, char** argv) {
    std::ifstream file1;
    std::vector<std::string> operations;
    std::vector<std::string> filenames;
    for (int i=1; i<argc;i++) {
        if (argv[i][0] == '-') {
            operations.emplace_back(argv[i]);
        } else {
            filenames.emplace_back(argv[i]);
        }
    }
    options options_needed;
    if (operations.size()==0) {
        options_needed.lines=true;
        options_needed.words=true;
        options_needed.bytes=true;
        options_needed.chars=true;
    } else {
        for (int j=0; j<operations.size(); j++) {
            std::string current_operation;
            current_operation = operations[j];
            if (current_operation.find("--")) {
                if (current_operation=="--lines") {
                    options_needed.lines=true;
                }
                if (current_operation=="--words") {
                    options_needed.words=true;
                }
                if (current_operation=="--bytes") {
                    options_needed.bytes=true;
                }
                if (current_operation=="--chars") {
                    options_needed.chars=true;
                }
            }
            if(current_operation[0]=='-') {
                for (int k=1; k<current_operation.size(); k++) {
                    if (current_operation[k]=='l') {
                        options_needed.lines=true;
                    }
                    if (current_operation[k]=='w') {
                        options_needed.words=true;
                    }
                    if (current_operation[k]=='c') {
                        options_needed.bytes=true;
                    }
                    if (current_operation[k]=='m') {
                        options_needed.chars=true;
                    }
                }
            }
        }
    }
    for (int i = 0; i < filenames.size(); i++) {
        file1.open(filenames[i]);
        if (!file1.is_open()) {
            std::cout<<"file not found ";
            continue;
        }
        else{
            file_data file_data1;
            word_count(file1, &file_data1);
            if (options_needed.lines) {
                std::cout<<file_data1.k_lines<<" ";
            }
            if (options_needed.words) {
                std::cout<<file_data1.k_words<<" ";
            }
            if (options_needed.bytes) {
                std::cout<<file_data1.k_bytes<<" ";
            }
            if (options_needed.chars) {
                std::cout<<file_data1.k_chars<<" ";
            }
        }
        std::cout<<filenames[i]<<std::endl;
        file1.close();
    }
    return 0;
}