#include <iostream>
#include "insert.h"

// Функция для копирования колонок из одного файла в другой
void cpColumns(const fil::path& fileIn, const fil::path& fileOut) {
    string columns; // Переменная для хранения колонок
    ifstream file(fileIn); // Открытие входного файла
    if (!file.is_open()) {
        cout << "The file not open" << fileIn << endl; 
        return;
    }
    file >> columns; // Чтение колонок из файла
    file.close(); // Закрытие входного файла

    ofstream fileO(fileOut); // Открытие выходного файла
    if (!fileO.is_open()) {
        cout << "The file not open" << fileOut << endl; 
        return;
    }
    fileO << columns << endl; // Запись колонок в выходной файл
    fileO.close(); // Закрытие выходного файла
}

// Функция для вставки данных в таблицу
void insert(string command, JsonTable& jatab) {
    istringstream iss(command); // Создание потока для разбора команды
    string mes;
    iss >> mes; // Чтение первого слова команды
    iss >> mes; // Чтение второго слова команды
    if (mes != "INTO") { // Проверка на наличие ключевого слова "INTO"
        cout << "The data entered incorrect" << endl;
        return;
    }

    iss >> mes; // Чтение имени таблицы
    string table = mes; // Сохранение имени таблицы
    if (!tableExist(table, jatab.head)) { // Проверка существования таблицы
        cout << "Table " << table << " not create" << endl;
        return;
    }
    if (isLocked(table, jatab.scheme)) { // Проверка на блокировку таблицы
        cout << "The file is blocked for editing" << endl;
        return;
    }

    iss >> mes; // Чтение слова "VALUES"
    if (mes != "VALUES") { // Проверка на наличие ключевого слова "VALUES"
        cout << "The data entered incorrect" << endl;
        return;
    }
    
    string values; // Переменная для хранения значений
    while (iss >> mes) { // Чтение значений
        values += mes; // Сбор значений в строку
    }
    
    // Проверка на корректность формата значений
    if (values.front() != '(' || values.back() != ')') {
        cout << "The data entered incorrect" << endl;
        return;
    }
    
    // Проверка на наличие кавычек вокруг строковых значений
    for (size_t i = 1; i < values.size(); i++) {
        if (values[i] == '\'') {
            i++;
            while (values[i] != '\'') {
                i++;
                if (values[i] == ',') {
                    cout << "Not all values are marked with quotation marks." << endl;
                    return;
                }
            }
        }
    }

    locker(table, jatab.scheme); // Блокировка таблицы для редактирования
    fil::path pkPath = fil::current_path() / jatab.scheme / table / (table + "_pk_sequence.txt"); // Путь к файлу последовательности первичного ключа
    ifstream file(pkPath); // Открытие файла последовательности
    if (!file.is_open()) {
        cout << "The file not open" << pkPath << endl;
        return;
    }
    int primKey; // Переменная для первичного ключа
    file >> primKey; // Чтение текущего значения первичного ключа
    file.close();
    primKey++; // Увеличение первичного ключа на 1

    ofstream fileO(pkPath); // Открытие файла для записи обновленного первичного ключа
    if (!fileO.is_open()) {
        cout << "The file not open" << pkPath << endl;
        return;
    }
    fileO << primKey; // Запись нового значения первичного ключа
    fileO.close();

    int csvCount = 1; // Счетчик для CSV файлов
    while (true) {
        // Формирование пути к текущему CSV файлу
        fil::path csvPath = fil::current_path() / jatab.scheme / table / (to_string(csvCount) + ".csv");
        ifstream file(csvPath); // Открытие текущего CSV файла
        if (!file.is_open()) { // Если файл не существует, создаем его
            ofstream fileO(csvPath);
            if (!fileO.is_open()) {
                cout << "The file not open" << csvPath << endl;
                return;
            }
            fileO.close();
        } else {
            file.close(); // Закрываем файл, если он существует
        }
        
        rapidcsv::Document csvdoc(csvPath.string()); // Создание документа CSV
        if (csvdoc.GetRowCount() < jatab.rowsCount) { // Проверка количества строк в файле
            break; // Выход из цикла, если количество строк меньше лимита
        }
        csvCount++; // Увеличение счетчика CSV файлов
    }

    // Путь к первому и последнему CSV файлам
    fil::path csvFirstPath = fil::current_path() / jatab.scheme / table / "1.csv";
    fil::path csvEndPath = fil::current_path() / jatab.scheme / table / (to_string(csvCount) + ".csv");
    rapidcsv::Document doc(csvEndPath.string()); // Создание документа для последнего CSV файла
    if (doc.GetRowCount() == 0) { // Если файл пустой, копируем колонки
        cpColumns(csvFirstPath, csvEndPath);
    }
    
    // Открытие последнего CSV файла для добавления данных
    ofstream csvFile(csvEndPath, ios::app);
    if (!csvFile.is_open()) {
        cout << "The file not open" << csvEndPath << endl;
        return;
    }
    
    // Запись первичного ключа и значений в CSV файл
    csvFile << to_string(primKey) << ",";
    for (size_t i = 1; i < values.size(); i++) {
        if (values[i] == '\'') {
            i++;
            while (values[i] != '\'') {
                csvFile << values[i]; // Запись значения
                i++;
            }
            if (values[i + 1] != ')') {
                csvFile << ","; // Запись запятой, если это не последний элемент
            } else {
                csvFile << endl; // Переход на новую строку
            }
        }
    }
    csvFile.close(); // Закрытие CSV файла
    locker(table, jatab.scheme); // Снятие блокировки с таблицы
}
