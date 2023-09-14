#include "../lib/CutSQL.h"
#include "gtest/gtest.h"

TEST(ParserTest, PT1) {
    std::string str = R"(CREATE TABLE Students (
id INT PRIMARY KEY,
name VARCHAR (20)
);

INSERT INTO Students VALUES (1, 'Masha');
INSERT INTO Students VALUES (15, 'Lisa');)";
    ParseClass pc(str);
    pc.parse_text();
    std::vector<std::vector<std::string>> wanted = {{"CREATE TABLE", "Students", "(\nid INT PRIMARY KEY,\nname VARCHAR (20)\n)"},
                                                    {"INSERT INTO",  "Students", "VALUES", "(1, 'Masha')"},
                                                    {"INSERT INTO",  "Students", "VALUES", "(15, 'Lisa')"}};
    std::vector<std::vector<std::string>> got = pc.parsed_queries();
    ASSERT_EQ(wanted, got);
}

TEST(ParserTest, PT2) {
    std::string str = R"(CREATE TABLE Students (
id INT PRIMARY KEY,
name VARCHAR (20),
phone_number VARCHAR (12)
);

INSERT INTO Students VALUES (3, 'Igor');
INSERT INTO Students VALUES (0, 'PascalABC');

SELECT name FROM Students
WHERE (id = 0);)";
    ParseClass pc(str);
    pc.parse_text();
    std::vector<std::vector<std::string>> wanted = {{"CREATE TABLE", "Students", "(\nid INT PRIMARY KEY,\nname VARCHAR (20),\nphone_number VARCHAR (12)\n)"},
                                                    {"INSERT INTO",  "Students", "VALUES", "(3, 'Igor')"},
                                                    {"INSERT INTO",  "Students", "VALUES", "(0, 'PascalABC')"},
                                                    {"SELECT",       "name",     "FROM",   "Students", "WHERE", "(id = 0)"}};
    std::vector<std::vector<std::string>> got = pc.parsed_queries();
    ASSERT_EQ(wanted, got);
}

TEST(DBTest, Insert) {
    std::string str = R"(CREATE TABLE Students (
id INT PRIMARY KEY,
name VARCHAR (20),
grade VARCHAR (1)
);

INSERT INTO Students VALUES (3, 'Igor', 'A');
INSERT INTO Students VALUES (0, 'PascalABC', 'B');)";
    ParseClass pc(str);
    pc.parse_text();
    MyUncoolDB db(pc);
    db.work_with_commands();

    ASSERT_EQ(get<std::string>(db["Students"](0, "name")), "\'PascalABC\'");
    ASSERT_EQ(get<std::string>(db["Students"](0, "grade")), "\'B\'");
    ASSERT_EQ(get<std::string>(db["Students"](3, "name")), "\'Igor\'");
    ASSERT_EQ(get<std::string>(db["Students"](3, "grade")), "\'A\'");
}

TEST(DBTest, InsertTwoTables) {
    std::string str = R"(CREATE TABLE Students (
id INT PRIMARY KEY,
name VARCHAR (20),
grade VARCHAR (1)
);

INSERT INTO Students VALUES (3, 'Igor', 'A');
INSERT INTO Students VALUES (0, 'PascalABC', 'B');

CREATE TABLE AAA (
age INT,
name VARCHAR (20),
phone INT PRIMARY KEY
);

INSERT INTO AAA VALUES (15, 'qwe', 8922);
INSERT INTO AAA VALUES (20, 'rty', 8982);)";
    ParseClass pc(str);
    pc.parse_text();
    MyUncoolDB db(pc);
    db.work_with_commands();

    ASSERT_EQ(get<std::string>(db["Students"](0, "name")), "\'PascalABC\'");
    ASSERT_EQ(get<std::string>(db["Students"](0, "grade")), "\'B\'");
    ASSERT_EQ(get<std::string>(db["Students"](3, "name")), "\'Igor\'");
    ASSERT_EQ(get<std::string>(db["Students"](3, "grade")), "\'A\'");

    ASSERT_EQ(get<int>(db["AAA"](8922, "age")), 15);
    ASSERT_EQ(get<std::string>(db["AAA"](8922, "name")), "\'qwe\'");
    ASSERT_EQ(get<int>(db["AAA"](8982, "age")), 20);
    ASSERT_EQ(get<std::string>(db["AAA"](8982, "name")), "\'rty\'");
}

TEST(DBTest, SelectAll) {
    std::string str = R"(CREATE TABLE Students (
            id INT PRIMARY KEY,
            name VARCHAR (20),
            grade VARCHAR (1)
    );

    INSERT INTO Students VALUES (3, 'Igor', 'A');
    INSERT INTO Students VALUES (0, 'PascalABC', 'B');
    INSERT INTO Students VALUES (6, 'abcabc', 'C');

    CREATE TABLE ABC ( letter VARCHAR (2), number INT, d DOUBLE PRIMARY KEY );
    INSERT INTO ABC VALUES ('A', 10, 1.5);
    INSERT INTO ABC VALUES ('B', 2, 1);
    INSERT INTO ABC VALUES ('A', 0, 11.5);
    INSERT INTO ABC VALUES ('D', 13, 0.5);

    SELECT * FROM Students;
    SELECT * FROM ABC;
)";

    ParseClass pc(str);
    pc.parse_text();
    MyUncoolDB db(pc);
    db.work_with_commands();
    std::vector<std::vector<std::vector<my_strange_type>>> wanted = {
            {{"id",     "name",   "grade"},
                    {3,       "\'Igor\'", "\'A\'"},
                    {0,       "\'PascalABC\'", "\'B\'"},
                    {6,       "\'abcabc\'", "\'C\'"}},
            {{"letter", "number", "d"},
                    {"\'A\'", 10,         1.5},
                    {"\'B\'", 2,               1.0},
                    {"\'A\'", 0,            11.5},
                    {"\'D\'", 13, 0.5}}
    };
    ASSERT_EQ(wanted, db.selected_tables());
}

TEST(DBTest, SelectRows) {
    std::string str = R"(CREATE TABLE Students (
            id INT PRIMARY KEY,
            name VARCHAR (20),
            grade VARCHAR (1)
    );

    INSERT INTO Students VALUES (3, 'Igor', 'A');
    INSERT INTO Students VALUES (0, 'PascalABC', 'B');
    INSERT INTO Students VALUES (6, 'abcabc', 'C');

    CREATE TABLE ABC ( letter VARCHAR (2), number INT, d DOUBLE PRIMARY KEY );
    INSERT INTO ABC VALUES ('A', 10, 1.5);
    INSERT INTO ABC VALUES ('B', 2, 1);
    INSERT INTO ABC VALUES ('A', 0, 11.5);
    INSERT INTO ABC VALUES ('D', 13, 0.5);

    SELECT id, grade FROM Students;
    SELECT number, d FROM ABC;
)";

    ParseClass pc(str);
    pc.parse_text();
    MyUncoolDB db(pc);
    db.work_with_commands();
    std::vector<std::vector<std::vector<my_strange_type>>> wanted = {
            {{"id",     "grade"},
                    {3,  "\'A\'"},
                    {0, "\'B\'"},
                    {6, "\'C\'"}},
            {{"number", "d"},
                    {10, 1.5},
                    {2, 1.0},
                    {0, 11.5},
                    {13, 0.5}}
    };
    ASSERT_EQ(wanted, db.selected_tables());
}

TEST(DBTest, SelectWhere) {
    std::string str = R"(CREATE TABLE Students (
            id INT PRIMARY KEY,
            name VARCHAR (20),
            grade VARCHAR (1)
    );

    INSERT INTO Students VALUES (3, 'Igor', 'A');
    INSERT INTO Students VALUES (0, 'PascalABC', 'B');
    INSERT INTO Students VALUES (6, 'abcabc', 'C');
    INSERT INTO Students VALUES (9, 'aa', 'A');

    CREATE TABLE ABC ( letter VARCHAR (2), number INT, d DOUBLE PRIMARY KEY );
    INSERT INTO ABC VALUES ('A', 10, 1.5);
    INSERT INTO ABC VALUES ('B', 2, 1);
    INSERT INTO ABC VALUES ('D', 0, 11.5);
    INSERT INTO ABC VALUES ('D', 13, 0.5);

    SELECT * FROM Students WHERE (grade='A');
    SELECT * FROM ABC WHERE (number>5);
)";

    ParseClass pc(str);
    pc.parse_text();
    MyUncoolDB db(pc);
    db.work_with_commands();
    std::vector<std::vector<std::vector<my_strange_type>>> wanted = {
            {{"id",     "name",   "grade"},
                    {3,       "\'Igor\'", "\'A\'"},
                    {9,       "\'aa\'", "\'A\'"}},
            {{"letter", "number", "d"},
                    {"\'A\'", 10,         1.5},
                    {"\'D\'", 13,       0.5}}
    };
    ASSERT_EQ(wanted, db.selected_tables());
}

TEST(DBTest, SelectomplexWhere) {
    std::string str = R"(CREATE TABLE Students (
            id INT PRIMARY KEY,
            name VARCHAR (20),
            grade VARCHAR (1)
    );

    INSERT INTO Students VALUES (3, 'Igor', 'A');
    INSERT INTO Students VALUES (0, 'PascalABC', 'B');
    INSERT INTO Students VALUES (6, 'abcabc', 'C');
    INSERT INTO Students VALUES (9, 'aa', 'A');
    INSERT INTO Students VALUES (5, 'kk', 'A');
    INSERT INTO Students VALUES (1, 'qwqw', 'B');

    SELECT * FROM Students WHERE (((grade='A') AND (id>4)) OR (name='abcabc'));
)";

    ParseClass pc(str);
    pc.parse_text();
    MyUncoolDB db(pc);
    db.work_with_commands();
    std::vector<std::vector<std::vector<my_strange_type>>> wanted = {
            {{"id", "name", "grade"},
             {6, "\'abcabc\'", "\'C\'"},
             {9, "\'aa\'", "\'A\'"},
             {5, "\'kk\'", "\'A\'"}}
    };
    ASSERT_EQ(wanted, db.selected_tables());
}

TEST(DBTest, Update) {
    std::string str = R"(CREATE TABLE Students (
id INT PRIMARY KEY,
name VARCHAR (20),
grade VARCHAR (1)
);

INSERT INTO Students VALUES (3, 'Igor', 'A');
INSERT INTO Students VALUES (0, 'PascalABC', 'B');

SELECT * FROM Students;

UPDATE Students SET grade='F';

SELECT * FROM Students;
)";
    ParseClass pc(str);
    pc.parse_text();
    MyUncoolDB db(pc);
    db.work_with_commands();

    std::vector<std::vector<std::vector<my_strange_type>>> wanted = {
            {{"id", "name", "grade"},
                    {3, "\'Igor\'", "\'A\'"},
                    {0, "\'PascalABC\'", "\'B\'"}},
            {{"id", "name", "grade"},
                    {3, "\'Igor\'", "\'F\'"},
                    {0, "\'PascalABC\'", "\'F\'"}}
    };
    ASSERT_EQ(wanted, db.selected_tables());
}

TEST(DBTest, UpdateWhere) {
    std::string str = R"(CREATE TABLE Students (
id INT PRIMARY KEY,
name VARCHAR (20),
grade VARCHAR (1)
);

INSERT INTO Students VALUES (3, 'Igor', 'A');
INSERT INTO Students VALUES (0, 'PascalABC', 'B');
INSERT INTO Students VALUES (6, 'abcabc', 'C');
INSERT INTO Students VALUES (-12, 'Kkk', 'A');
INSERT INTO Students VALUES (1, 'a', 'B');
INSERT INTO Students VALUES (99, 'AQWS', 'E');
INSERT INTO Students VALUES (101, 'poi', 'B');

SELECT * FROM Students;

UPDATE Students SET grade='F' WHERE (grade='B');

SELECT * FROM Students;
)";
    ParseClass pc(str);
    pc.parse_text();
    MyUncoolDB db(pc);
    db.work_with_commands();

    std::vector<std::vector<std::vector<my_strange_type>>> wanted = {
            {{"id", "name", "grade"},
                    {3, "\'Igor\'", "\'A\'"},
                    {0, "\'PascalABC\'", "\'B\'"},
                    {6, "\'abcabc\'", "\'C\'"},
                    {-12, "\'Kkk\'", "\'A\'"},
                    {1, "\'a\'", "\'B\'"},
                    {99, "\'AQWS\'", "\'E\'"},
                    {101, "\'poi\'", "\'B\'"}},
            {{"id", "name", "grade"},
                    {3, "\'Igor\'", "\'A\'"},
                    {0, "\'PascalABC\'", "\'F\'"},
                    {6, "\'abcabc\'", "\'C\'"},
                    {-12, "\'Kkk\'", "\'A\'"},
                    {1, "\'a\'", "\'F\'"},
                    {99, "\'AQWS\'", "\'E\'"},
                    {101, "\'poi\'", "\'F\'"}}
    };
    ASSERT_EQ(wanted, db.selected_tables());

}

TEST(DBTest, DeleteAll) {
    std::string str = R"(CREATE TABLE Students (
id INT PRIMARY KEY,
name VARCHAR (20),
grade VARCHAR (1)
);

INSERT INTO Students VALUES (3, 'Igor', 'A');
INSERT INTO Students VALUES (0, 'PascalABC', 'B');

SELECT * FROM Students;

DELETE FROM Students;

SELECT * FROM Students;
)";
    ParseClass pc(str);
    pc.parse_text();
    MyUncoolDB db(pc);
    db.work_with_commands();

    std::vector<std::vector<std::vector<my_strange_type>>> wanted = {
            {{"id", "name", "grade"},
                    {3, "\'Igor\'", "\'A\'"},
                    {0, "\'PascalABC\'", "\'B\'"}},
            {{"id", "name", "grade"}}
    };
    ASSERT_EQ(wanted, db.selected_tables());
}

TEST(DBTest, DeleteWhere) {
    std::string str = R"(CREATE TABLE Students (
id INT PRIMARY KEY,
name VARCHAR (20),
grade VARCHAR (1)
);

INSERT INTO Students VALUES (3, 'Igor', 'A');
INSERT INTO Students VALUES (0, 'PascalABC', 'B');
INSERT INTO Students VALUES (6, 'abcabc', 'C');
INSERT INTO Students VALUES (-12, 'Kkk', 'A');
INSERT INTO Students VALUES (1, 'a', 'B');
INSERT INTO Students VALUES (99, 'AQWS', 'E');
INSERT INTO Students VALUES (101, 'poi', 'B');


SELECT * FROM Students;

DELETE FROM Students WHERE (grade='B' OR grade='E' OR id<0);

SELECT * FROM Students;
)";
    ParseClass pc(str);
    pc.parse_text();
    MyUncoolDB db(pc);
    db.work_with_commands();

    std::vector<std::vector<std::vector<my_strange_type>>> wanted = {
            {{"id", "name", "grade"},
                    {3, "\'Igor\'", "\'A\'"},
                    {0, "\'PascalABC\'", "\'B\'"},
                    {6, "\'abcabc\'", "\'C\'"},
                    {-12, "\'Kkk\'", "\'A\'"},
                    {1, "\'a\'", "\'B\'"},
                    {99, "\'AQWS\'", "\'E\'"},
                    {101, "\'poi\'", "\'B\'"}},
            {{"id", "name", "grade"},
                    {3, "\'Igor\'", "\'A\'"},
                    {6, "\'abcabc\'", "\'C\'"}}
    };
    ASSERT_EQ(wanted, db.selected_tables());
}

TEST(DBTest, InnerJoin) {
    std::string str = R"(CREATE TABLE Students (
            id INT PRIMARY KEY,
            name VARCHAR (20),
            grade VARCHAR (1)
    );

    INSERT INTO Students VALUES (3, 'Igor', 'A');
    INSERT INTO Students VALUES (0, 'PascalABC', 'B');
    INSERT INTO Students VALUES (6, 'abcabc', 'C');

CREATE TABLE ABC ( letter VARCHAR (2), number INT, d DOUBLE PRIMARY KEY );
INSERT INTO ABC VALUES ('A', 10, 1.5);
INSERT INTO ABC VALUES ('B', 2, 1);
INSERT INTO ABC VALUES ('A', 0, 11.5);
INSERT INTO ABC VALUES ('D', 13, 0.5);

SELECT Students.id, Students.name, ABC.letter, ABC.number FROM Students JOIN ABC ON Students.grade=ABC.letter;
)";
    ParseClass pc(str);
    pc.parse_text();
    MyUncoolDB db(pc);
    db.work_with_commands();

    std::vector<std::vector<std::vector<my_strange_type>>> wanted = {
            {{"id", "name", "letter", "number"},
             {3, "\'Igor\'", "\'A\'", 10},
             {3, "\'Igor\'", "\'A\'", 0},
             {0, "\'PascalABC\'", "\'B\'", 2}}
    };
    ASSERT_EQ(wanted, db.selected_tables());
}

TEST(DBTest, InnerJoinAll) {
    std::string str = R"(CREATE TABLE Students (
            id INT PRIMARY KEY,
            name VARCHAR (20),
            grade VARCHAR (1)
    );

    INSERT INTO Students VALUES (3, 'Igor', 'A');
    INSERT INTO Students VALUES (0, 'PascalABC', 'B');
    INSERT INTO Students VALUES (6, 'abcabc', 'C');

CREATE TABLE ABC ( letter VARCHAR (2), number INT, d DOUBLE PRIMARY KEY );
INSERT INTO ABC VALUES ('A', 10, 1.5);
INSERT INTO ABC VALUES ('B', 2, 1);
INSERT INTO ABC VALUES ('A', 0, 11.5);
INSERT INTO ABC VALUES ('D', 13, 0.5);

SELECT * FROM Students JOIN ABC ON Students.grade=ABC.letter;
)";
    ParseClass pc(str);
    pc.parse_text();
    MyUncoolDB db(pc);
    db.work_with_commands();

    std::vector<std::vector<std::vector<my_strange_type>>> wanted = {
            {{"id", "name", "grade", "letter", "number", "d"},
             {3, "\'Igor\'", "\'A\'", "\'A\'", 10, 1.5},
             {3, "\'Igor\'", "\'A\'", "\'A\'", 0, 11.5},
             {0, "\'PascalABC\'", "\'B\'", "\'B\'", 2, 1.0}}
    };
    ASSERT_EQ(wanted, db.selected_tables());
}

TEST(DBTest, LeftJoin) {
    std::string str = R"(CREATE TABLE Students (
            id INT PRIMARY KEY,
            name VARCHAR (20),
            grade VARCHAR (1)
    );

    INSERT INTO Students VALUES (3, 'Igor', 'A');
    INSERT INTO Students VALUES (0, 'PascalABC', 'B');
    INSERT INTO Students VALUES (6, 'abcabc', 'C');

CREATE TABLE ABC ( letter VARCHAR (2), number INT, d DOUBLE PRIMARY KEY );
INSERT INTO ABC VALUES ('A', 10, 1.5);
INSERT INTO ABC VALUES ('B', 2, 1);
INSERT INTO ABC VALUES ('A', 0, 11.5);
INSERT INTO ABC VALUES ('D', 13, 0.5);

SELECT Students.id, Students.name, ABC.letter, ABC.number FROM Students LEFT JOIN ABC ON Students.grade=ABC.letter;
)";
    ParseClass pc(str);
    pc.parse_text();
    MyUncoolDB db(pc);
    db.work_with_commands();

    std::vector<std::vector<std::vector<my_strange_type>>> wanted = {
            {{"id", "name", "letter", "number"},
             {3, "\'Igor\'", "\'A\'", 10},
             {3, "\'Igor\'", "\'A\'", 0},
             {0, "\'PascalABC\'", "\'B\'", 2},
             {6, "\'abcabc\'", "NULL", "NULL"}}
    };
    ASSERT_EQ(wanted, db.selected_tables());
}

TEST(DBTest, RightJoin) {
    std::string str = R"(CREATE TABLE Students (
            id INT PRIMARY KEY,
            name VARCHAR (20),
            grade VARCHAR (1)
    );

    INSERT INTO Students VALUES (3, 'Igor', 'A');
    INSERT INTO Students VALUES (0, 'PascalABC', 'B');
    INSERT INTO Students VALUES (6, 'abcabc', 'C');

CREATE TABLE ABC ( letter VARCHAR (2), number INT, d DOUBLE PRIMARY KEY );
INSERT INTO ABC VALUES ('A', 10, 1.5);
INSERT INTO ABC VALUES ('B', 2, 1);
INSERT INTO ABC VALUES ('A', 0, 11.5);
INSERT INTO ABC VALUES ('D', 13, 0.5);

SELECT Students.id, Students.name, ABC.letter, ABC.number FROM Students RIGHT JOIN ABC ON Students.grade=ABC.letter;
)";
    ParseClass pc(str);
    pc.parse_text();
    MyUncoolDB db(pc);
    db.work_with_commands();

    std::vector<std::vector<std::vector<my_strange_type>>> wanted = {
            {{"letter", "number", "id", "name"},
             {"\'A\'", 10, 3, "\'Igor\'"},
             {"\'B\'", 2, 0, "\'PascalABC\'"},
             {"\'A\'", 0, 3, "\'Igor\'"},
             {"\'D\'", 13, "NULL", "NULL"}}
    };
    ASSERT_EQ(wanted, db.selected_tables());
}