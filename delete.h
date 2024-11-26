#pragma once
#include <iostream>
#include "parser.h"
#include "tableWork.h"
#include "rapidcsv.hpp"

using namespace std;
namespace fil = filesystem;
void deleteRows(string command, JsonTable& jatab);