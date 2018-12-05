//
// Created by Никита on 04/12/2018.
//

#ifndef INC_5_DIFFERENTIATOR_SIMPLIFICATOR_H
#define INC_5_DIFFERENTIATOR_SIMPLIFICATOR_H

#include "Tree.h"
#include "Differentiator.h"


bool IsZero(node_t curr) {
    if (curr == nullptr) {
        throw std::invalid_argument("Pointer was nullptr.");
    }
    return curr->type_name_ == NUMBER && curr->value_ == 0;
}

bool IsOne(node_t curr) {
    if (curr == nullptr) {
        throw std::invalid_argument("Pointer was nullptr.");
    }
    return curr->type_name_ == NUMBER && curr->value_ == 1;
}

node_t Simplificator(const node_t curr) {
    if (curr == nullptr) {
        return nullptr;
    }
    auto simple_left = Simplificator(curr->left_);
    auto simple_right = Simplificator(curr->right_);

    switch (curr->type_name_) {
        case NodeTypeName::SUB:{
            if (IsZero(simple_left) && IsZero(simple_right)) {
                return CreateNumber(0);
            }
            if (IsOne(simple_left) && IsOne(simple_right)) {
                return CreateNumber(0);
            }
            if (IsZero(simple_left)) {
                return simple_right;
            }
            if (IsZero(simple_right)) {
                return simple_left;
            }
            return CreateBinaryOperation(simple_left, simple_right, curr->type_name_);
        }
        case NodeTypeName::ADD:{
            if (IsZero(simple_left) && IsZero(simple_right)) {
                return CreateNumber(0);
            }
            if (IsZero(simple_left)) {
                return simple_right;
            }
            if (IsZero(simple_right)) {
                return simple_left;
            }
            return CreateBinaryOperation(simple_left, simple_right, curr->type_name_);
        }
        case NodeTypeName::MUL : {
            if (IsZero(simple_left) || IsZero(simple_right)) {
                return CreateNumber(0);
            }
            if (IsOne(simple_left)) {
                return simple_right;
            }
            if (IsOne(simple_right)) {
                return simple_left;
            }
            return CreateBinaryOperation(simple_left, simple_right, curr->type_name_);
        }
        case NodeTypeName::DIV : {
            if (IsZero(simple_right)) {
                throw std::invalid_argument("Division by zero!");
            }
            if (IsZero(simple_left)) {
                return CreateNumber(0);
            }
            if (IsOne(simple_right)) {
                return simple_left;
            }
            return CreateBinaryOperation(simple_left, simple_right, curr->type_name_);
        }

        case NodeTypeName::POW : {
            /* lets suppose f(x) = h(x)^g(x) */
            if (IsZero(simple_left)) {
                return CreateNumber(0);
            }
            if (IsOne(simple_left)) {
                return CreateNumber(1);
            }
            if (IsZero(simple_right)) {
                return CreateNumber(1);
            }
            if (IsOne(simple_right)) {
                return simple_left;
            }
            return CreateBinaryOperation(simple_left, simple_right, curr->type_name_);
        }

        case NodeTypeName::UNARY_MINUS : {
            if (IsZero(simple_left)) {
                return CreateNumber(0);
            }
            if (IsOne(simple_left)) {
                return CreateNumber(-1);
            }
            return CreateUnaryOperation(simple_left, curr->type_name_);
        }

        case NodeTypeName::SIN : {
            if (IsZero(simple_left)) {
                return CreateNumber(0);
            }
            return CreateUnaryOperation(simple_left, curr->type_name_);
        }

        case NodeTypeName::COS : {
            return CreateUnaryOperation(simple_left, curr->type_name_);
        }

        case NodeTypeName::LOG : {
            return CreateUnaryOperation(simple_left, curr->type_name_);
        }
        default: return curr;
    }
}

#endif //INC_5_DIFFERENTIATOR_SIMPLIFICATOR_H
