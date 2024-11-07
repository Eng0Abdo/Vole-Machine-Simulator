#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <cstdlib>
using namespace std;

class ALU {
private:
    map<char, int> hexaMap;

public:
    ALU() {
        hexaMap['0'] = 0; hexaMap['1'] = 1; hexaMap['2'] = 2; hexaMap['3'] = 3;
        hexaMap['4'] = 4; hexaMap['5'] = 5; hexaMap['6'] = 6; hexaMap['7'] = 7;
        hexaMap['8'] = 8; hexaMap['9'] = 9; hexaMap['A'] = 10; hexaMap['B'] = 11;
        hexaMap['C'] = 12; hexaMap['D'] = 13; hexaMap['E'] = 14; hexaMap['F'] = 15;
    }
    int hexaToDecimal(string hexNum) {
        int decimal = 0;
        int power = hexNum.length() - 1;
        for (char c : hexNum) {
            c = toupper(c);
            decimal += hexaMap[c] * pow(16, power);
            power--;
        }
        return decimal;
    }

    string decimalToHexa(int decimal) {
        if (decimal == 0) return "0";

        std::string hex = "";
        const char hexDigits[] = "0123456789ABCDEF";

        while (decimal > 0) {
            int remainder = decimal % 16;
            hex = hexDigits[remainder] + hex;
            decimal /= 16;
        }

        return hex;
    }

    string decToBinary(int decimal_number) {
        if (decimal_number == 0) {
            return "0";
        }
        if (decimal_number == 1) {
            return "1";
        }
        return decToBinary(decimal_number / 2) + to_string(decimal_number % 2);
    }


    string binaryToTwosComplement(const string& binary) {
        string result = binary;
        int n = result.size();

        int i = n - 1;
        while (i >= 0 && result[i] == '0') {
            --i;
        }

        if (i == -1) {
            return result;
        }

        for (int j = 0; j < i; ++j) {
            result[j] = (result[j] == '0') ? '1' : '0';
        }

        return result;
    }

    unsigned int binaryToDecimal(const string& binary) {
        unsigned int decimalValue = 0;
        int power = 0;

        for (int i = binary.size() - 1; i >= 0; --i) {
            if (binary[i] == '1') {
                decimalValue += pow(2, power);
            }
            power++;
        }

        return decimalValue;
    }

    string addBinary(const string& a, const string& b) {
        string result = "";
        int carry = 0;
        int i = a.size() - 1;
        int j = b.size() - 1;

        while (i >= 0 || j >= 0 || carry == 1) {
            int sum = carry;

            if (i >= 0) {
                sum += a[i] - '0';
                i--;
            }

            if (j >= 0) {
                sum += b[j] - '0';
                j--;
            }
            result += (sum % 2) + '0';
            carry = sum / 2;
        }

        reverse(result.begin(), result.end());
        return result;
    }

    string binaryToHex(const string& binary) {
        string truncatedBinary = binary.substr(max(0, (int)binary.size() - 8));

        unsigned int decimalValue = binaryToDecimal(truncatedBinary);
        stringstream hexStream;
        hexStream << hex << decimalValue;
        string hexResult = hexStream.str();

        transform(hexResult.begin(), hexResult.end(), hexResult.begin(), ::toupper);
        if (hexResult.size() < 2) {
            hexResult = string(2 - hexResult.size(), '0') + hexResult;
        }
        else if (hexResult.size() > 2) {
            hexResult = hexResult.substr(hexResult.size() - 2);
        }

        return hexResult;
    }

    float convertToFloatBinary(const string& hexNum) {
        int decimalValue = hexaToDecimal(hexNum);
        string binary = decToBinary(decimalValue);

        // Ensure binary string is 8 bits long
        while (binary.length() < 8) {
            binary = "0" + binary;
        }

        // Extract the parts
        int sign = (binary[0] == '1') ? -1 : 1;  // 1 if negative, else positive
        int exponent = stoi(binary.substr(1, 3), nullptr, 2);  // 3-bit exponent
        int mantissa = stoi(binary.substr(4, 4), nullptr, 2);  // 4-bit mantissa

        // Apply the exponent bias (bias is 4)
        exponent -= 4;

        // Calculate the mantissa as a normalized fraction
        float normalizedMantissa = 1.0 + mantissa / 16.0;  // 1.xxxxx format

        // Calculate the final floating-point number
        float result = sign * normalizedMantissa * pow(2, exponent);
        return result;
    }

    string converttohexa(const float& float_number) {
        int signBit = (float_number < 0) ? 1 : 0;

        // Convert the absolute value to handle sign separately
        float absValue = fabs(float_number);

        // Calculate the exponent and mantissa
        int exponent = 4;  // Start with a bias of 4
        float mantissa = absValue;

        // Normalize the mantissa to be within the range [1.0, 2.0)
        while (mantissa >= 2.0) {
            mantissa /= 2.0;
            exponent++;
        }
        while (mantissa < 1.0 && exponent > 0) {
            mantissa *= 2.0;
            exponent--;
        }

        // Convert exponent and mantissa to binary
        string exponentBinary = decToBinary(exponent).substr(0, 3);  // 3-bit exponent
        int mantissaBits = (mantissa - 1.0) * 16;  // 4-bit fractional part
        string mantissaBinary = decToBinary(mantissaBits).substr(0, 4);

        // Combine sign, exponent, and mantissa into a single binary string
        string finalBinary = to_string(signBit) + exponentBinary + mantissaBinary;

        // Convert binary to decimal, then to hex
        int finalDecimal = binaryToDecimal(finalBinary);
        string finalHex = decimalToHexa(finalDecimal);

        // Pad to two hex digits if necessary
        if (finalHex.size() < 2) {
            finalHex = "0" + finalHex;
        }

        return finalHex;
    }
};

class Register {
private:
    vector<string> registerMemory;
public:
    Register() {
        for (int i = 0; i < 16; i++) {
            registerMemory.push_back("00");
        }
    }

    string getCell(int address) {
        return registerMemory[address];
    }

    void setCell(int address, string value) {
        registerMemory[address] = value;
    }

    void outputState() {
        cout << "Registers state:" << endl;
        for (int i = 0; i < 16; i++) {
            cout << "Register " << i << " Valu is : " << registerMemory[i] << endl;
        }
        cout << endl;
    }

};



class Memory {
private:
    map<int, string> memory;
public:
    Memory() {
        for (int i = 0; i < 256; i++) {
            memory[i] = "00";
        }
    }

    string getCell(int address) {
        return memory[address];
    }


    void setCell(int address, string val) {
        memory[address] = val;
    }
};

class CU {
private:
    ALU alu;
public:
    void load(int idxReg, int intMem, Register& reg, Memory& mem) {
        reg.setCell(idxReg, mem.getCell(intMem));
        reg.outputState();
    }

    void load(int idxReg, string val, Register& reg) {
        reg.setCell(idxReg, val);
        reg.outputState();
    }

    void store(int idxReg, int idxMem, Register& reg, Memory& mem) {
        mem.setCell(idxMem, reg.getCell(idxReg));
        reg.outputState();
    }

    void move(int idxReg1, int idxReg2, Register& reg) {
        reg.setCell(idxReg2, reg.getCell(idxReg1));
        reg.outputState();
    }

    void jump(int idxReg, int idxMem, Register& reg, int& PC) {
        if (reg.getCell(idxReg) == reg.getCell(0)) {
            PC = idxMem;
            reg.outputState();
        }
    }

    void add(int idx1, int idx2, int idx3, Register& reg) {
        string hex1 = reg.getCell(idx2);
        string hex2 = reg.getCell(idx3);

        string binary1 = alu.decToBinary(alu.hexaToDecimal(hex1));
        string binary2 = alu.decToBinary(alu.hexaToDecimal(hex2));

        string sum = alu.addBinary(binary1, binary2);

        if (sum.size() > 8) {
            sum = sum.substr(sum.size() - 8);
        }
        string hexResult = alu.binaryToHex(sum);
        reg.setCell(idx1, hexResult);
        reg.outputState();
    }
    void addFloat(int idx1, int idx2, int idx3, Register& reg, ALU& alu) {
        string num1 = reg.getCell(idx2);
        string num2 = reg.getCell(idx3);
        float float1 = alu.convertToFloatBinary(num1);
        float float2 = alu.convertToFloatBinary(num2);
        float sum = float1 + float2;
        string finaloutput = alu.converttohexa(sum);


        reg.setCell(idx1, finaloutput);
        reg.outputState();
    }

};

class CPU {
private:
    int programCounter;
    string instructionRegister;
    char instruction;
    string data;
    Register regist;
    ALU alu;
    CU cu;
public:
    CPU() : programCounter{ 0 } {}

    void fetch(Memory& mem) {
        if (programCounter < 256) {
            string instructionPair = mem.getCell(programCounter);
            instruction = instructionPair[0];
            instructionRegister = instructionPair[1];
            data = mem.getCell(++programCounter);
            programCounter++;
        }
        else {
            cerr << "Error: Program counter out of memory range." << endl;
            exit(1);
        }
    }


    void decodeAndExecute(Memory& mem) {
        if (instruction == '1') {
            cu.load(alu.hexaToDecimal(instructionRegister), alu.hexaToDecimal(data), regist, mem);
        }
        else if (instruction == '2') {
            cu.load(alu.hexaToDecimal(instructionRegister), data, regist);
        }
        else if (instruction == '3') {
            cu.store(alu.hexaToDecimal(instructionRegister), alu.hexaToDecimal(data), regist, mem);
        }
        else if (instruction == '4') {
            string r1 = "";
            string r2 = "";
            r1 += data[0];
            r2 += data[1];
            cu.move(alu.hexaToDecimal(r1), alu.hexaToDecimal(r2), regist);
        }
        else if (instruction == '5') {
            string r1 = "";
            string r2 = "";
            r1 += data[0];
            r2 += data[1];
            cu.add(alu.hexaToDecimal(instructionRegister), alu.hexaToDecimal(r1), alu.hexaToDecimal(r2), regist);
        }
        else if (instruction == '6') {
            // Adding the floating-point addition operation
            string r1 = "";
            string r2 = "";
            r1 += data[0];
            r2 += data[1];
            cu.addFloat(alu.hexaToDecimal(instructionRegister), alu.hexaToDecimal(r1), alu.hexaToDecimal(r2), regist, alu);
        }
        else if (instruction == 'B' || instruction == 'b') {
            cu.jump(alu.hexaToDecimal(instructionRegister), alu.hexaToDecimal(data), regist, programCounter);
            cout << "cell number = " << alu.hexaToDecimal(data) << endl;
        }
        else if (instruction == 'C' || instruction == 'c') {
            cout << " End of simulation ^_^\n";
            exit(0);
        }
    }
};



class Machine {
private:
    CPU processor;
    Memory memory;
    vector<string> programData;

public:

    void loadProgramFile() {
        string filename;
        ifstream file;

        // Keep prompting for a filename until a valid file is found
        while (true) {
            cout << "Enter the filename of the program to load: ";
            cin >> filename;
            file.open(filename);

            if (!file.is_open()) {
                cerr << "Error: Could not open file " << filename << ". Please enter a valid filename." << endl;
            }
            else {
                break;
            }
        }

        string content;
        stringstream buffer;
        buffer << file.rdbuf();
        content = buffer.str();
        file.close();

        // Remove whitespaces
        content.erase(remove_if(content.begin(), content.end(), ::isspace), content.end());

        // Validate that content contains only hexadecimal characters
        for (char ch : content) {
            if (!isxdigit(ch)) {
                cerr << "Error: File contains non-hexadecimal characters." << endl;
                return;
            }
        }

        // Load program data into memory
        programData.clear();
        for (size_t i = 0; i < content.size(); i += 2) {
            string two_bits = content.substr(i, 2);
            programData.push_back(two_bits);
        }

        for (size_t i = 0; i < programData.size(); ++i) {
            memory.setCell(i, programData[i]);
        }

        cout << "File loaded and validated successfully." << endl;
    }

    void outputState() {
        cout << "Machine state:" << endl;
        for (int i = 0; i < 256; i += 2) {
            cout << memory.getCell(i) << " " << memory.getCell(i + 1) << endl;

        }
        cout << endl;
    }
    void run() {
        outputState();
        while (true) {
            processor.fetch(memory);
            processor.decodeAndExecute(memory);
        }
    }
};