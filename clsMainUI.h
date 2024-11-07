#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class clsMainUI {
private:

    vector<string> Result;

public:

    void DisplayMenu() const {
        cout << "----- Vole Machine -----\n";
        cout << "1. Load program from file\n";
        cout << "2. Enter instructions manually\n";
        cout << "3. Exit\n";
    }

    string readString(string message) {
        cout << message;
        string sentence;
        getline(cin >> ws, sentence);
        return sentence;
    }

    bool isExistingFile(string file1) {
        ifstream file(file1);
        return (file.good());
    }

    void inputFileName() {
        string fileName;
        do {
            fileName = readString("please enter the name of instructions file: ");
        } while (!isExistingFile(fileName));
        ifstream file(fileName);
        string line;
        while (getline(file, line)) {
            Result.push_back(line);
        }
        file.close();
        cout << "Instructions file loaded successfully...\n";
    }

    void inputInstruction() {
        string instruction;
        int cnt = 0;
        cout << "Enter instructions (type 'C000' to stop):\n";

        while (instruction != "C000" && cnt < 122) {
            cout << "Enter instruction number " << cnt + 1 << ": ";
            getline(cin >> ws, instruction); // Using ws to ignore leading whitespace
            if (instruction != "C000") {
                Result.push_back(instruction);
                cnt++;
            }
        }

        if (cnt == 123) {
            cout << "Warning: Maximum instruction limit reached (123).\n";
        }

        Result.push_back("C000"); // Ensure termination signal is added
    }

    float ReadNumber(string message) {
        float number;
        cout << message;
        cin >> number;
        while (cin.fail()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid Number, Enter a valid one" << endl;
            cin >> number;
        }
        return number;
    }

    int readNumInRange(string message, int from, int to) {
        int num;
        do {
            num = ReadNumber(message);
        } while (round(num) != num || num > to || num < from);
        return num;
    }

    void getFileOrInstructions() {
        // Loop until the user decides to exit
        while (true) {
            DisplayMenu();
            int choice = readNumInRange("please enter you choice: ", 1, 3);
            switch (choice) {
                case 1:
                    inputFileName();
                    return;
                case 2:
                    inputInstruction();
                    return; // Exit after selection
                case 3:
                    cout << "Exiting program...\n";
                    return; // Exit the loop and method

            }
        }
        system("cls");
    }

    void PrintMemoryCells(vector<string>& memory) {
        cout << "Address | Valuen";
        cout << "---------------" << endl;
        for (int i = 0; i < 256; ++i) {
            cout << "0x" << clsALU::float_to_hex(i) << " | "
                 << memory[i] << endl;
        }
    }

    void PrintRegisterCells(vector<string> &registers) {
        cout << "Address | Valuen";
        cout << "---------------" << endl;
        for (int i = 0; i < 16; ++i) {
            cout << "0x" << clsALU::float_to_hex(i) << " | "
                 << registers[i] << endl;
        }
    }

    friend class clsCPU;
};