//
// Created by Никита on 03/12/2018.
//

#ifndef INC_5_DIFFERENTIATOR_DIFFERENTIATOR_H
#define INC_5_DIFFERENTIATOR_DIFFERENTIATOR_H

#include "tree.h"

node_t Differentiator(node_t curr) {
    if (curr == nullptr) {
        return nullptr;
    }
    switch (curr->type_name_) {
        case NodeTypeName::SUB:{
            auto d_left = Differentiator(curr->left_);
            auto d_right = Differentiator(curr->right_);
            return CreateBinaryOperation(
                    d_left,
                    d_right,
                    NodeTypeName::SUB);
        }
        case NodeTypeName::ADD:{
            auto d_left = Differentiator(curr->left_);
            auto d_right = Differentiator(curr->right_);
            return CreateBinaryOperation(
                    d_left,
                    d_right,
                    NodeTypeName::ADD);
        }
        case NodeTypeName::MUL : {
            auto d_left = Differentiator(curr->left_);
            auto d_right = Differentiator(curr->right_);
            auto first_mul = CreateBinaryOperation(
                    d_left,
                    curr->right_,
                    NodeTypeName::MUL);
            auto second_mul = CreateBinaryOperation(
                    curr->left_,
                    d_right,
                    NodeTypeName::MUL);
            return CreateBinaryOperation(
                    first_mul,
                    second_mul,
                    NodeTypeName::SUB);
        }
        case NodeTypeName::DIV : {
            auto d_left = Differentiator(curr->left_);
            auto d_right = Differentiator(curr->right_);
            auto first_mul = CreateBinaryOperation(
                    d_left,
                    curr->right_,
                    NodeTypeName::MUL);
            auto second_mul = CreateBinaryOperation(
                    curr->left_,
                    d_right,
                    NodeTypeName::MUL);
            auto numerator = CreateBinaryOperation(
                    first_mul,
                    second_mul,
                    NodeTypeName::SUB);
            auto denomitanor = CreateBinaryOperation(
                    curr->right_,
                    CreateNumber(2),
                    NodeTypeName::POW);
            return CreateBinaryOperation(
                    numerator,
                    denomitanor,
                    NodeTypeName::DIV);
        }

        case NodeTypeName::POW : {
            /* lets suppose f(x) = h(x)^g(x)
             * so, to diff f(x) = exp(g(x)*ln(h(x)))
             * after that, f'(x) = f(x) * (g(x)*ln(h(x)))' */
            auto h_x = curr->left_;
            auto g_x = curr->right_;
            auto helpful_mul = CreateBinaryOperation(
                    g_x,
                    CreateUnaryOperation(h_x, NodeTypeName::LOG),
                    NodeTypeName::MUL);
            return CreateBinaryOperation(
                    curr,
                    Differentiator(helpful_mul),
                    NodeTypeName::MUL);
        }

        case NodeTypeName::UNARY_MINUS : {
            return Differentiator(curr->left_);
        }

        case NodeTypeName::SIN : {
            auto tmp = CreateUnaryOperation(
                    curr->left_,
                    NodeTypeName::COS);
            return CreateBinaryOperation(
                    tmp,
                    Differentiator(curr->left_),
                    NodeTypeName::MUL);
        }

        case NodeTypeName::COS : {
            auto tmp = CreateUnaryOperation(
                    CreateUnaryOperation(
                            curr->left_,
                            NodeTypeName::COS),
                    NodeTypeName::UNARY_MINUS);
            return CreateBinaryOperation(
                    tmp,
                    Differentiator(curr->left_),
                    NodeTypeName::MUL);
        }

        case NodeTypeName::LOG : {
            auto tmp =  CreateBinaryOperation(
                    CreateNumber(1),
                    curr->left_,
                    NodeTypeName::DIV);
            return CreateBinaryOperation(
                    tmp,
                    Differentiator(curr->left_),
                    NodeTypeName::MUL);
        }

        case NodeTypeName::VARIABLE : {
            return CreateNumber(1);
        }

        case NodeTypeName::NUMBER : {
            return CreateNumber(0);
        }
        default: return nullptr;
    }
}

#endif //INC_5_DIFFERENTIATOR_DIFFERENTIATOR_H
