#ifndef LABWORK12_CUTSQL_H
#define LABWORK12_CUTSQL_H

#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <variant>
#include <vector>
#include <queue>
#include <unordered_map>

/** For correct parsing user should put all parameters and predicates in braces
 * and put spaces between key words and parameters.
 * "PRIMARY KEY" and "FOREIGN KEY" cannot be added right after parameter declaration.
 */

const std::set<std::string> key_words = {"SELECT", "FROM", "WHERE", "LEFT JOIN", "RIGHT JOIN", "INNER JOIN",
                                         "CREATE TABLE", "DROP TABLE", "AND", "OR", "NOT", "NULL", "ON", "UPDATE",
                                         "INSERT", "VALUES", "DELETE FROM", "PRIMARY KEY", "FOREIGN KEY", "SET"};

using my_strange_type = std::variant<bool, int, float, double, std::string>;

std::ostream& operator<<(std::ostream& stream, const my_strange_type& value);

std::vector<std::string> polish_notation(const std::string& usual);

template <typename T>
bool work_with_predicates(const T& parameter, const std::string& my_operator, const T& comparable) {
    if (my_operator == "=") return parameter == comparable;
    else if (my_operator == ">") return parameter > comparable;
    else if (my_operator == "<") return parameter < comparable;
    else if (my_operator == ">=") return parameter >= comparable;
    else if (my_operator == "<=") return parameter <= comparable;
    else if (my_operator == "<>") return parameter != comparable;
    else throw std::invalid_argument("wrong operator");
};

class ParseClass {
public:
    ParseClass() = default;

    explicit ParseClass(std::string& str) : to_parse_(str) {};

    std::vector<std::vector<std::string>> parsed_queries() {
        return parsed_queries_;
    }

    void parse_text();

private:
    std::string to_parse_;
    std::vector<std::vector<std::string>> parsed_queries_;
};

class Element {
public:
    Element() : data_(my_strange_type()) {};

    explicit Element(my_strange_type data) {
        data_ = std::move(data);
    }

    [[nodiscard]] my_strange_type get_data() const {
        return data_;
    }

    void set_data(my_strange_type a) {
        data_ = std::move(a);
    }

    [[nodiscard]] bool null() const {
        return null_;
    }

    [[nodiscard]] bool not_null() const {
        return not_null_;
    }

    void set_null() {
        null_ = true;
    }

    void set_not_null() {
        not_null_ = true;
    }

private:
    my_strange_type data_;
    bool null_ = false;
    bool not_null_ = false;
};

class MyUncoolDB;

class MyUncoolTable {
public:
    MyUncoolTable() = default;

    explicit MyUncoolTable(std::string& str, MyUncoolDB* parent) : name_(str), parent_(parent) {}

    const std::string& name() const {
        return name_;
    }

    const MyUncoolDB* parent() const {
        return parent_;
    }

    int primary_key_index() const {
        return primary_key_index_;
    }

    const std::vector<my_strange_type>& primary_keys() const {
        return primary_keys_;
    }

    const std::vector<std::string>& rows() const {
        return rows_;
    }

    int parameter_index_by_name(const std::string& str) {
        return parameter_index_by_name_[str];
    }

    std::unordered_map<my_strange_type, std::vector<Element>> lines() const {
        return lines_;
    }

    void set_primary_key(int index) {
        primary_key_index_ = index;
    }

    my_strange_type operator()(const my_strange_type& prim_key, const std::string& row);

    template <typename T>
    void insert_row(int& cur_parameter_index, const std::string& prev_string) {
        cur_parameter_index++;
        my_strange_type a = T();
        auto el = Element(a);
        rows_.push_back(prev_string);
        parameter_index_by_name_[prev_string] = cur_parameter_index;
        types_.push_back(el);
    }

    void parse_parameters(const std::string& str);

    bool insert_parameter(std::string& cur_parameter, int& cur_index, std::vector<Element>& line);

    void insert_line(const std::string& str);

    bool predicate_parse(const std::string& predicate, std::vector<Element>& cur_line);

    void select(const std::vector<std::string>& parameters, const std::string& predicates);

    void
    update(const std::unordered_map<std::string, std::string>& parameters_to_new_value, const std::string& predicates);

    void row_delete(const std::string& predicates);

    ~MyUncoolTable() = default;

private:
    std::string name_;
    MyUncoolDB* parent_ = nullptr;
    int primary_key_index_ = -1;
    std::vector<my_strange_type> primary_keys_;
    std::vector<Element> types_; // типы столбцов в правильном порядке
    std::vector<std::string> rows_; // все параметры в упорядоченном порядке
    std::unordered_map<int, Element*> foreign_keys_; // по индексу foreign key возвращает указатель на столбец в другой таблице
    std::unordered_map<std::string, int> parameter_index_by_name_; // по названию столбца возвращает индекс, на котором тот стоит
    std::unordered_map<my_strange_type, std::vector<Element>> lines_; // по primary key возвращает строку
};

class MyUncoolDB {
public:
    MyUncoolDB() = default;

    explicit MyUncoolDB(ParseClass& str) : queries_(str) {};

    std::unordered_map<std::string, MyUncoolTable*> all_tables() {
        return all_tables_;
    }

    std::vector<std::vector<std::vector<my_strange_type>>>& selected_tables() {
        return selected_tables_;
    }

    MyUncoolTable operator[](const std::string& str) {
        if (!all_tables_.contains(str)) throw std::invalid_argument("table does not exist");
        return *all_tables_[str];
    }

    void create_table_parse(std::vector<std::string>& commands_in_query, int& i);

    void drop_table_parse(const std::vector<std::string>& commands_in_query, int& i);

    void insert_into_parse(const std::vector<std::string>& commands_in_query, int& i);

    void select_parse(const std::vector<std::string>& commands_in_query, int& i);

    void update_parse(const std::vector<std::string>& commands_in_query, int& i);

    void delete_from_parse(const std::vector<std::string>& commands_in_query, int& i);

    void work_with_commands();

    bool complex_predicate_parse(const std::string& predicate, const std::string& first_table_name,
                                 const std::string& second_table_name, my_strange_type& prim_key1,
                                 my_strange_type& prim_key2);


    void join_select(std::string first_table_name, std::string second_table_name,
                     const std::vector<std::string>& selected_parameters, const std::string& join_condition,
                     const std::string& predicates, const std::string& type_of_join);

    void to_file(std::fstream& file);

    ~MyUncoolDB() = default;

private:
    std::unordered_map<std::string, MyUncoolTable*> all_tables_;
    ParseClass queries_;
    std::vector<std::vector<std::vector<my_strange_type>>> selected_tables_;
};

#endif // LABWORK12_CUTSQL_H