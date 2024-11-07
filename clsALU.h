#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <format>
#include "clsMemory.h"
#include "clsRegistres.h"
#include <bitset>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cctype>
using namespace std;

class clsALU
{
protected:


public:
    static int Hex_to_dec(string hex) {
        return stoi(hex, nullptr, 16);
    }

    static int Hex_to_dec(char hexChar) {
        hexChar = std::toupper(hexChar);
        if (hexChar >= '0' && hexChar <= '9') {
            return hexChar - '0';
        } else if (hexChar >= 'A' && hexChar <= 'F') {
            return hexChar - 'A' + 10;
        }
        return -1;
    }

    static string decToHex(int decimal) {
        stringstream ss;
        ss << uppercase << hex << decimal;
        return ss.str();
    }

    static float Hex_to_float(string hexstr) {
        int value= stoi(hexstr,nullptr,16);
        bitset<8> binaryPattern(value);
        int sign =(0x80 & value)>>7;
        int exponant = (0x70 & value) >> 4;
        float mant = (0x0F & value);
        mant *= pow(2, -4);
        int bias = 4;
        return pow(-1, sign) * mant * pow(2, exponant-bias);
    }

    static string float_to_hex(float flot) {
        string sign = (flot < 0) ? "1" : "0";
        if (sign[0] - '0') {
            flot = -flot;
        }
        int exponent = 0;
        while (flot >= 1.0) {
            flot /= 2.0;
            exponent++;
        }
        while (flot < 0.5 && flot>0) {
            flot *= 2;
            exponent--;
        }
        exponent += 4;
        string exponentBits = bitset<3>(exponent).to_string();
        string mantissaBits;
        for (int i = 1; i <= 4; i++) {
            flot *= 2;
            if (flot >= 1.0) {
                mantissaBits += '1';
                flot -= 1.0;
            }
            else {
                mantissaBits += '0';
            }
        }
        string ans = sign + exponentBits + mantissaBits;
        char hexLookup[16] = { '0', '1', '2', '3', '4', '5', '6', '7',
                               '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
        string hexAns = string(1, hexLookup[stoi(ans.substr(0, 4), nullptr, 2)]) +
                        string(1, hexLookup[stoi(ans.substr(4, 4), nullptr, 2)]);
        return hexAns;
    }

    static int Hex_to_dec_2sComplement(string hex) {
        int value = stoi(hex, nullptr, 16);
        int negative = (0x80 & value) >> 7;
        negative *= -128;
        return negative + (0x7F & value);
    }

    static string dec_to_hex_2sComplement(int dec) {
        int sign = (dec < 0) ? '1' : '0';
        if (sign -= '0') {
            sign *= 128;
            dec = 128+dec;
        }
        dec = (0x7F & dec) + sign;
        string hexadec = bitset<8>(dec).to_string();
        std::stringstream hexa;
        hexa << std::hex << std::uppercase << dec;
        return hexa.str();
    }

    string Add(string value1,  string value2, bool deci = true) {
        if (!deci) {
            return float_to_hex(Hex_to_float(value1) + Hex_to_float(value2));
        } else {
            return dec_to_hex_2sComplement(Hex_to_dec_2sComplement(value1) + Hex_to_dec_2sComplement(value2));
        }
    }

    string bitwise_or(string value1, string value2, bool deci = true) {
        return(decToHex(Hex_to_dec(value1) | Hex_to_dec(value2)));
    }

    string bitwise_and(string value1, string value2, bool deci = true) {
        return(decToHex(Hex_to_dec(value1) & Hex_to_dec(value2)));
    }

    string XOR(string value1, string value2, bool deci = true) {
        return(decToHex(Hex_to_dec(value1) ^ Hex_to_dec(value2)));
    }

    static bool isValid_Address(string address) {
        for (char c : address) {
            if ( !((toupper(c) - 'A' >= 0 && toupper(c) - 'A' <= 25) || (toupper(c) - '0' >= 0 && toupper(c) - '0' <= 9)) ) {
                return false;
            }
        }
        return true;
    }
};