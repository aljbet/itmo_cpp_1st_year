#include "../lib/CutSQL.h"

int main() {
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

SELECT * FROM Students RIGHT JOIN ABC ON Students.grade=ABC.letter;
)";

    ParseClass pc(str);
    pc.parse_text();
    MyUncoolDB db(pc);
    db.work_with_commands();
    return 0;
}