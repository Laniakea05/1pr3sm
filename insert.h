#pragma once
#include <iostream>
#include "parser.h"
#include "tableWork.h"
#include "rapidcsv.hpp"

using namespace std;
namespace fil = filesystem;
void cpColumns(const fil::path& fileIn, const fil::path& fileOut);
void insert(string command, JsonTable& jatab);