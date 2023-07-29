
# Лабораторная работа 6

OneMoreFunnyLanguage format

OMFL - это формат для хранения конфигурационных файлов. По своей природе он похож на ini, но является более гибким. Основной его целью является возможность получать из конфига значение по определенному ключу и логическое разделение на секции и подсекции.

## Задача

Ваша задача состоит в том, чтобы реализовать парсер для OMFL.

## Формат

OMFL-файл это текстовый файл в кодировке ASCII формате OMFL. Далее будет описана спецификация формата. Основной конструкцией для OMFL является пара KeyValue.
Формат чувствителен к регистру. Пробелы и пустые строки игнорируются.

### Ключ\Значение

Ключ, знак равенства, значение.
```text
key = "value"
```

И ключ, и значение являются обязательными. Перенос строки - запрещен.
Значение не может быть переопределено для одного и того же ключа (в рамках одной секции).

#### Ключ

Ключ может состоять из:

- Заглавных и строчных латинских букв
- Цифр
- Символов '-' и '_'

Ключ не может быть нулевой длины.

```text
number = 0
name = "M3100"
```

#### Значение

Значение может быть одним из следующих типов:

- целое число
- вещественное число
- строка
- логическая переменная
- массив из значений

#### Целое число

Состоит из цифр(одной или более). Возможно добавление символа '+' или '-' в качестве первого.

```text
key1 = 1
key2 = -2022
key3 = +128
```

Возможное значение поместится в int32_t

#### Вещественное число

Состоит из цифр (одной или более) и одной '.'. Перед точкой и после неё должна быть хотя бы одна цифра.
Возможно добавление символа '+' или '-' в качестве первого.

```text
key1 = 1.23
key2 = -2.77
key3 = -0.0001
```

##### Строка

Строка окружена двойными кавычками. Содержит любые символы.

```text
key1 = "value"
key2 = "Hello world!"
```

#### Логическое значение

Для логических значений используется литерал "true" или "false"

```text
key1 = true
key2 = false
```

#### Массив

Массив окружён символами '[' и ']'. Элементы разделены ','.
Массив может состоять из любых корректных значений, необязательно одного типа.

```text
key1 = [1, 2, 3, 4, 5]
key2 = ["Hello", "world"]
key3 = [[1, 2, 3, 4, 5], ["Hello", "world"]]
key4 = [1, 3.3, "ITMO", [true, false]]
```

### Секции

Помимо блока ключ значения, формат поддерживает секции. Секции позволяют объединять множества Ключ\Значение в логические блоки.
Секция определяется как Ключ, окружённый знаками '[' и ']'

```text
[name]
```

После объявления секции все последующие пары Ключ\Значения принадлежат ей до того момента, пока не будет объявлена следующая.

```text
[section-1]
key1 = 1
key2 = "2"

[section-2]
key1 = 2
```

Хотя секция подчиняется правилам Ключей, она может содержать еще и символ '.', что определяет вложенные друг в друга секции.

```text
[section-1.part-1.x]

[section-1.part-1.y]

[section-1.part-2.x]

[section-2.z]
```

Таким образом секция может содержать как пары Ключ\Значение, так и другие секции. Ключ и название подсекции не могут совпадать.

```text
[A]
key1 = 1

[A.B]
key2 = 3

[A.B.C]
key3 = 3
```

#### Комментарии

Конфиг может содержать однострочный комментарий. Комментарии начинаются с символа '#', кроме случаев когда '#' находится внутри строкового значения.

```text
    key1 = "value1"  # some comment
    # another comment
```

## Парсер

Цель работы состоит в том, чтобы реализовать парсер формат OMFL.

Парсер должен:

- Валидировать корректность файла согласно Формату OMFL.
- Считывать данные из файла в объект, класс которого обладает интерфейсом, позволяющим получить секцию или значение по ключу.
- То, каким интерфейсом должен обладать парсер, описано в тестах.

Т.к. мы еще не знакомы с исключениями, то при попытке получить значение или секцию по несуществующему ключу возвращаемое значение не определено.

## Тесты

В директории tests находятся только базовые тесты на соблюдение формата и парсер.
Рекомендуется написать тесты на те структуры данных и функции, которые вы создадите, и, возможно, дополнить существующие.

## Happy New Year Deadline

1. 18.12.22 24:00 - 0.8
2. 25.12.22 24:00 - 0.65
3. 01.01.23 24:00 - 0.5