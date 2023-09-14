#include "../lib/CutSQL.h"

void ParseClass::parse_text() {
    std::string cur_string;
    std::vector<std::string> cur_query;
    int k_braces = 0;
    for (char ch: to_parse_) {
        if (ch == ';') {
            cur_query.push_back(cur_string);
            parsed_queries_.push_back(cur_query);
            cur_query.clear();
            cur_string.clear();
        } else if (ch == '(') {
            k_braces++;
            cur_string.push_back(ch);
        } else if (ch == ')') {
            k_braces--;
            cur_string.push_back(ch);
        } else if (k_braces == 0) {
            if (ch == ' ' || ch == '\n' || ch == '\t' || ch == ',') {
                if (cur_string == "TABLE") {
                    if (cur_query.back() == "CREATE" || cur_query.back() == "DROP") {
                        cur_query.back() += ' ' + cur_string;
                    } else {
                        throw std::invalid_argument("wrong argument");
                    }
                    cur_string.clear();
                } else if (cur_string == "KEY") {
                    if (cur_query.back() == "PRIMARY" || cur_query.back() == "FOREIGN") {
                        cur_query.back() += ' ' + cur_string;
                    } else {
                        throw std::invalid_argument("wrong argument");
                    }
                    cur_string.clear();
                } else if (cur_string == "JOIN") {
                    if (cur_query.back() == "LEFT" || cur_query.back() == "RIGHT" || cur_query.back() == "INNER") {
                        cur_query.back() += ' ' + cur_string;
                    } else {
                        cur_query.push_back("INNER " + cur_string);
                    }
                    cur_string.clear();
                } else if (cur_string == "INTO") {
                    if (cur_query.back() == "INSERT") {
                        cur_query.back() += ' ' + cur_string;
                    } else {
                        throw std::invalid_argument("wrong argument");
                    }
                    cur_string.clear();
                } else if (cur_string == "FROM") {
                    if (cur_query.back() == "DELETE") {
                        cur_query.back() += ' ' + cur_string;
                    } else cur_query.emplace_back(cur_string);
                    cur_string.clear();
                } else if (!cur_string.empty()) {
                    cur_query.push_back(cur_string);
                    cur_string.clear();
                }
            } else cur_string.push_back(ch);
        } else {
            cur_string.push_back(ch);
        }
    }
}

void MyUncoolDB::create_table_parse(std::vector<std::string>& commands_in_query, int& i) {
    if (i + 1 == commands_in_query.size()) {
        throw std::length_error("not enough arguments");
    } else if (key_words.contains(commands_in_query[i + 1])) {
        throw std::invalid_argument("no name for a new table provided");
    } else if (all_tables_[commands_in_query[i + 1]]) {
        throw std::logic_error("cannot create an existing table");
    } else {
        ++i;
        if (key_words.find(commands_in_query[i]) != key_words.end()) {
            throw std::logic_error("key word cannot be chosen as a table name");
        }
        if (commands_in_query[i].find('(') < commands_in_query[i].size()) {
            throw std::logic_error("forgot space");
        }
        auto new_table = new MyUncoolTable(commands_in_query[i], this);
        all_tables_[commands_in_query[i]] = new_table;
        ++i;
        if (commands_in_query[i].find('(') >= commands_in_query[i].size()) {
            throw std::logic_error("table must have at least one column");
        }
        new_table->parse_parameters(commands_in_query[i]);
        if (new_table->primary_key_index() == -1) throw std::invalid_argument("parameter parsing error");
    }
}

void MyUncoolDB::drop_table_parse(const std::vector<std::string>& commands_in_query, int& i) {
    if (i + 1 == commands_in_query.size()) {
        throw std::length_error("not enough arguments");
    }
    ++i;
    if (!all_tables_[commands_in_query[i]]) {
        throw std::logic_error("cannot drop non-existing table");
    }
    all_tables_.erase(commands_in_query[i]);
}

void MyUncoolDB::insert_into_parse(const std::vector<std::string>& commands_in_query, int& i) {
    if (i + 1 == commands_in_query.size()) {
        throw std::length_error("not enough arguments");
    }
    ++i;
    if (!all_tables_[commands_in_query[i]]) {
        throw std::logic_error("cannot insert into non-existing table");
    }
    ++i;
    if (commands_in_query[i] != "VALUES") {
        throw std::invalid_argument("wrong values");
    }
    ++i;
    all_tables_[commands_in_query[i - 2]]->insert_line(commands_in_query[i]);
}

void MyUncoolDB::select_parse(const std::vector<std::string>& commands_in_query, int& i) {
    std::vector<std::string> selected_parameters;
    ++i;
    while (commands_in_query[i] != "FROM") {
        selected_parameters.push_back(commands_in_query[i]);
        ++i;
    }
    ++i;
    const std::string& table_name = commands_in_query[i];
    if (!all_tables_[table_name])
        throw std::invalid_argument("cannot select from non-existing table");
    std::string join;
    std::string second_table_name;
    std::string join_condition;
    if (i + 1 < commands_in_query.size() &&
        (commands_in_query[i + 1] == "INNER JOIN" || commands_in_query[i + 1] == "LEFT JOIN" ||
         commands_in_query[i + 1] == "RIGHT JOIN")) {
        join = commands_in_query[i + 1];
        i += 2;
        if (!all_tables_[commands_in_query[i]])
            throw std::invalid_argument("cannot join non-existing table");
        second_table_name = commands_in_query[i];
        ++i;
        if (commands_in_query[i] == "ON") {
            ++i;
            join_condition = commands_in_query[i];
        }
        if (selected_parameters.front() == "*") {
            if (selected_parameters.size() > 1)
                throw std::invalid_argument("cannot parse selected arguments");
            selected_parameters.clear();
            for (auto& r: all_tables_[table_name]->rows())
                selected_parameters.push_back(table_name + '.' + r);
            for (auto& r: all_tables_[second_table_name]->rows())
                selected_parameters.push_back(second_table_name + '.' + r);
        }
    } else {
        if (selected_parameters.front() == "*") {
            if (selected_parameters.size() > 1)
                throw std::invalid_argument("cannot parse selected arguments");
            selected_parameters = all_tables_[table_name]->rows();

        }
    }
    std::string predicates;
    if (i + 1 < commands_in_query.size() && commands_in_query[i + 1] == "WHERE") {
        i += 2;
        predicates = commands_in_query[i];
    }
    if (join.empty()) all_tables_[table_name]->select(selected_parameters, predicates);
    else join_select(table_name, second_table_name, selected_parameters, join_condition, predicates, join);
}

void MyUncoolDB::update_parse(const std::vector<std::string>& commands_in_query, int& i) {
    ++i;
    const std::string& table_name = commands_in_query[i];
    ++i;
    if (commands_in_query[i] != "SET") {
        throw std::invalid_argument("no values to set");
    }
    ++i;
    std::unordered_map<std::string, std::string> parameter_to_new_value;
    while (i < commands_in_query.size() && commands_in_query[i] != "WHERE") {
        std::string cur_str = commands_in_query[i];
        if (cur_str.front() == '(' && cur_str.back() == ')') {
            cur_str = cur_str.substr(1, cur_str.size() - 2);
        }
        std::string name;
        std::string value;
        int index = 0;
        while (cur_str[index] == ' ' || cur_str[index] == '\n' || cur_str[index] == '\t') ++index;
        while (!(cur_str[index] == ' ' || cur_str[index] == '\n' || cur_str[index] == '\t' ||
                 cur_str[index] == '=')) {
            name.push_back(cur_str[index]);
            ++index;
        }
        while (cur_str[index] == ' ' || cur_str[index] == '\n' || cur_str[index] == '\t' ||
               cur_str[index] == '=')
            ++index;
        while (!(cur_str[index] == ' ' || cur_str[index] == '\n' || cur_str[index] == '\t' ||
                 cur_str[index] == '=') && index < cur_str.size()) {
            value.push_back(cur_str[index]);
            ++index;
        }
        parameter_to_new_value[name] = value;
        ++i;
    }
    std::string predicates;
    if (i < commands_in_query.size()) {
        ++i;
        predicates = commands_in_query[i];
    }
    all_tables_[table_name]->update(parameter_to_new_value, predicates);
}

void MyUncoolDB::delete_from_parse(const std::vector<std::string>& commands_in_query, int& i) {
    ++i;
    const std::string& table_name = commands_in_query[i];
    ++i;
    std::string predicates;
    if (i != commands_in_query.size() && commands_in_query[i] == "WHERE") {
        ++i;
        if (i < commands_in_query.size()) {
            predicates = commands_in_query[i];
        } else throw std::invalid_argument("no predicates when needed");
    }
    all_tables_[table_name]->row_delete(predicates);
}

void MyUncoolDB::work_with_commands() {
    for (auto commands_in_query: queries_.parsed_queries()) {
        int i = 0;
        while (i < commands_in_query.size()) {
            if (commands_in_query[i] == "CREATE TABLE") {
                create_table_parse(commands_in_query, i);
            } else if (commands_in_query[i] == "DROP TABLE") {
                drop_table_parse(commands_in_query, i);
            } else if (commands_in_query[i] == "INSERT INTO") {
                insert_into_parse(commands_in_query, i);
            } else if (commands_in_query[i] == "SELECT") {
                select_parse(commands_in_query, i);
            } else if (commands_in_query[i] == "UPDATE") {
                update_parse(commands_in_query, i);
            } else if (commands_in_query[i] == "DELETE FROM") {
                delete_from_parse(commands_in_query, i);
            }
            ++i;
        }
    }
}

my_strange_type MyUncoolTable::operator()(const my_strange_type& prim_key, const std::string& row) {
    if (std::find(primary_keys_.begin(), primary_keys_.end(), prim_key) == primary_keys_.end() ||
        std::find(rows_.begin(), rows_.end(), row) == rows_.end())
        throw std::invalid_argument("no such element in data base");
    return lines_[prim_key][parameter_index_by_name(row)].get_data();
}

void MyUncoolTable::parse_parameters(const std::string& str) {
    std::string cur_string;
    std::string prev_string;
    std::string prev_prev_string;
    std::string last_foreign_key;
    int cur_parameter_index = -1;
    int k_braces = 0;
    for (char ch: str) {
        if (ch == '(') k_braces++;
        else if (ch == ')') k_braces--;
        else if (ch == ' ' || ch == ';' || ch == '\n' || ch == '\t' || ch == ',') {
            if (cur_string == "BOOL") {
                insert_row<bool>(cur_parameter_index, prev_string);
            } else if (cur_string == "INT") {
                insert_row<int>(cur_parameter_index, prev_string);
            } else if (cur_string == "FLOAT") {
                insert_row<float>(cur_parameter_index, prev_string);
            } else if (cur_string == "DOUBLE") {
                insert_row<double>(cur_parameter_index, prev_string);
            } else if (cur_string == "VARCHAR") {
                insert_row<std::string>(cur_parameter_index, prev_string);
            } else if (prev_string == "VARCHAR") {
                try {
                    std::stoi(cur_string);
                } catch (std::invalid_argument&) {
                    throw std::invalid_argument("no size for VARCHAR type");
                }
            } else if (cur_string == "KEY") {
                if (prev_string == "PRIMARY") {
                    if (primary_key_index_ != -1) throw std::logic_error("cannot have more than one primary key");
                    set_primary_key(cur_parameter_index);
                    types_.back().set_not_null();
                } else if (prev_string == "FOREIGN") {
                    last_foreign_key = cur_string;
                } else {
                    throw std::invalid_argument("missing a word before 'KEY'");
                }
            } else if (prev_string == "REFERENCES") {
                std::string table_name;
                std::string parameter_name;
                bool found = false;
                for (char symbol: cur_string) {
                    if (symbol == '.') found = true;
                    else if (!found) table_name.push_back(symbol);
                    else parameter_name.push_back(symbol);
                }
                if (!parent_->all_tables()[table_name]) {
                    throw std::logic_error("cannot point to non-existing table");
                }
                auto other_table = parent_->all_tables()[table_name];
                if (!other_table->parameter_index_by_name_.contains(parameter_name)) {
                    throw std::logic_error("no such parameter in table");
                }
                foreign_keys_[parameter_index_by_name_[last_foreign_key]] = &other_table->types_[other_table->parameter_index_by_name_[parameter_name]];
            } else if (cur_string == "NULL") {
                if (prev_string == "NOT") types_[cur_parameter_index].set_not_null();
                else types_[cur_parameter_index].set_null();
            }
            if (!(cur_string == " " || cur_string == ";" || cur_string == "\n" || cur_string == "\t" ||
                  cur_string == ",")) {
                prev_prev_string = prev_string;
                prev_string = cur_string;
            }
            cur_string.clear();
        } else cur_string.push_back(ch);
    }
    if (k_braces != 0) {
        throw std::invalid_argument("symbol ')' is missing");
    }
}

bool MyUncoolTable::insert_parameter(std::string& cur_parameter, int& cur_index, std::vector<Element>& line) {
    if (cur_parameter.empty()) return true;
    if (types_[cur_index].null()) {
        line[cur_index] = Element("NULL");
        cur_index++;
        if (cur_parameter == "NULL") return true;
    }
    if (cur_parameter == "NULL" && types_[cur_index].not_null())
        throw std::logic_error("try to insert null in non-null field");
    else if (std::holds_alternative<bool>(types_[cur_index].get_data())) {
        if (cur_parameter == "true" || cur_parameter == "1") line[cur_index] = Element(true);
        else if (cur_parameter == "false" || cur_parameter == "0") line[cur_index] = Element(false);
        else return false;
        cur_index++;
    } else if (std::holds_alternative<int>(types_[cur_index].get_data())) {
        bool is_int = true;
        for (char small_ch: cur_parameter) {
            if (!(std::isdigit(small_ch) || small_ch == '-')) {
                is_int = false;
                break;
            }
        }
        if (is_int) line[cur_index] = Element(std::stoi(cur_parameter));
        else return false;
        cur_index++;
    } else if (std::holds_alternative<float>(types_[cur_index].get_data()) ||
               std::holds_alternative<double>(types_[cur_index].get_data())) {
        bool is_float = true;
        bool found_dot = false;
        for (char small_ch: cur_parameter) {
            if (std::isdigit(small_ch)) continue;
            if (small_ch == '.' && !found_dot) found_dot = true;
            else {
                is_float = false;
                break;
            }
        }
        if (is_float) {
            if (std::holds_alternative<float>(types_[cur_index].get_data()))
                line[cur_index] = Element(std::stof(cur_parameter));
            else line[cur_index] = Element(std::stod(cur_parameter));
        } else return false;
        cur_index++;
    } else if (std::holds_alternative<std::string>(types_[cur_index].get_data())) {
        if (cur_parameter.front() == '\'' && cur_parameter.back() == '\'')
            line[cur_index] = Element(cur_parameter);
        else return false;
        cur_index++;
    }
    cur_parameter.clear();
    return true;
}

void MyUncoolTable::insert_line(const std::string& str) {
    std::string cur_parameter;
    int cur_index = 0;
    std::vector<Element> line(types_.size());
    bool ok = true;
    for (char ch: str) {
        if (!ok) break;
        if (ch == '(' || ch == ')') continue;
        if (ch == ',' || ch == ' ' || ch == '\n' || ch == '\t') {
            ok &= insert_parameter(cur_parameter, cur_index, line);
        } else cur_parameter.push_back(ch);
    }

    insert_parameter(cur_parameter, cur_index, line);

    if (cur_index != types_.size()) throw std::invalid_argument("not enough arguments to insert");

    if (ok) {
        if (std::find(primary_keys_.begin(), primary_keys_.end(), line[primary_key_index_].get_data()) !=
            primary_keys_.end())
            throw std::invalid_argument("primary keys should be unique");
        primary_keys_.push_back(line[primary_key_index_].get_data());
        lines_[line[primary_key_index_].get_data()] = line;
    } else throw std::invalid_argument("wrong arguments");
}

bool MyUncoolTable::predicate_parse(const std::string& predicate, std::vector<Element>& cur_line) {
    size_t index = 0;
    std::string parameter_name;
    while (!(predicate[index] == '=' || predicate[index] == '>' || predicate[index] == '<')) {
        parameter_name.push_back(predicate[index]);
        ++index;
    }
    auto parameter = cur_line[parameter_index_by_name_[parameter_name]].get_data();
    std::string cur_operator;
    if (predicate[index] == '=') cur_operator = "=";
    else if (predicate[index] == '<') {
        if (predicate[index + 1] == '=') {
            cur_operator = "<=";
            ++index;
        } else if (predicate[index + 1] == '>') {
            cur_operator = "<>";
            ++index;
        } else cur_operator = "<";
    } else if (predicate[index] == '>') {
        if (predicate[index + 1] == '=') {
            cur_operator = ">=";
            ++index;
        } else cur_operator = ">";
    }
    ++index;
    std::string comparable;
    while (index < predicate.size()) {
        comparable.push_back(predicate[index]);
        ++index;
    }
    if (std::holds_alternative<bool>(parameter)) {
        if (comparable == "true" || comparable == "1") {
            return work_with_predicates(get<bool>(parameter), cur_operator, true);
        } else if (comparable == "false" || comparable == "0") {
            return work_with_predicates(get<bool>(parameter), cur_operator, false);
        } else throw std::invalid_argument("cannot compare different types");
    } else if (std::holds_alternative<int>(parameter)) {
        return work_with_predicates(get<int>(parameter), cur_operator, std::stoi(comparable));
    } else if (std::holds_alternative<float>(parameter)) {
        return work_with_predicates(get<float>(parameter), cur_operator, std::stof(comparable));
    } else if (std::holds_alternative<double>(parameter)) {
        return work_with_predicates(get<double>(parameter), cur_operator, std::stod(comparable));
    } else {
        if (comparable.front() == '\'' && comparable.back() == '\'') {
            return work_with_predicates(get<std::string>(parameter), cur_operator, comparable);
        } else throw std::invalid_argument("cannot compare different types");
    }
}

bool MyUncoolDB::complex_predicate_parse(const std::string& predicate, const std::string& first_table_name,
                                         const std::string& second_table_name, my_strange_type& prim_key1,
                                         my_strange_type& prim_key2) {
    // поиск первого параметра
    std::string cur_table;
    std::string cur_param;
    int i = 0;
    while (predicate[i] != '.') {
        cur_table.push_back(predicate[i]);
        ++i;
    }
    ++i;
    while (!(predicate[i] == ' ' || predicate[i] == '=' || predicate[i] == '>' || predicate[i] == '<')) {
        cur_param.push_back(predicate[i]);
        ++i;
    }
    my_strange_type parameter1;
    if (cur_table == first_table_name) {
        auto table = all_tables_[first_table_name];
        parameter1 = table->lines()[prim_key1][table->parameter_index_by_name(cur_param)].get_data();
    } else if (cur_table == second_table_name) {
        auto table = all_tables_[second_table_name];
        parameter1 = table->lines()[prim_key2][table->parameter_index_by_name(cur_param)].get_data();
    } else throw std::invalid_argument("cannot find parameter in non-existing table");
    cur_table.clear();
    cur_param.clear();
    // поиск оператора
    while (predicate[i] == ' ') ++i;
    std::string cur_operator;
    if (predicate[i] == '=') cur_operator = "=";
    else if (predicate[i] == '<') {
        if (predicate[i + 1] == '=') {
            cur_operator = "<=";
            ++i;
        } else if (predicate[i + 1] == '>') {
            cur_operator = "<>";
            ++i;
        } else cur_operator = "<";
    } else if (predicate[i] == '>') {
        if (predicate[i + 1] == '=') {
            cur_operator = ">=";
            ++i;
        } else cur_operator = ">";
    }
    ++i;
    // поиск второго параметра
    while (predicate[i] == ' ') ++i;
    while (predicate[i] != '.') {
        cur_table.push_back(predicate[i]);
        ++i;
    }
    ++i;
    while (i < predicate.size()) {
        cur_param.push_back(predicate[i]);
        ++i;
    }
    my_strange_type parameter2;
    if (cur_table == first_table_name) {
        auto table = all_tables_[first_table_name];
        parameter1 = table->lines()[prim_key1][table->parameter_index_by_name(cur_param)].get_data();
    } else if (cur_table == second_table_name) {
        auto table = all_tables_[second_table_name];
        parameter2 = table->lines()[prim_key2][table->parameter_index_by_name(cur_param)].get_data();
    } else throw std::invalid_argument("cannot find parameter in non-existing table");
    // проверка
    if (std::holds_alternative<bool>(parameter1)) {
        if (std::holds_alternative<bool>(parameter2)) {
            return work_with_predicates(get<bool>(parameter1), cur_operator, get<bool>(parameter2));
        } else throw std::invalid_argument("cannot compare different types");
    } else if (std::holds_alternative<int>(parameter1)) {
        if (std::holds_alternative<int>(parameter2)) {
            return work_with_predicates(get<int>(parameter1), cur_operator, get<int>(parameter2));
        } else throw std::invalid_argument("cannot compare different types");
    } else if (std::holds_alternative<float>(parameter1)) {
        if (std::holds_alternative<float>(parameter2)) {
            return work_with_predicates(get<float>(parameter1), cur_operator, get<float>(parameter2));
        } else throw std::invalid_argument("cannot compare different types");
    } else if (std::holds_alternative<double>(parameter1)) {
        if (std::holds_alternative<double>(parameter2)) {
            return work_with_predicates(get<double>(parameter1), cur_operator, get<double>(parameter2));
        } else throw std::invalid_argument("cannot compare different types");
    } else {
        if (std::holds_alternative<std::string>(parameter2)) {
            return work_with_predicates(get<std::string>(parameter1), cur_operator, get<std::string>(parameter2));
        } else throw std::invalid_argument("cannot compare different types");
    }
}

void MyUncoolTable::select(const std::vector<std::string>& parameters, const std::string& predicates) {
    std::vector<my_strange_type> cur_vec;
    std::vector<std::vector<my_strange_type>> selected;
    for (auto& str: parameters) {
        cur_vec.emplace_back(str);
        std::cout << str << '\t';
    }
    selected.emplace_back(cur_vec);
    cur_vec.clear();
    std::cout << std::endl;
    std::vector<std::string> polish_notation_predicates = polish_notation(predicates);
    for (const auto& i: primary_keys_) {
        auto cur_line = lines_[i];
        std::stack<bool> stack;
        for (auto& cur: polish_notation_predicates) {
            if (cur == "AND" || cur == "OR") {
                bool a = stack.top();
                stack.pop();
                bool b = stack.top();
                stack.pop();
                if (cur == "AND") stack.emplace(a && b);
                else stack.emplace(a || b);
            } else {
                stack.push(predicate_parse(cur, cur_line));
            }
        }
        if (predicates.empty() || stack.top()) {
            for (const auto& param: parameters) {
                cur_vec.emplace_back(cur_line[parameter_index_by_name_[param]].get_data());
                std::cout << cur_line[parameter_index_by_name_[param]].get_data() << '\t';
            }
            selected.emplace_back(cur_vec);
            cur_vec.clear();
            std::cout << std::endl;
        }
    }
    parent_->selected_tables().emplace_back(selected);
    std::cout << std::endl;
}

void MyUncoolDB::join_select(std::string first_table_name, std::string second_table_name,
                             const std::vector<std::string>& selected_parameters, const std::string& join_condition,
                             const std::string& predicates, const std::string& type_of_join) {
    auto first_table = all_tables_[first_table_name];
    auto second_table = all_tables_[second_table_name];
    if (type_of_join == "RIGHT JOIN") {
        first_table = all_tables_[second_table_name];
        second_table = all_tables_[first_table_name];
        std::swap(first_table_name, second_table_name);
    }
    std::vector<std::string> first_table_parameters;
    std::vector<std::string> second_table_parameters;
    std::vector<std::string> polish_notation_predicates = polish_notation(predicates);
    for (auto& param: selected_parameters) { // парсинг параметров, которые надо вывести
        std::string cur_table;
        std::string cur_param;
        int i = 0;
        while (param[i] != '.') {
            cur_table.push_back(param[i]);
            ++i;
        }
        ++i;
        while (i < param.size()) {
            cur_param.push_back(param[i]);
            ++i;
        }
        if (cur_table == first_table_name) {
            first_table_parameters.push_back(cur_param);
        } else if (cur_table == second_table_name) {
            second_table_parameters.push_back(cur_param);
        } else throw std::invalid_argument("cannot find parameter in non-existing table");
    }
    std::vector<my_strange_type> cur_vec;
    for (auto& a: first_table_parameters) {
        cur_vec.emplace_back(a);
        std::cout << a << '\t';
    }
    for (auto& a: second_table_parameters) {
        cur_vec.emplace_back(a);
        std::cout << a << '\t';
    }
    std::vector<std::vector<my_strange_type>> selected;
    selected.emplace_back(cur_vec);
    cur_vec.clear();
    std::cout << std::endl;
    // парсинг соответствия
    int i = 0;
    int index_in_first;
    int index_in_second;
    for (int K = 0; K < 2; ++K) {
        while (join_condition[i] == ' ') ++i;
        std::string cur;
        while (join_condition[i] != '.') {
            cur.push_back(join_condition[i]);
            ++i;
        }
        ++i;
        std::string cur_param;
        while (i < join_condition.size() && join_condition[i] != '=') {
            cur_param.push_back(join_condition[i]);
            ++i;
        }
        ++i;
        if (cur == first_table_name) index_in_first = first_table->parameter_index_by_name(cur_param);
        else index_in_second = second_table->parameter_index_by_name(cur_param);
    }

    for (auto f: first_table->primary_keys()) {
        auto cur_line1 = first_table->lines()[f];
        bool matched = false; // нашли ли пару
        for (auto s: second_table->primary_keys()) { // сначала строим соответствие, потом проверяем по предикатам
            auto cur_line2 = second_table->lines()[s];
            if (!join_condition.empty()) {
                if (cur_line1[index_in_first].get_data() != cur_line2[index_in_second].get_data()) continue;
                // соответствие проверили, теперь сравниваем предикаты
                std::stack<bool> stack;
                for (auto& cur: polish_notation_predicates) {
                    if (cur == "AND" || cur == "OR") {
                        bool a = stack.top();
                        stack.pop();
                        bool b = stack.top();
                        stack.pop();
                        if (cur == "AND") stack.emplace(a && b);
                        else stack.emplace(a || b);
                    } else {
                        stack.push(complex_predicate_parse(cur, first_table_name, second_table_name, f, s));
                    }
                }
                if (predicates.empty() || stack.top()) {
                    matched = true;
                    for (const auto& param: first_table_parameters) {
                        cur_vec.emplace_back(cur_line1[first_table->parameter_index_by_name(param)].get_data());
                        std::cout << cur_line1[first_table->parameter_index_by_name(param)].get_data() << '\t';
                    }
                    for (const auto& param: second_table_parameters) {
                        cur_vec.emplace_back(cur_line2[second_table->parameter_index_by_name(param)].get_data());
                        std::cout << cur_line2[second_table->parameter_index_by_name(param)].get_data() << '\t';
                    }
                    selected.emplace_back(cur_vec);
                    cur_vec.clear();
                    std::cout << std::endl;
                }
            } else {
                matched = true;
                for (const auto& param: first_table_parameters) {
                    cur_vec.emplace_back(cur_line1[first_table->parameter_index_by_name(param)].get_data());
                    std::cout << cur_line1[first_table->parameter_index_by_name(param)].get_data() << '\t';
                }
                for (const auto& param: second_table_parameters) {
                    cur_vec.emplace_back(cur_line2[second_table->parameter_index_by_name(param)].get_data());
                    std::cout << cur_line2[second_table->parameter_index_by_name(param)].get_data() << '\t';
                }
                selected.emplace_back(cur_vec);
                cur_vec.clear();
                std::cout << std::endl;
            }
        }
        if (!matched && (type_of_join == "RIGHT JOIN" || type_of_join == "LEFT JOIN")) {
            for (const auto& param: first_table_parameters) {
                cur_vec.emplace_back(cur_line1[first_table->parameter_index_by_name(param)].get_data());
                std::cout << cur_line1[first_table->parameter_index_by_name(param)].get_data() << '\t';
            }
            for (const auto& param: second_table_parameters) {
                cur_vec.emplace_back("NULL");
                std::cout << "NULL" << '\t';
            }
            selected.emplace_back(cur_vec);
            cur_vec.clear();
            std::cout << std::endl;
        }
    }
    selected_tables_.emplace_back(selected);
    std::cout << std::endl;
}

void MyUncoolTable::update(const std::unordered_map<std::string, std::string>& parameters_to_new_value,
                           const std::string& predicates) {

    std::vector<std::string> polish_notation_predicates = polish_notation(predicates);
    for (const auto& i: primary_keys_) {
        auto cur_line = &lines_[i];
        std::stack<bool> stack;
        for (auto& cur: polish_notation_predicates) {
            if (cur == "AND" || cur == "OR") {
                bool a = stack.top();
                stack.pop();
                bool b = stack.top();
                stack.pop();
                if (cur == "AND") stack.emplace(a && b);
                else stack.emplace(a || b);
            } else {
                stack.push(predicate_parse(cur, *cur_line));
            }
        }
        if (predicates.empty() || stack.top()) {
            for (auto a: parameters_to_new_value) {
                if (std::holds_alternative<bool>((*cur_line)[parameter_index_by_name_[a.first]].get_data())) {
                    if (a.second == "true" || a.second == "1") {
                        (*cur_line)[parameter_index_by_name_[a.first]].set_data(true);
                    } else if (a.second == "false" || a.second == "0") {
                        (*cur_line)[parameter_index_by_name_[a.first]].set_data(false);
                    } else throw std::invalid_argument("cannot compare different types");
                } else if (std::holds_alternative<int>((*cur_line)[parameter_index_by_name_[a.first]].get_data())) {
                    (*cur_line)[parameter_index_by_name_[a.first]].set_data(std::stoi(a.second));
                } else if (std::holds_alternative<float>((*cur_line)[parameter_index_by_name_[a.first]].get_data())) {
                    (*cur_line)[parameter_index_by_name_[a.first]].set_data(std::stof(a.second));
                } else if (std::holds_alternative<double>((*cur_line)[parameter_index_by_name_[a.first]].get_data())) {
                    (*cur_line)[parameter_index_by_name_[a.first]].set_data(std::stod(a.second));
                } else if (std::holds_alternative<std::string>(
                        (*cur_line)[parameter_index_by_name_[a.first]].get_data())) {
                    if (a.second.front() == '\'' && a.second.back() == '\'') {
                        (*cur_line)[parameter_index_by_name_[a.first]].set_data(a.second);
                    } else throw std::invalid_argument("cannot compare different types");
                }
            }
        }
    }
}

void MyUncoolTable::row_delete(const std::string& predicates) {
    std::vector<std::string> polish_notation_predicates = polish_notation(predicates);
    auto cur_lines = lines_;
    auto prim_keys = primary_keys_;
    for (const auto& i: primary_keys_) {
        std::stack<bool> stack;
        for (auto& cur: polish_notation_predicates) {
            if (cur == "AND" || cur == "OR") {
                bool a = stack.top();
                stack.pop();
                bool b = stack.top();
                stack.pop();
                if (cur == "AND") stack.emplace(a && b);
                else stack.emplace(a || b);
            } else {
                stack.push(predicate_parse(cur, lines_[i]));
            }
        }
        if (predicates.empty() || stack.top()) {
            cur_lines.erase(i);
            auto it = std::remove(prim_keys.begin(), prim_keys.end(), i);
            prim_keys.erase(it);
        }
    }
    lines_ = cur_lines;
    primary_keys_ = prim_keys;
}

std::ostream& operator<<(std::ostream& stream, const my_strange_type& value) {
    std::visit([&stream](auto&& arg) { stream << arg; }, value);
    return stream;
}

std::vector<std::string> polish_notation(const std::string& usual) {
    std::vector<std::string> vec;
    std::string cur_str;
    for (char ch: usual) {
        if (ch == ')') {
            if (!cur_str.empty()) {
                vec.emplace_back(cur_str);
                cur_str.clear();
            }
            vec.emplace_back(")");
        } else if (ch == '(') vec.emplace_back("(");
        else if (ch == ' ' || ch == '\t' || ch == '\n') {
            if (!cur_str.empty()) {
                vec.push_back(cur_str);
                cur_str.clear();
            }
        } else cur_str.push_back(ch);
    }
    std::queue<std::string> queue;
    std::stack<std::string> stack;
    for (auto& s: vec) {
        if (s == "AND" || s == "OR") {
            if (stack.empty() || stack.top() == "(" || (s == "AND" && stack.top() == "OR")) stack.push(s);
            else {
                while (stack.top() != "OR" && stack.top() != "(") {
                    queue.push(stack.top());
                    stack.pop();
                }
                stack.push(s);
            }
        } else if (s == "(") stack.push(s);
        else if (s == ")") {
            while (stack.top() != "(") {
                queue.push(stack.top());
                stack.pop();
            }
            stack.pop();
        } else queue.push(s);
    }
    while (!stack.empty()) {
        queue.push(stack.top());
        stack.pop();
    }
    std::vector<std::string> v;
    while (!queue.empty()) {
        v.push_back(queue.front());
        queue.pop();
    }
    return v;
}

void MyUncoolDB::to_file(std::fstream& file) {
    for (auto& table: all_tables_) {
        file << table.first;
        for (auto& p: table.second->rows()) {
            file << p;
        }
        for (auto& line: table.second->lines()) {
            for (auto& param: line.second) {
                file << param.get_data();
            }
        }
    }
}
