#pragma once
#include <iostream>
#include <iomanip>
#include "parser.h"
#include "insert.h"
#include "rapidcsv.hpp"

using namespace std;
namespace fil = filesystem;
bool isLocked(string table, string scheme);
void locker(string table, string scheme);
bool tableExist(string table, Tables* head);
bool columnExist(string table, string columnName, Tables* head);
void separateDot(string message, string& table, string& column);
void crossJoin(string table1, string column1, string table2, string column2, JsonTable& jatab);
void crossJoinDepend(string table1, string column1, string table2, string column2, JsonTable& jatab);
void condSingleT(JsonTable& jatab, string tableCond1, string tableCond2, string columnCond1, string columnCond2);
void condSingleS(JsonTable& jatab, string tableCond1, string table, string columnCond1, string sCond);
void condAnd(JsonTable& jatab, string tableCond1, string tableCond2, string columnCond1, string columnCond2, string tableCond3, string columnCond3, string sCond);
void condOr(JsonTable& jatab, string tableCond1, string tableCond2, string columnCond1, string columnCond2, string tableCond3, string columnCond3, string sCond);
bool isDot(string message);
bool isDependenceTables(string table, string column, string tableCond, string columnCond, size_t csvNum, size_t row, JsonTable& jatab);
bool isDependenceString(string table, string column, string sCond, size_t csvNum, size_t row, JsonTable& jatab);