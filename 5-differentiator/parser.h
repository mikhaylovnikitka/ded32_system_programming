//
// Created by Никита on 27/11/2018.
//

#ifndef INC_5_DIFFERENTIATOR_PARSER_H
#define INC_5_DIFFERENTIATOR_PARSER_H

#include <iostream>
#include "tree.h"

#define ASSERT_TRUE(x) assert((x))

bool IsNumber(std::string& str) {
    for (int i = 0; i < str.size(); ++i) {
        if (!isdigit(str[i]) && str[i] != '.') {
            return false;
        }
    }
    return true;
}

std::string MySubStr(int l, int r, std::string& str) {
    ASSERT_TRUE(r <= str.size());
    ASSERT_TRUE(l >= 0);
    return str.substr(l, r - l + 1);
}

node_t Parser(int l, int r, std::string& str) {
    std::cout << str[l] << ' ' << str[r] << std::endl;
    std::cout << MySubStr(l, r, str) << std::endl;
    if (str[l] == '(' && str[r] == ')') {
        std::cout << "[INFO] Resizing..." << std::endl;
        l++;
        r--;
    }
    int bracesBalance = 0;
    for (int i = l; i <= r; ++i) {
        if (str[i] == '(') {
            bracesBalance++;
        }
        if (str[i] == ')') {
            bracesBalance--;
        }
        if ((str[i] == '*' || str[i] == '+' || str[i] == '-' || str[i] == '/' || str[i] == '^') && bracesBalance == 0) {
            std::cout << str[i] << ' ' << l << ' ' << r << ' ' << std::endl;
            auto left = Parser(l, i-1, str);
            auto right = Parser(i+1, r, str);
            NodeTypeName typeName;
            switch (str[i]) {
                case '*' : typeName = MUL; break;
                case '+' : typeName = ADD; break;
                case '-' : typeName = SUB; break;
                case '/' : typeName = DIV; break;
                case '^' : typeName = POW; break;
                default: typeName = SUB;
            }
            return CreateBinaryOperation(left, right, typeName);
        }
    }

    //Check SIN, COS, LOG
    std::string example = "sin(x)";
    std::cout << MySubStr(l, r, str) << std::endl;
    if (r - l + 1 >= example.size()) {
        if (MySubStr(l, l + 2, str) == "sin") {
            auto argument = Parser(l + 3, r, str);
            return CreateUnaryOperation(argument, SIN);
        }
        if (MySubStr(l, l + 2, str) == "cos") {
            auto argument = Parser(l + 3, r, str);
            return CreateUnaryOperation(argument, COS);
        }
        if (MySubStr(l, l + 2, str) == "log") {
            auto argument = Parser(l + 3, r, str);
            return CreateUnaryOperation(argument, LOG);
        }
    }

    //Check UnaryMinus
    example = "-(x)";
    if (r - l + 1 >= example.size()) {
        if (str[l] == '-') {
            auto argument = Parser(1, r, str);
            return CreateUnaryOperation(argument, UNARY_MINUS);
        }
    }

    //Check single Variable and Number
    example = "x";
    if (r - l + 1 >= example.size()) {
        auto exp = MySubStr(l, r, str);
        std::cout << exp << std::endl;
        if (IsNumber(exp)) {
            std::cout << "[INFO] Creating Number " << exp << std::endl;
            return CreateNumber(std::stoi(exp));
        } else {
            std::cout << "[INFO] Creating Variable " << exp <<  std::endl;
            return CreateVariable(1);
        }
    }

    return nullptr;
}

#endif //INC_5_DIFFERENTIATOR_PARSER_H
