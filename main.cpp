#include <iostream>
#include "insert.h"
#include "delete.h"
#include "parser.h"
#include "tableWork.h"
#include "select.h"

using namespace std;

int main() {
    //system("chcp 65001"); 
    JsonTable jatab;
    parser(jatab);
    string command;
    while (true) {
        cout << "<< ";
        getline(cin, command);
        if (command == "") {
            continue;
        }

        istringstream iss(command);
        string firstMessage;
        iss >> firstMessage;

        if (firstMessage == "INSERT") {
            insert(command, jatab);
        }
        if (firstMessage == "DELETE") {
            deleteRows(command, jatab);
        }
        if (firstMessage == "SELECT") {
            select(command, jatab);
        }
        if (firstMessage == "EXIT") {
            return 1;
        }
    }
}

