#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class clsRegistres
{

public:
	vector<string>registers;

	clsRegistres() {
		registers.resize(16 , "00");
	}

	string getCell(int address) {
		return registers[address];
	}

	void setCell(int address,string value) {
		registers[address] = value;
	}

    static void displayOnScreen(string value) {
        std::cout << "Screen Output: " << value << std::endl;
    }

};

