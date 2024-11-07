#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class clsMemory
{
protected:

public:
	vector<string>memory;

	clsMemory() {
		memory.resize(256 , "00");
	}

	string getCell(int address) {
		return memory[address];
	}

	void setCell(int address, string value) {
		memory[address] = value;
	}

    static void displayOnScreen(string value) {
        std::cout << "Screen Output: " << value << std::endl;
    }

};

