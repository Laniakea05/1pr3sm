#include <iostream>
#include "select.h"

// Функция для выполнения операции SELECT с двумя таблицами и возможным условием WHERE
void select(string command, JsonTable& jatab) {
    istringstream iss(command); // Создаем поток для разбора команды
    string mes;
    iss >> mes; // Читаем первое слово команды (например, SELECT)
    iss >> mes; // Читаем второе слово команды (например, имя первой таблицы с колонкой)

    string table1, column1;
    separateDot(mes, table1, column1); // Разделяем имя таблицы и колонки

    // Проверяем существование первой таблицы
    if (!tableExist(table1, jatab.head)) {
        cout << "This table ("<< table1 << ") not in list." << endl; // Выводим сообщение об ошибке
        return; // Завершаем выполнение функции
    }

    // Проверяем существование колонки в первой таблице
    if (!columnExist(table1, column1, jatab.head)) {
        cout << "This column ("<< column1 << ") not in table." << endl; // Выводим сообщение об ошибке
        return; // Завершаем выполнение функции
    }

    iss >> mes; // Читаем следующее слово команды (например, имя второй таблицы с колонкой)
    string table2, column2;
    separateDot(mes, table2, column2); // Разделяем имя таблицы и колонки

    // Проверяем существование второй таблицы
    if (!tableExist(table2, jatab.head)) {
        cout << "This table ("<< table2 << ") not in list." << endl; // Выводим сообщение об ошибке
        return; // Завершаем выполнение функции
    }

    // Проверяем существование колонки во второй таблице
    if (!columnExist(table2, column2, jatab.head)) {
        cout << "This column ("<< column2 << ") not in table." << endl; // Выводим сообщение об ошибке
        return; // Завершаем выполнение функции
    }

    string command2;
    cout << "<< "; // Запрашиваем ввод следующей команды
    getline(cin, command2); // Читаем команду
    istringstream iss2(command2); // Создаем поток для разбора команды
    string mes2;
    iss2 >> mes2; // Читаем первое слово команды (например, FROM)

    // Проверяем правильность команды
    if (mes2 != "FROM") {
        cout << "The data entered is incorrectly." << endl; 
        return; // Завершаем выполнение функции
    }

    iss2 >> mes2; // Читаем имя первой таблицы
    mes2.pop_back(); // Убираем последний символ (например, запятую)
    if (mes2 != table1) {
        cout << "The data entered is incorrectly." << endl; 
        return; // Завершаем выполнение функции
    }

    iss2 >> mes2; // Читаем имя второй таблицы
    if (mes2 != table2) {
        cout << "The data entered is incorrectly." << endl; 
        return; // Завершаем выполнение функции
    }

    string command3;
    cout << "<< "; // Запрашиваем ввод следующей команды
    getline(cin, command3); // Читаем команду
    istringstream iss3(command3); // Создаем поток для разбора команды
    string mes3;
    iss3 >> mes3; // Читаем первое слово команды (например, WHERE)

    // Если нет условия WHERE, выполняем кросс-джоин
    if (mes3 == "") {
        crossJoin(table1, column1, table2, column2, jatab); // Выполняем кросс-джоин
        return; 
    }
    else if (mes3 != "WHERE") {
        cout << "The data entered is incorrectly." << endl; 
        return; 
    }
    else {
        iss3 >> mes3; // Читаем условие
        string tableCond1, columnCond1;
        separateDot(mes3, tableCond1, columnCond1); // Разделяем имя таблицы и колонки в условии

        // Проверяем существование таблицы в условии
        if (!tableExist(tableCond1, jatab.head)) {
            cout << "This table ("<< tableCond1 << ") not in list." << endl; // Выводим сообщение об ошибке
            return; // Завершаем выполнение функции
        }

        // Проверяем существование колонки в условии
        if (!columnExist(tableCond1, columnCond1, jatab.head)) {
            cout << "This column ("<< columnCond1 << ") not in table." << endl; // Выводим сообщение об ошибке
            return; // Завершаем выполнение функции
        }

        iss3 >> mes3; // Читаем оператор сравнения (например, =)
        if (mes3 != "=") {
            cout << "The data entered is incorrectly." << endl; // Выводим сообщение об ошибке
            return; // Завершаем выполнение функции
        }

        iss3 >> mes3; // Читаем значение для сравнения
        if (isDot(mes3) == true) { // Проверяем, является ли значение ссылкой на другую колонку
            string tableCond2, columnCond2;
            separateDot(mes3, tableCond2, columnCond2); // Разделяем имя таблицы и колонки

            // Проверяем соответствие таблиц
            if (tableCond1 != table1) {
                cout << "The data entered is incorrectly." << endl; // Выводим сообщение об ошибке
                return; // Завершаем выполнение функции
            }
            if (tableCond2 != table2) {
                cout << "The data entered is incorrectly." << endl; // Выводим сообщение об ошибке
                return; // Завершаем выполнение функции
            }

            // Проверяем существование второй таблицы в условии
            if (!tableExist(tableCond2, jatab.head)) {
                cout << "This table ("<< tableCond2 << ") not in list." << endl; // Выводим сообщение об ошибке
                return; // Завершаем выполнение функции
            }

            // Проверяем существование колонки во второй таблице в условии
            if (!columnExist(tableCond2, columnCond2, jatab.head)) {
                cout << "This column ("<< columnCond2 << ") not in table." << endl; // Выводим сообщение об ошибке
                return; // Завершаем выполнение функции
            }

            iss3 >> mes3; // Читаем следующий оператор (AND/OR)
            if (mes3 != "AND" && mes3 != "OR") {
                condSingleT(jatab, tableCond1, tableCond2, columnCond1, columnCond2); // Обрабатываем условие
                crossJoinDepend(table1, column1, table2, column2, jatab); // Выполняем зависимый кросс-джоин
                size_t icsv = 1;
                while (true) {
                    // Формируем пути к результатам
                    fil::path csvPathRes1 = fil::current_path() / jatab.scheme / tableCond1 / ("res_" + to_string(icsv) + ".csv");
                    fil::path csvPathRes2 = fil::current_path() / jatab.scheme / tableCond2 / ("res_" + to_string(icsv) + ".csv");
                    // Проверяем существование файлов результата
                    if (!fil::exists(csvPathRes2) && !fil::exists(csvPathRes1)) break; // Если оба файла не существуют, выходим из цикла
                    if (fil::exists(csvPathRes1)) fil::remove(csvPathRes1); // Удаляем файл результата
                    if (fil::exists(csvPathRes2)) fil::remove(csvPathRes2); // Удаляем файл результата
                    icsv++; // Увеличиваем счетчик
                }
            }
            // Обработка условия AND
            if (mes3 == "AND") {
                iss3 >> mes3; // Читаем следующую часть условия
                string tableCond3, columnCond3;
                separateDot(mes3, tableCond3, columnCond3); // Разделяем имя таблицы и колонки

                // Проверяем соответствие таблиц
                if (tableCond3 != table1 && tableCond3 != table2) {
                    cout << "The data entered is incorrectly." << endl; // Выводим сообщение об ошибке
                    return; // Завершаем выполнение функции
                }

                // Проверяем существование таблицы в условии
                if (!tableExist(tableCond3, jatab.head)) {
                    cout << "This table ("<< tableCond3 << ") not in list." << endl; // Выводим сообщение об ошибке
                    return; // Завершаем выполнение функции
                }

                // Проверяем существование колонки в условии
                if (!columnExist(tableCond3, columnCond3, jatab.head)) {
                    cout << "This column ("<< columnCond3 << ") not in table." << endl; // Выводим сообщение об ошибке
                    return; // Завершаем выполнение функции
                }

                iss3 >> mes3; // Читаем оператор сравнения (например, =)
                if (mes3 != "=") {
                    cout << "The data entered is incorrectly." << endl; // Выводим сообщение об ошибке
                    return; // Завершаем выполнение функции
                }

                iss3 >> mes3; // Читаем значение для сравнения
                string sCond;
                // Убираем одинарные кавычки из значения
                for (size_t i = 0; i < mes3.size(); i++) {
                    if (mes3[i] != '\'') {
                        sCond += mes3[i]; // Составляем строку без кавычек
                    }
                }
                // Обрабатываем условие AND
                condAnd(jatab, tableCond1, tableCond2, columnCond1, columnCond2, tableCond3, columnCond3, sCond);
                crossJoinDepend(table1, column1, table2, column2, jatab); // Выполняем зависимый кросс-джоин
                size_t icsv = 1;
                while (true) {
                    // Формируем пути к результатам
                    fil::path csvPathRes1 = fil::current_path() / jatab.scheme / tableCond1 / ("res_" + to_string(icsv) + ".csv");
                    fil::path csvPathRes2 = fil::current_path() / jatab.scheme / tableCond2 / ("res_" + to_string(icsv) + ".csv");
                    // Проверяем существование файлов результата
                    if (!fil::exists(csvPathRes2) && !fil::exists(csvPathRes1)) break; // Если оба файла не существуют, выходим из цикла
                    if (fil::exists(csvPathRes1)) fil::remove(csvPathRes1); // Удаляем файл результата
                    if (fil::exists(csvPathRes2)) fil::remove(csvPathRes2); // Удаляем файл результата
                    icsv++; // Увеличиваем счетчик
                }
            }
            // Обработка условия OR
            if (mes3 == "OR") {
                iss3 >> mes3; // Читаем следующую часть условия
                string tableCond3, columnCond3;
                separateDot(mes3, tableCond3, columnCond3); // Разделяем имя таблицы и колонки

                // Проверяем соответствие таблиц
                if (tableCond3 != table1 && tableCond3 != table2) {
                    cout << "The data entered is incorrectly." << endl; // Выводим сообщение об ошибке
                    return; // Завершаем выполнение функции
                }
                if (!tableExist(tableCond3, jatab.head)) {
                    cout << "This table ("<< tableCond3 << ") not in list." << endl;
                    return;
                }
                if (!columnExist(tableCond3, columnCond3, jatab.head)) {
                    cout << "This column ("<< columnCond3 << ") not in table." << endl;
                    return;
                }
                iss3 >> mes3;
                if (mes3 != "=") {
                    cout << "The data entered is incorrectly." << endl;
                    return;
                }
                iss3 >> mes3;
                string sCond;
                for (size_t i = 0; i < mes3.size(); i++) {
                    if (mes3[i] != '\'') {
                        sCond += mes3[i];
                    }
                }
                condOr(jatab, tableCond1, tableCond2, columnCond1, columnCond2, tableCond3, columnCond3, sCond);
                crossJoinDepend(table1, column1, table2, column2, jatab);
                size_t icsv = 1;
                while (true) {
                    fil::path csvPathRes1 = fil::current_path() / jatab.scheme / tableCond1 / ("res_" + to_string(icsv) + ".csv");
                    fil::path csvPathRes2 = fil::current_path() / jatab.scheme / tableCond2 / ("res_" + to_string(icsv) + ".csv");
                    if (!fil::exists(csvPathRes2) && !fil::exists(csvPathRes1)) break;
                    if (fil::exists(csvPathRes1)) fil::remove(csvPathRes1);
                    if (fil::exists(csvPathRes2)) fil::remove(csvPathRes2);
                    icsv++;
                }
            }
        }

        else {
            if (tableCond1 != table1 && tableCond1 != table2) {
                cout << "The data entered is incorrectly." << endl;
                return;
            }
            string sCond;
            for (size_t i = 0; i < mes3.size(); i++) {
                if (mes3[i] != '\'') {
                sCond += mes3[i];
                }
            }
            iss3 >> mes3;
            if (mes3 != "AND" && mes3 != "OR") {
                if (tableCond1 == table1) {
                    condSingleS(jatab, tableCond1, table2, columnCond1, sCond);
                    crossJoinDepend(table1, column1, table2, column2, jatab);
                    size_t icsv = 1;
                    while (true) {
                        fil::path csvPathRes1 = fil::current_path() / jatab.scheme / tableCond1 / ("res_" + to_string(icsv) + ".csv");
                        fil::path csvPathRes2 = fil::current_path() / jatab.scheme / table2 / ("res_" + to_string(icsv) + ".csv");
                        if (!fil::exists(csvPathRes2) && !fil::exists(csvPathRes1)) break;
                        if (fil::exists(csvPathRes1)) fil::remove(csvPathRes1);
                        if (fil::exists(csvPathRes2)) fil::remove(csvPathRes2);
                        icsv++;
                    }
                }
                if (tableCond1 == table2) {
                    condSingleS(jatab, tableCond1, table1, columnCond1, sCond);
                    crossJoinDepend(table1, column1, table2, column2, jatab);
                    size_t icsv = 1;
                    while (true) {
                        fil::path csvPathRes1 = fil::current_path() / jatab.scheme / tableCond1 / ("res_" + to_string(icsv) + ".csv");
                        fil::path csvPathRes2 = fil::current_path() / jatab.scheme / table1 / ("res_" + to_string(icsv) + ".csv");
                        if (!fil::exists(csvPathRes2) && !fil::exists(csvPathRes1)) break;
                        if (fil::exists(csvPathRes1)) fil::remove(csvPathRes1);
                        if (fil::exists(csvPathRes2)) fil::remove(csvPathRes2);
                        icsv++;
                    }
                }
            }
            if (mes3 == "AND") {
                iss3 >> mes3;
                string tableCond2, columnCond2;
                separateDot(mes3, tableCond2, columnCond2);
                if (tableCond2 != table1) {
                    cout << "The data entered is incorrectly." << endl;
                    return;
                }
                if (!tableExist(tableCond2, jatab.head)) {
                    cout << "This table ("<< tableCond2 << ") not in list." << endl;
                    return;
                }
                if (!columnExist(tableCond2, columnCond2, jatab.head)) {
                    cout << "This column ("<< columnCond2 << ") not in table." << endl;
                    return;
                }
                iss3 >> mes3;
                if (mes3 != "=") {
                    cout << "The data entered is incorrectly." << endl;
                    return;
                }

                iss3 >> mes3;
                string tableCond3, columnCond3;
                separateDot(mes3, tableCond3, columnCond3);
                if (tableCond3 != table2) {
                    cout << "The data entered is incorrectly." << endl;
                    return;
                }
                if (!tableExist(tableCond3, jatab.head)) {
                    cout << "This table ("<< tableCond3 << ") not in list." << endl;
                    return;
                }
                if (!columnExist(tableCond3, columnCond3, jatab.head)) {
                    cout << "This column ("<< columnCond3 << ") not in table." << endl;
                    return;
                }

                condAnd(jatab, tableCond2, tableCond3, columnCond2, columnCond3, tableCond1, columnCond1, sCond);
                crossJoinDepend(table1, column1, table2, column2, jatab);
                size_t icsv = 1;
                while (true) {
                    fil::path csvPathRes1 = fil::current_path() / jatab.scheme / tableCond2 / ("res_" + to_string(icsv) + ".csv");
                    fil::path csvPathRes2 = fil::current_path() / jatab.scheme / tableCond3 / ("res_" + to_string(icsv) + ".csv");
                    if (!fil::exists(csvPathRes2) && !fil::exists(csvPathRes1)) break;
                    if (fil::exists(csvPathRes1)) fil::remove(csvPathRes1);
                    if (fil::exists(csvPathRes2)) fil::remove(csvPathRes2);
                    icsv++;
                }
            }
            if (mes3 == "OR") {
                iss3 >> mes3;
                string tableCond2, columnCond2;
                separateDot(mes3, tableCond2, columnCond2);
                if (tableCond2 != table1) {
                    cout << "The data entered is incorrectly." << endl;
                    return;
                }
                if (!tableExist(tableCond2, jatab.head)) {
                    cout << "This table ("<< tableCond2 << ") not in list." << endl;
                    return;
                }
                if (!columnExist(tableCond2, columnCond2, jatab.head)) {
                    cout << "This column ("<< columnCond2 << ") not in table." << endl;
                    return;
                }
                iss3 >> mes3;
                if (mes3 != "=") {
                    cout << "The data entered is incorrectly." << endl;
                    return;
                }

                iss3 >> mes3;
                string tableCond3, columnCond3;
                separateDot(mes3, tableCond3, columnCond3);
                if (tableCond3 != table2) {
                    cout << "The data entered is incorrectly." << endl;
                    return;
                }
                if (!tableExist(tableCond3, jatab.head)) {
                    cout << "This table ("<< tableCond3 << ") not in list." << endl;
                    return;
                }
                if (!columnExist(tableCond3, columnCond3, jatab.head)) {
                    cout << "This column ("<< columnCond3 << ") not in table." << endl;
                    return;
                }

                condOr(jatab, tableCond2, tableCond3, columnCond2, columnCond3, tableCond1, columnCond1, sCond);
                crossJoinDepend(table1, column1, table2, column2, jatab);
                size_t icsv = 1;
                while (true) {
                    fil::path csvPathRes1 = fil::current_path() / jatab.scheme / tableCond2 / ("res_" + to_string(icsv) + ".csv");
                    fil::path csvPathRes2 = fil::current_path() / jatab.scheme / tableCond3 / ("res_" + to_string(icsv) + ".csv");
                    if (!fil::exists(csvPathRes2) && !fil::exists(csvPathRes1)) break;
                    if (fil::exists(csvPathRes1)) fil::remove(csvPathRes1);
                    if (fil::exists(csvPathRes2)) fil::remove(csvPathRes2);
                    icsv++;
                }
            }
        }
    }
}