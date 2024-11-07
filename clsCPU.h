#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "clsMemory.h"
#include "clsRegistres.h"
#include "clsCU.h"
#include "clsALU.h"
#include "clsMainUI.h"
#include <regex>

using namespace std;

class clsCPU {
private:
    clsMainUI _Main;
    clsRegistres _Register;
    clsMemory _Memory;
    vector<string> _Result;
    int count, counter;

    void TakeTheResults() {
        _Main.getFileOrInstructions();
        _Result = _Main.Result;
    }

    bool isValidInstruction(string &instruction) {

        regex validInstructionPattern("^[1-9A-Da-d][0-9A-Fa-f]{3}$");
        if (!regex_match(instruction, validInstructionPattern)) {
            return false;
        }

        string instructionUpper = instruction;

        for (char &c: instructionUpper) {
            c = toupper(c);
        }
        return true;

    }

public:

    void StoreInTheMemory() {
        TakeTheResults();

        count = _Main.readNumInRange("please enter the initial programe counter [1,254]", 1, 254);
        counter = count;
        string Line1;
        string Line2;

        for (auto &i: _Result) {
            if (counter >= 254) {
                break;
            }
            if (isValidInstruction(i)) {
                Line1 = i.substr(0, 2);
                Line2 = i.substr(2, 4);
                _Memory.setCell(counter, Line1);
                counter++;
                _Memory.setCell(counter, Line2);
                counter++;
            } else {
                cout << "\033[1;31mThe instruction " << (counter + 1) / 2 << " is invalid\n \033[0m";
                continue;
            }
        }

        counter = count;
    }

    void DoConstructions() {

        bool IsEnd = false;
        char instruction;

        while ((!IsEnd) && (counter <= min(254, (int) _Result.size() * 2))) {
            clsALU alu1;
            instruction = _Memory.getCell(counter)[0];

            if (instruction == '1') {

                clsCU::load((clsALU::Hex_to_dec(_Memory.getCell(counter)[1])),
                            clsALU::Hex_to_dec(_Memory.getCell(counter + 1)),
                            _Register, _Memory);
            }

            else if (instruction == '2') {
                clsCU::load(clsALU::Hex_to_dec(_Memory.getCell(counter)[1]),
                            _Memory.getCell(counter + 1), _Register);
            }

            else if (instruction == '3') {
                clsCU::store(clsALU::Hex_to_dec(_Memory.getCell(counter)[1]),
                             clsALU::Hex_to_dec(_Memory.getCell(counter + 1)),
                             _Register, _Memory);
            }

            else if (instruction == '4') {
                clsCU::move(clsALU::Hex_to_dec(_Memory.getCell(counter + 1)[0]),
                            clsALU::Hex_to_dec(_Memory.getCell(counter + 1)[1]),
                            _Register);
            }

            else if (instruction == '5') {
                _Register.setCell(clsALU::Hex_to_dec(_Memory.getCell(counter)[1]) ,
                                  alu1.Add(_Register.getCell(clsALU::Hex_to_dec(_Memory.getCell(counter + 1)[0]) ),
                                  _Register.getCell(clsALU::Hex_to_dec(_Memory.getCell(counter + 1)[1])) ) );
            }

            else if (instruction == '6') {
                _Register.setCell(clsALU::Hex_to_dec(_Memory.getCell(counter)[1]) ,
                                  alu1.Add(_Register.getCell(clsALU::Hex_to_dec(_Memory.getCell(counter + 1)[0]) ),
                                   _Register.getCell(clsALU::Hex_to_dec(_Memory.getCell(counter + 1)[1])) , false)  );
            }

            else if(instruction == 'B')
            {
                if(clsCU::jump(clsALU::Hex_to_dec(_Memory.getCell(counter)[1])
                        ,clsALU::Hex_to_dec(_Memory.getCell(counter + 1)) , _Register , counter))
                {
                    continue;
                }
            }

            else if (instruction == '7') {
                _Register.setCell(clsALU::Hex_to_dec(_Memory.getCell(counter)[1]),
                                  alu1.bitwise_or(_Register.getCell(clsALU::Hex_to_dec(_Memory.getCell(counter + 1)[0])),
                                                  _Register.getCell(clsALU::Hex_to_dec(_Memory.getCell(counter + 1)[1]))));
            }

            else if (instruction == '8') {
                _Register.setCell(clsALU::Hex_to_dec(_Memory.getCell(counter)[1]),
                                  alu1.bitwise_and(_Register.getCell(clsALU::Hex_to_dec(_Memory.getCell(counter + 1)[0])),
                                                   _Register.getCell(clsALU::Hex_to_dec(_Memory.getCell(counter + 1)[1])), false));
            }

            else if (instruction == 'A')
            {
                clsCU::rotateRight(reinterpret_cast<string &>(_Memory.getCell(counter)[1]), clsALU::Hex_to_dec(_Memory.getCell(counter + 1)[1]) );
            }

            else if (instruction == '9') {
                _Register.setCell(clsALU::Hex_to_dec(_Memory.getCell(counter)[1]),
                                  alu1.bitwise_and(_Register.getCell(clsALU::Hex_to_dec(_Memory.getCell(counter + 1)[0])),
                                                   _Register.getCell(clsALU::Hex_to_dec(_Memory.getCell(counter + 1)[1])), false));
            }

            else if(instruction == 'D')
            {
                clsCU::jump(clsALU::Hex_to_dec(_Memory.getCell(counter)[1])
                        ,clsALU::Hex_to_dec(_Memory.getCell(counter + 1)) , _Register , counter , false);
            }

            else if(instruction == 'C')
            {
                IsEnd = true;
            }

            counter +=2;
        }
    }

    void PrintMemoryCells()
    {
        cout << "Memory\n" << endl;
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 16; ++j) {
                cout << setw(3) << " | " << _Memory.memory[i* 16 + j] << " ";
            }
            cout << endl;
        }
        cout << "__________________________________________________\n";
        cout << endl << '\n';
    }

    void PrintRegisterCells()
    {
        cout << endl << "\nAddress | Value\n";
        cout << "---------------" << endl;
        for (int i = 0; i < 16; ++i) {
            cout << "0x" << (clsALU::decToHex(i)) << setw(4)  << "    | " << _Register.registers[i] << endl;
        }
    }

    void Start()
    {
        StoreInTheMemory();

        DoConstructions();

        cout << "__________________________________________________\n";
        PrintMemoryCells();
        cout << "__________________________________________________\n";

        cout << "__________________________________________________\n";
        PrintRegisterCells();
        cout << "__________________________________________________\n";

    }

};