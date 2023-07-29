#pragma once

#include <filesystem>
#include <istream>
#include <map>


namespace omfl {
    class Argument {
    public:
        std::vector<Argument> subsections;
        std::string name;
        std::string data;

        Argument() = default;

        explicit Argument(const std::string& x) : data(x) {}

        bool valid() const;

        Argument Get(const std::string& str) const;

        bool IsInt() const;

        bool IsInt(const std::string& str) const;

        uint32_t AsInt() const;

        uint32_t AsIntOrDefault(uint32_t a) const;

        bool IsBool() const;

        bool IsBool(const std::string& str) const;

        bool AsBool() const;

        bool IsFloat() const;

        bool IsFloat(const std::string& str) const;

        float AsFloat();

        float AsFloatOrDefault(float a);

        bool IsString() const;

        bool IsString(const std::string& str) const;

        std::string AsString() const;

        std::string AsStringOrDefault(const std::string& str);

        bool IsArray() const;

        bool IsArray(std::string str) const;

        Argument operator[](int a);
    };

    Argument parse(const std::string& str);
}
