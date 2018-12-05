//
// Created by Никита on 04/12/2018.
//

#ifndef INC_5_DIFFERENTIATOR_VALIDATOR_H
#define INC_5_DIFFERENTIATOR_VALIDATOR_H

#include "Tree.h"

bool TreeValidator(const node_t& curr) noexcept {
    if (curr == nullptr) {
        return false;
    }
    auto type_name = curr->type_name_;
    bool binary_operation = (type_name == ADD) ||
                            (type_name == SUB) ||
                            (type_name == MUL) ||
                            (type_name == DIV) ||
                            (type_name == POW);

    if (binary_operation) {
        return TreeValidator(curr->left_) && TreeValidator(curr->right_);
    }

    bool unary_operation = (type_name == UNARY_MINUS) ||
                           (type_name == SIN) ||
                           (type_name == COS) ||
                           (type_name == LOG);
    if (unary_operation) {
        return TreeValidator(curr->left_) && curr->right_ == nullptr;
    }

    if (type_name == VARIABLE || type_name == NUMBER) {
        return curr->left_ == nullptr && curr->right_ == nullptr;
    }
}

bool InputValidator(const std::string& s) noexcept {
    int balance = 0;
    for (auto ch:s) {
        if (ch == '(') {
            balance++;
        }
        if (ch == ')') {
            balance--;
        }
        if (balance < 0) {
            return false;
        }
    }
    return balance == 0;
}

#endif //INC_5_DIFFERENTIATOR_VALIDATOR_H
