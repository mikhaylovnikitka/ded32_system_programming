//
// Created by Никита on 10/12/2018.
//

#ifndef INC_4_PROCESSOR_CODETOBYTE_H
#define INC_4_PROCESSOR_CODETOBYTE_H

#pragma once

#include "../Commands/GlobalVariables.h"
#include "../TextClass/TextClass.h"

#include <iostream>

std::string MySubStr(size_t l, size_t r, std::string& str) {
    return str.substr(l, r - l + 1);
}

void ConvertToByte(const std::string& filename) {
    std::vector<char*> code;
    std::string full_input = "../Samples/" + filename;
    std::string full_output = "../Samples/ByteCodeFromSamples/" + filename + "ByteCode";
    Text text(full_input.c_str(), code);
    std::ofstream fout(full_output.c_str());
    int instruction_pointer = 0;
    for (int iter = 0; iter < code.size(); ++iter){
        auto line = code[iter];
        if (line[0] == ':') { //it is label
            std::string label_name;
            int i = 2;
            while (line[i] != '\0') {
                label_name += line[i];
                ++i;
            }
            label_to_num[label_name] = instruction_pointer;
            continue;
        }
        instruction_pointer++;
        int i = 0;
        std::string command("");
        for (char symbol = line[0]; symbol != '\0'; symbol = line[++i]) {
            if (symbol == ' ') {
                break;
            }
            command += symbol;
        }
        if (command == "") {
            continue;
        }
        if (line[i] == '\0') {
            fout << gv::func_to_num.find(command)->second << std::endl;
            continue;
        }
        std::string parameter("");
        for (char symbol = line[++i]; symbol != '\0'; symbol = line[++i]) {
            parameter += symbol;
        }
        if (command != "") {
            fout << gv::func_to_num.find(command)->second << ' ' << parameter << std::endl;
        }
    }
}

void ExecuteByteCode(const std::string filename) {
    std::vector<char*> code;
    std::string full_input = "../Samples/ByteCodeFromSamples/" + filename + "ByteCode";
    Text text(full_input.c_str(), code);

    Memory mem{10};
    std::stack<int> s;
    mem.ip = 0;
    while (mem.ip != code.size()) {
        auto line = code[mem.ip];
        int i = 0;
        std::string command("");
        for (char symbol = line[0]; symbol != '\0'; symbol = line[++i]) {
            if (symbol == ' ') {
                break;
            }
            command += symbol;
        }

        mem.ip++; //next line

        if (command == "") {
            continue;
        }
        CommandInterface* a = gv::num_to_comm[std::stoi(command)];

        if (line[i] == '\0') {
            ArgsInterface args{};
            a->Execute(s, &mem, &args);
            continue;
        }
        std::string parameter("");
        for (char symbol = line[++i]; symbol != '\0'; symbol = line[++i]) {
            parameter += symbol;
        }

        if (a->GetLabel()[0] == 'j') {
            ArgsInterface args;
            args.label_name = parameter;
            a->Execute(s, &mem, &args);
            continue;
        }

        if (parameter[0] == '[') {
            ArgsInterface args(std::stoi(MySubStr(1, parameter.size() - 2, parameter)), true);
            a->Execute(s, &mem, &args);
        } else {
            if (parameter[0] == 'r') {
                ArgsInterface args{std::move(parameter)};
                a->Execute(s, &mem, &args);

            } else {
                ArgsInterface args(std::stoi(parameter), false);
                a->Execute(s, &mem, &args);
            }
        }
    }

}

#endif //INC_4_PROCESSOR_CODETOBYTE_H
