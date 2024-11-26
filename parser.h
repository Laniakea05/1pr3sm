#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;
namespace fil = filesystem;

struct Node {
    string data; //Название столбца
    Node* next;
};

struct Tables {
    string name; //Название таблицы
    Node* column;
    Tables* next; // Следующая таблица
};

struct JsonTable { //Структура таблицы с json
    int rowsCount; // количество строк
    string scheme; // имя схемы
    Tables* head;
};

void removeDir(const fil::path& dirPath);
void createFiles(const fil::path& schemePath, const json& jsonStruct, JsonTable& jatable);
void parser(JsonTable& jatab);