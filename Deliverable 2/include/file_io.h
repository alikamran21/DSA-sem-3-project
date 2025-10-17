#ifndef FILE_IO_H
#define FILE_IO_H

#include <iostream>
#include <fstream>
#include <string>
#include "user_action.h"

using namespace std;

// Handles saving and reading user actions from text files
class FileIO {
public:
    // Save one user action (append to file)
    static bool saveAction(const UserAction& action, const string& filename) {
        ofstream file(filename, ios::app);
        if (!file.is_open()) {
            cerr << "Error: Unable to open file " << filename << endl;
            return false;
        }

        file << action.timestamp << ", "
             << action.userID << ", "
             << action.action << ", "
             << action.processName << ", "
             << action.duration << ", "
             << action.nextAction << ", "
             << action.status << endl;

        file.close();
        return true;
    }

    // Read all data from a file and display it (for testing)
    static void readFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Unable to open file " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
};

#endif
