////
//// Created by Никита on 27/11/2018.
////

#ifndef INC_5_DIFFERENTIATOR_PARSER_H
#define INC_5_DIFFERENTIATOR_PARSER_H

#include <string>
#include <exception>
#include "Tree.h"

class ParserException: public std::exception {
};

class Parser {
public:

    Parser(const std::string& text):
            text(text), pointer(0) {}

    node_t parse();

private:

    void throwIf(bool condition) {
        if (condition) {
            throw ParserException();
        }
    }

    node_t parseExpression();
    node_t parseProduct();
    node_t parsePower();
    node_t parsePrimary();
    node_t parseNumber();

    const std::string text;
    size_t pointer;
};

#pragma once

#include "Parser.h"

node_t Parser::parse() {
    node_t result = parseExpression();
    throwIf(text[pointer] != '\0');
    return result;
}

node_t Parser::parseExpression() {
    auto result = parseProduct();
    while (text[pointer] == '+' || text[pointer] == '-') {
        NodeTypeName nodeType = (text[pointer] == '+') ? NodeTypeName::ADD : NodeTypeName::SUB;
        ++pointer;
        auto right = parseProduct();
        auto new_result = std::make_shared<Node>(nodeType);
        new_result->left_ = result;
        new_result->right_ = right;
        result = new_result;
    }
    return result;
}

node_t Parser::parseProduct() {
    auto result = parsePower();
    while (text[pointer] == '*' || text[pointer] == '/') {
        NodeTypeName nodeType = (text[pointer] == '*') ? NodeTypeName::MUL : NodeTypeName::DIV;
        ++pointer;
        auto new_result = std::make_shared<Node>(nodeType);
        new_result->left_ = result;
        new_result->right_ = parsePower();;
        result = new_result;
    }
    return result;
}

node_t Parser::parsePower() {
    auto result = parsePrimary();
    if (text[pointer] == '^') {
        ++pointer;
        auto new_result = std::make_shared<Node>(NodeTypeName::POW);
        new_result->left_ = result;
        new_result->right_ = parsePower();
        result = new_result;
    }
    return result;
}

node_t Parser::parsePrimary() {
    if (text[pointer] == '(') {
        ++pointer;
        auto result = parseExpression();
        throwIf(text[pointer] != ')');
        ++pointer;
        return result;
    }
    if (text[pointer] == '-') {
        ++pointer;
        auto result = std::make_shared<Node>(NodeTypeName::UNARY_MINUS);
        result->left_ = parsePrimary();
        return result;
    }
    if (isdigit(text[pointer])) {
        return parseNumber();
    }
    std::string id;
    while (isalpha(text[pointer])) {
        id.push_back(text[pointer]);
        ++pointer;
    }
    if (id.size() == 1) {
        return std::make_shared<Node>(NodeTypeName::VARIABLE, id[0]);
    }
    throwIf(id.empty());
    throwIf(id != "sin" && id != "cos" && id != "log");
    throwIf(text[pointer] != '(');
    ++pointer;
    auto nodeExpression = parseExpression();
    throwIf(text[pointer] != ')');
    ++pointer;
    NodeTypeName resultType = (id == "sin") ? NodeTypeName::SIN :
                              (id == "cos") ? NodeTypeName::COS : NodeTypeName::LOG;
    auto result = std::make_shared<Node>(resultType);
    result->left_ = nodeExpression;
    return result;
}

node_t Parser::parseNumber() {
    throwIf(!isdigit(text[pointer]));
    int value = 0;
    while (isdigit(text[pointer])) {
        value = value * 10 + (text[pointer] - '0');
        ++pointer;
    }
    return std::make_shared<Node>(NodeTypeName::NUMBER, value);
}

#endif //INC_5_DIFFERENTIATOR_PARSER_H

//
//#include <iostream>
//#include "tree.h"
//#include "TexLogger.h"
//
//#define ASSERT_TRUE(x) assert((x))
//
//bool IsNumber(std::string& str) {
//    for (char i : str) {
//        if (!isdigit(i) && i != '.') {
//            return false;
//        }
//    }
//    return true;
//}
//
//std::string MySubStr(size_t l, size_t r, std::string& str) {
//    ASSERT_TRUE(r <= str.size());
//    return str.substr(l, r - l + 1);
//}
//
//node_t Parser(size_t l, size_t r, std::string& str) {
//    std::cout << str[l] << ' ' << str[r] << std::endl;
//    std::cout << MySubStr(l, r, str) << std::endl;
//    if (str[l] == '(' && str[r] == ')') {
//        /*resizing the area*/
//        l++;
//        r--;
//    }
//    int bracesBalance = 0;
//    for (int i = l; i <= r; ++i) {
//        if (str[i] == '(') {
//            bracesBalance++;
//        }
//        if (str[i] == ')') {
//            bracesBalance--;
//        }
//        if ((str[i] == '*' || str[i] == '+' || str[i] == '-' || str[i] == '/' || str[i] == '^') && bracesBalance == 0) {
//            std::cout << str[i] << ' ' << l << ' ' << r << ' ' << std::endl;
//            auto left = Parser(l, i-1, str);
//            auto right = Parser(i+1, r, str);
//            NodeTypeName typeName;
//            switch (str[i]) {
//                case '*' : typeName = MUL; break;
//                case '+' : typeName = ADD; break;
//                case '-' : typeName = SUB; break;
//                case '/' : typeName = DIV; break;
//                case '^' : typeName = POW; break;
//                default: typeName = SUB;
//            }
//            return CreateBinaryOperation(left, right, typeName);
//        }
//    }
//
//    //Check SIN, COS, LOG
//    std::string example = "sin(x)";
//    std::cout << MySubStr(l, r, str) << std::endl;
//    if (r - l + 1 >= example.size()) {
//        if (MySubStr(l, l + 2, str) == "sin") {
//            auto argument = Parser(l + 3, r, str);
//            return CreateUnaryOperation(argument, SIN);
//        }
//        if (MySubStr(l, l + 2, str) == "cos") {
//            auto argument = Parser(l + 3, r, str);
//            return CreateUnaryOperation(argument, COS);
//        }
//        if (MySubStr(l, l + 2, str) == "log") {
//            auto argument = Parser(l + 3, r, str);
//            return CreateUnaryOperation(argument, LOG);
//        }
//    }
//
//    //Check UnaryMinus
//    example = "-(x)";
//    if (r - l + 1 >= example.size()) {
//        if (str[l] == '-') {
//            auto argument = Parser(1, r, str);
//            return CreateUnaryOperation(argument, UNARY_MINUS);
//        }
//    }
//
//    //Check single Variable and Number
//    example = "x";
//    if (r - l + 1 >= example.size()) {
//        auto exp = MySubStr(l, r, str);
//        std::cout << exp << std::endl;
//        if (IsNumber(exp)) {
//            return CreateNumber(std::stoi(exp));
//        } else {
//            /*exp will be with braces (x)*/
//            std::cout << "VARIABLE  " << exp << std::endl;
//            int iter = 0;
//            if (exp[0] == '(') {
//                iter++;
//            }
//            return CreateVariable(exp[iter]);
//        }
//    }
//
//    return nullptr;
//}
//