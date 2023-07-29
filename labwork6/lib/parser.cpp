#include "parser.h"
#include <iterator>

namespace omfl {
    Argument parse(const std::string& str) {
        Argument p;
        Argument* current_section = &p;
        std::string current_argument;
        std::string current_value;
        std::istringstream stream(str);
        std::string line;
        while (std::getline(stream, line)) {
            size_t i = 0;
            int k_left_brace = 0;
            int k_right_brace = 0;
            current_argument = "";
            current_value = "";
            bool is_array = false;
            while (!(('A' <= line[i] && line[i] <= 'Z') || ('a' <= line[i] && line[i] <= 'z') ||
                     isdigit(line[i]) || line[i] == '-' || line[i] == '_' || line[i] == '[') && i < line.size()) {
                if (line[i] == '#') {
                    break;
                }
                i++;
            }
            if (line[i] == '[') {
                i++;
                std::string new_section_name;
                while (line[i] != ']') {
                    new_section_name.push_back(line[i]);
                    i++;
                }
                i++;
                Argument new_section;
                std::string current_subsection_name;
                Argument* current_subsection = &p;
                size_t j = 0;
                while (j < new_section_name.size()) {
                    if (new_section_name[j] == '.') {
                        for (Argument sect: current_subsection->subsections) {
                            if (sect.name == current_subsection_name) {
                                current_subsection = &sect;
                                break;
                            }
                        }
                        current_subsection_name = "";
                        j++;
                    }
                    current_subsection_name.push_back(new_section_name[j]);
                    j++;
                }
                for (Argument sect: current_subsection->subsections) {
                    if (sect.name == current_subsection_name) {
                        current_subsection = &sect;
                        break;
                    }
                }
                new_section.name = current_subsection_name;
                if (new_section_name != current_subsection->name) {
                    current_subsection->subsections.push_back(new_section);
                    current_section = &current_subsection->subsections[current_subsection->subsections.size() - 1];
                }
            }
            while (line[i] != ' ' && line[i] != '=' && i < line.size()) {
                if (line[i] == '#') {
                    break;
                }
                current_argument.push_back(line[i]);
                i++;
            }
            while (line[i] == ' ' || line[i] == '=') {
                if (line[i] == '#') {
                    break;
                }
                i++;
            }
            if (line[i] == '[') {
                is_array = true;
                k_left_brace = 1;
                current_value.push_back(line[i]);
                i++;
            }
            while ((!is_array && line[i] != ' ' && i < line.size()) ||
                   (is_array && k_left_brace - k_right_brace != 0)) {
                if (line[i] == '#') {
                    break;
                }
                if (line[i] == '[') {
                    k_left_brace++;
                }
                if (line[i] == ']') {
                    k_right_brace++;
                }
                current_value.push_back(line[i]);
                i++;
            }
            if (!current_argument.empty()) {
                Argument new_argument;
                new_argument.name = current_argument;
                new_argument.data = current_value;
                current_section->subsections.push_back(new_argument);
            }
        }
        return p;
    }

    bool Argument::IsInt() const {
        return IsInt(data);
    }

    bool Argument::IsInt(const std::string& str) const {
        size_t pos = 0;
        if (str[0] == '+' || str[0] == '-') {
            pos++;
        }

        for (size_t i = pos; i < str.size(); i++) {
            if (!isdigit(str[i])) {
                return false;
            }
        }
        return !str.empty() && str != "+" && str != "-";
    }

    uint32_t Argument::AsInt() const {
        return std::stoi(data);
    }

    uint32_t Argument::AsIntOrDefault(uint32_t a) const {
        if (!IsInt()) {
            return a;
        }
        return AsInt();
    }

    bool Argument::IsBool() const {
        return IsBool(data);
    }

    bool Argument::IsBool(const std::string& str) const {
        return str == "true" || str == "false";
    }

    bool Argument::AsBool() const {
        return data == "true";
    }

    bool Argument::IsString() const {
        return IsString(data);
    }

    bool Argument::IsString(const std::string& str) const {
        return str.front() == '"' && str.back() == '"';
    }

    std::string Argument::AsString() const {
        return data.substr(1, data.size() - 2);
    }

    std::string Argument::AsStringOrDefault(const std::string& str) {
        if (!IsString()) {
            return str;
        }
        return AsString();
    }

    bool Argument::IsFloat() const {
        return IsFloat(data);
    }

    bool Argument::IsFloat(const std::string& str) const {
        if (str == "+" || str == "-" || str == ".") {
            return false;
        }
        if (std::count(str.begin(), str.end(), '.') != 1 || str.front() == '.' || str.back() == '.') {
            return false;
        }
        if ((str.front() == '+' || str.front() == '-') && str[1] == '.') {
            return false;
        }
        size_t pos = 0;
        if (str[0] == '+' || str[0] == '-') {
            pos++;
        }
        for (size_t i = pos; i < str.size(); i++) {
            if (!isdigit(str[i]) && str[i] != '.') {
                return false;
            }
        }
        return true;
    }

    float Argument::AsFloat() {
        return std::stof(data);
    }

    float Argument::AsFloatOrDefault(float a) {
        if (!IsFloat()) {
            return a;
        }
        return AsFloat();
    }

    bool Argument::IsArray() const {
        return IsArray(data);
    }

    bool Argument::IsArray(std::string str) const {
        std::vector<std::string> array;
        if (!(str.front() == '[' && str.back() == ']')) {
            return false;
        }
        str.erase(0, 1);
        str.erase(str.size() - 1, 1);
        std::string temp;
        int k_braces = 0;
        int i = 0;
        while (i < str.size()) {
            if (str[i] == '[') {
                k_braces++;
                temp.push_back(str[i]);
                i++;
            }
            while (k_braces != 0) {
                if (str[i] == '[') {
                    k_braces++;
                } else if (str[i] == ']') {
                    k_braces--;
                }
                temp.push_back(str[i]);
                i++;
            }
            if (!temp.empty()) {
                array.push_back(temp);
                temp = "";
                i++;
            }
            if (i == str.size()) {
                break;
            }
            while (str[i] != ',' && str[i] != ' ' && i < str.size()) {
                temp.push_back(str[i]);
                i++;
            }
            if (!temp.empty()) {
                array.push_back(temp);
                temp = "";
                i++;
            }
            i++;
        }
        for (std::string str1: array) {
            if (str1.back() == ',') {
                str1.erase(str1.size() - 1, 1);
            }
            if (!(IsInt(str1) || IsBool(str1) || IsString(str1) || IsFloat(str1) || IsArray(str1))) {
                return false;
            }
        }
        return true;
    }

    Argument Argument::operator[](int a) {
        data.erase(0, 1);
        data.erase(data.size() - 1, 1);
        std::istringstream iss(data);
        std::vector<std::string> array;
        std::string temp;
        int k_braces = 0;
        int i = 0;
        while (i < data.size()) {
            if (data[i] == '[') {
                k_braces++;
                temp.push_back(data[i]);
                i++;
            }
            while (k_braces != 0) {
                if (data[i] == '[') {
                    k_braces++;
                } else if (data[i] == ']') {
                    k_braces--;
                }
                temp.push_back(data[i]);
                i++;
            }
            if (!temp.empty()) {
                array.push_back(temp);
                temp = "";
                i++;
            }
            if (i == data.size()) {
                break;
            }
            while (data[i] != ',' && data[i] != ' ' && i < data.size()) {
                temp.push_back(data[i]);
                i++;
            }
            if (!temp.empty()) {
                array.push_back(temp);
                temp = "";
                i++;
            }
            i++;
        }
        Argument ans;
        if (a < array.size()) {
            ans.data = array[a];
        }
        return ans;
    }

    bool Argument::valid() const {
        if (subsections.empty() && data.empty() && name.empty()) {
            return true;
        }
        if (!subsections.empty()) {
            return std::all_of(subsections.begin(), subsections.end(), [](const Argument& arg){return arg.valid();});
        }
        if (data.empty()) {
            return false;
        }
        if (!(this->IsInt() || this->IsBool() || this->IsString() || this->IsFloat() || this->IsArray())) {
            return false;
        }
        return true;
    }

    Argument Argument::Get(const std::string& str) const {
        if (str.find('.') == std::string::npos) {
            for (const Argument& sect: subsections) {
                if (sect.name == str) {
                    return sect;
                }
            }
        } else {
            size_t i = 0;
            std::string sect_name;
            auto current_section = const_cast<Argument*>(this);
            while (!current_section->subsections.empty() && i < str.size()) {
                while (str[i] != '.' && i < str.size()) {
                    sect_name.push_back(str[i]);
                    i++;
                }
                i++;
                for (Argument& sect: current_section->subsections) {
                    if (sect.name == sect_name) {
                        current_section = &sect;
                    }
                }
                sect_name.clear();
            }
            return *current_section;
        }
    }
}