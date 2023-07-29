#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace ArgumentParser {

    class ArgParser {
    private:
        std::string name;
        bool help_needed = false;
        std::map<std::string, std::string> string_arguments_map;
        std::map<std::string, std::vector<int>> int_arguments_map;
        std::map<std::string, bool> bool_arguments_map;
        std::map<std::string, const std::string*> string_stored_values;
        std::map<std::string, bool*> bool_stored_values;
        std::map<std::string, std::vector<int>*> int_stored_values;
        std::map<char, std::string> shortnames_map;
        std::map<std::string, char> names_to_short_map;
        std::map<std::string, std::string> names_descriptions;
        std::string to_have_default_value;
        std::string to_have_store_value;
        size_t number_of_undefined;
        bool is_positional;
        std::string help_description;

    public:
        ArgParser(const std::string& str) {
            name = str;
            number_of_undefined = 0;
            is_positional = false;
        }

        ArgParser& AddFlag(const std::string& flag, const std::string& description);

        ArgParser& AddFlag(char shortname, const std::string& flag);

        ArgParser& AddFlag(char shortname1, const std::string& flag1, const std::string& flag2);

        ArgParser& AddStringArgument(const std::string& str);

        ArgParser& AddStringArgument(char shortname, const std::string& str);

        ArgParser& AddStringArgument(char shortname1, const std::string& string1, const std::string& string2);

        ArgParser& AddIntArgument(const std::string& int1);

        ArgParser& AddIntArgument(const std::string& int1, const std::string& int2);

        ArgParser& AddIntArgument(char shortname, const std::string& fullname);

        ArgParser& StoreValue(const std::string& a);

        ArgParser& StoreValue(bool& a);

        ArgParser& StoreValues(std::vector<int>& vec);

        ArgParser& Default(const char* s);

        ArgParser& Default(bool a);

        ArgParser& MultiValue(size_t a = 1);

        ArgParser& Positional();

        bool GetFlag(const std::string& bool_name);

        std::string GetStringValue(const std::string& string_name);

        int GetIntValue(const std::string& int_name, int count = 0);

        void AddHelp(char shortname, const std::string& help, const std::string& desc);

        bool Parse(const std::vector<std::string>& arguments_vector);

        bool Parse(int number, char** elements);

        bool Help() const;

        void HelpDescription();
    };
}