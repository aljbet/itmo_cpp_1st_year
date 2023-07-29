#include "ArgParser.h"

using namespace ArgumentParser;

ArgParser& ArgParser::AddFlag(const std::string& flag, const std::string& definition) {
    this->names_descriptions[flag] = definition;
    this->bool_arguments_map[flag];
    this->to_have_default_value = flag;
    this->to_have_store_value = flag;
    this->number_of_undefined++;
    return *this;
}

ArgParser& ArgParser::AddFlag(char shortname, const std::string& flag) {
    this->shortnames_map[shortname] = flag;
    this->names_to_short_map[flag] = shortname;
    this->bool_arguments_map[flag];
    this->to_have_default_value = flag;
    this->to_have_store_value = flag;
    this->number_of_undefined++;
    return *this;
}

ArgParser& ArgParser::AddFlag(char shortname, const std::string& flag, const std::string& definition) {
    this->shortnames_map[shortname] = flag;
    this->names_to_short_map[flag] = shortname;
    this->names_descriptions[flag] = definition;
    this->bool_arguments_map[flag];
    this->to_have_default_value = flag;
    this->to_have_store_value = flag;
    this->number_of_undefined++;
    return *this;
}

ArgParser& ArgParser::AddStringArgument(const std::string& str) {
    this->string_arguments_map[str];
    this->to_have_default_value = str;
    this->to_have_store_value = str;
    this->number_of_undefined++;
    return *this;
}

ArgParser& ArgParser::AddStringArgument(char shortname, const std::string& str) {
    this->shortnames_map[shortname] = str;
    this->names_to_short_map[str] = shortname;
    this->string_arguments_map[str];
    this->to_have_default_value = str;
    this->to_have_store_value = str;
    this->number_of_undefined++;
    return *this;
}

ArgParser& ArgParser::AddStringArgument(char shortname, const std::string& str, const std::string& definition) {
    this->shortnames_map[shortname] = str;
    this->names_to_short_map[str] = shortname;
    this->names_descriptions[str] = definition;
    this->string_arguments_map[str];
    this->to_have_default_value = str;
    this->to_have_store_value = str;
    this->number_of_undefined++;
    return *this;
}

ArgParser& ArgParser::AddIntArgument(const std::string& int1) {
    this->int_arguments_map[int1];
    this->to_have_default_value = int1;
    this->to_have_store_value = int1;
    this->number_of_undefined++;
    return *this;
}

ArgParser& ArgParser::AddIntArgument(const std::string& int1, const std::string& definition) {
    this->int_arguments_map[int1];
    this->names_descriptions[int1] = definition;
    this->to_have_default_value = int1;
    this->to_have_store_value = int1;
    this->number_of_undefined++;
    return *this;
}

ArgParser& ArgParser::AddIntArgument(char shortname, const std::string& int1) {
    this->shortnames_map[shortname] = int1;
    this->names_to_short_map[int1] = shortname;
    this->int_arguments_map[int1];
    this->to_have_default_value = int1;
    this->to_have_store_value = int1;
    this->number_of_undefined++;
    return *this;
}

ArgParser& ArgParser::StoreValue(const std::string& a) {
    this->string_stored_values[this->to_have_store_value] = &a;
    return *this;
}

ArgParser& ArgParser::StoreValue(bool& a) {
    this->bool_stored_values[this->to_have_store_value] = &a;
    return *this;
}

ArgParser& ArgParser::StoreValues(std::vector<int>& vec) {
    this->int_stored_values[this->to_have_store_value] = &vec;
    return *this;
}

ArgParser& ArgParser::Default(const char* s) {
    std::string str = this->to_have_default_value;
    this->string_arguments_map[str] = s;
    this->number_of_undefined--;
    return *this;
}

ArgParser& ArgParser::Default(bool a) {
    std::string str = this->to_have_default_value;
    this->bool_arguments_map[str] = a;
    this->number_of_undefined--;
    return *this;
}

ArgParser& ArgParser::MultiValue(size_t a) {
    this->number_of_undefined += a - 1;
    return *this;
}

ArgParser& ArgParser::Positional() {
    this->is_positional = true;
    return *this;
}

bool ArgParser::GetFlag(const std::string& bool_name) {
    return this->bool_arguments_map[bool_name];
}

std::string ArgParser::GetStringValue(const std::string& string_name) {
    return this->string_arguments_map[string_name];
}

int ArgParser::GetIntValue(const std::string& int_name, int count) {
    return this->int_arguments_map[int_name][count];
}

bool ArgParser::Parse(const std::vector<std::string>& arguments_vector) {
    bool ans = true;
    if (this->is_positional) {
        for (const std::string& cur_arg: arguments_vector) {
            int current_answer = std::stoi(cur_arg);
            std::string current_argument = int_arguments_map.begin()->first;
            this->int_arguments_map[current_argument].push_back(current_answer);
            if (this->int_stored_values.find(current_argument) != this->int_stored_values.end()) {
                this->int_stored_values[current_argument]->push_back(current_answer);
            }
            this->number_of_undefined--;
        }
    } else {
        for (const std::string& current_operation: arguments_vector) {
            std::string current_argument;
            std::string current_value;
            if (current_operation[0] == '-') {
                int i = 1;
                if (current_operation[1] == '-') {
                    i = 2;
                    while (current_operation[i] != '=' && i < current_operation.size()) {
                        current_argument.push_back(current_operation[i]);
                        if (this->bool_arguments_map.find(current_argument) != this->bool_arguments_map.end()) {
                            this->number_of_undefined--;
                            bool current_answer = true;
                            this->bool_arguments_map[current_argument] = true;
                            if (this->bool_stored_values.find(current_argument) != this->bool_stored_values.end()) {
                                *(this->bool_stored_values[current_argument]) = current_answer;
                            }
                            break;
                        }
                        i++;
                    }
                    if (current_argument == "help") {
                        this->help_needed = true;
                    }
                } else {
                    while (current_operation[i] != '=' && i < current_operation.size()) {
                        current_argument = this->shortnames_map[current_operation[i]];
                        if (this->bool_arguments_map.find(current_argument) != this->bool_arguments_map.end()) {
                            this->number_of_undefined--;
                            bool current_answer = true;
                            this->bool_arguments_map[current_argument] = true;
                            if (this->bool_stored_values.find(current_argument) != this->bool_stored_values.end()) {
                                *(this->bool_stored_values[current_argument]) = current_answer;
                            }
                        }
                        i++;
                    }
                }
                i++;
                while (i < current_operation.size()) {
                    current_value.push_back(current_operation[i]);
                    i++;
                }
                if (this->string_arguments_map.find(current_argument) != this->string_arguments_map.end()) {
                    this->number_of_undefined--;
                    this->string_arguments_map[current_argument] = current_value;
                    if (this->string_stored_values.find(current_argument) != this->string_stored_values.end()) {
                        this->string_stored_values[current_argument] = &current_value;
                    }
                } else if (this->int_arguments_map.find(current_argument) != this->int_arguments_map.end()) {
                    this->number_of_undefined--;
                    int current_answer = std::stoi(current_value);
                    this->int_arguments_map[current_argument].push_back(current_answer);
                    if (this->int_stored_values.find(current_argument) != this->int_stored_values.end()) {
                        this->int_stored_values[current_argument]->push_back(current_answer);
                    }
                }
                if (current_argument == "help") {
                    this->help_needed = true;
                    return true;
                }
            } else {
                this->Help();
                ans = false;
            }
        }
    }
    if (this->number_of_undefined > 0) ans = false;
    return ans;
}

bool ArgParser::Parse(int number, char** elements) {
    bool ans = true;
    if (this->is_positional) {
        for (int j = 1; j < number; j++) {
            std::string current_value = elements[j];
            int current_answer = std::stoi(current_value);
            std::string current_argument = int_arguments_map.begin()->first;
            this->int_arguments_map[current_argument].push_back(current_answer);
            if (this->int_stored_values.find(current_argument) != this->int_stored_values.end()) {
                this->int_stored_values[current_argument]->push_back(current_answer);
            }
            this->number_of_undefined--;
        }
    } else {
        for (int j = 1; j < number; j++) {
            std::string current_operation = elements[j];
            std::string current_argument;
            std::string current_value;
            if (current_operation[0] == '-') {
                int i = 1;
                if (current_operation[1] == '-') {
                    i = 2;
                    while (current_operation[i] != '=' && i < current_operation.size()) {
                        current_argument.push_back(current_operation[i]);
                        if (this->bool_arguments_map.find(current_argument) != this->bool_arguments_map.end()) {
                            this->number_of_undefined--;
                            bool current_answer = true;
                            this->bool_arguments_map[current_argument] = true;
                            if (this->bool_stored_values.find(current_argument) != this->bool_stored_values.end()) {
                                *(this->bool_stored_values[current_argument]) = current_answer;
                            }
                            break;
                        }
                        i++;
                    }
                    if (current_argument == "help") {
                        this->help_needed = true;
                    }
                } else {
                    while (current_operation[i] != '=' && i < current_operation.size()) {
                        current_argument = this->shortnames_map[current_operation[i]];
                        if (this->bool_arguments_map.find(current_argument) != this->bool_arguments_map.end()) {
                            this->number_of_undefined--;
                            bool current_answer = true;
                            this->bool_arguments_map[current_argument] = true;
                            if (this->bool_stored_values.find(current_argument) != this->bool_stored_values.end()) {
                                *(this->bool_stored_values[current_argument]) = current_answer;
                            }
                        }
                        i++;
                    }
                }
                i++;
                while (i < current_operation.size()) {
                    current_value.push_back(current_operation[i]);
                    i++;
                }
                if (this->string_arguments_map.find(current_argument) != this->string_arguments_map.end()) {
                    this->number_of_undefined--;
                    this->string_arguments_map[current_argument] = current_value;
                    if (this->string_stored_values.find(current_argument) != this->string_stored_values.end()) {
                        (this->string_stored_values[current_argument]) = &current_value;
                    }
                } else if (this->int_arguments_map.find(current_argument) != this->int_arguments_map.end()) {
                    this->number_of_undefined--;
                    int current_answer = std::stoi(current_value);
                    this->int_arguments_map[current_argument].push_back(current_answer);
                    if (this->int_stored_values.find(current_argument) != this->int_stored_values.end()) {
                        this->int_stored_values[current_argument]->push_back(current_answer);
                    }
                }
                if (current_argument == "help") {
                    this->help_needed = true;
                    return true;
                }
            } else {
                this->Help();
                ans = false;
            }
        }
    }
    if (this->number_of_undefined > 0) {
        ans = false;
    }
    return ans;
}

bool ArgParser::Help() const {
    return this->help_needed;
}

void ArgParser::AddHelp(char shortname, const std::string& help, const std::string& desc) {
    this->help_needed = true;
    this->help_description = desc;
}

void ArgParser::HelpDescription() {
    std::cout << this->name << '\n';
    std::cout << this->help_description << '\n';
    std::cout << this->name << '\n';
    for (const auto& item: string_arguments_map) {
        if (this->names_to_short_map.find(item.first) != this->names_to_short_map.end()) {
            std::cout << '-' << names_to_short_map[item.first] << " ";
        }
        std::cout << "--" << item.first << "=" << item.second << " ";
        if (this->names_descriptions.find(item.first) != this->names_descriptions.end()) {
            std::cout << '-' << names_descriptions[item.first] << " ";
        }
        std::cout << '\n';
    }
    for (const auto& item: bool_arguments_map) {
        if (this->names_to_short_map.find(item.first) != this->names_to_short_map.end()) {
            std::cout << '-' << names_to_short_map[item.first] << " ";
        }
        std::cout << "--" << item.first << "=" << item.second << " ";
        if (this->names_descriptions.find(item.first) != this->names_descriptions.end()) {
            std::cout << '-' << names_descriptions[item.first] << " ";
        }
        std::cout << '\n';
    }
    for (const auto& item: int_arguments_map) {
        if (this->names_to_short_map.find(item.first) != this->names_to_short_map.end()) {
            std::cout << '-' << names_to_short_map[item.first] << " ";
        }
        std::cout << "--" << item.first << "=[";
        for (int j: item.second) {
            std::cout << j << " ";
        }
        std::cout << "] ";
        if (this->names_descriptions.find(item.first) != this->names_descriptions.end()) {
            std::cout << '-' << names_descriptions[item.first] << " ";
        }
        std::cout << '\n';
    }
}