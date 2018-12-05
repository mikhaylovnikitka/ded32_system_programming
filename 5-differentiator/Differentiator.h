//
// Created by Никита on 03/12/2018.
//

#ifndef INC_5_DIFFERENTIATOR_DIFFERENTIATOR_H
#define INC_5_DIFFERENTIATOR_DIFFERENTIATOR_H

#include "Tree.h"
#include "GlobalVar.h"

node_t Differentiator(const node_t curr, int variable) {
    if (curr == nullptr) {
        return nullptr;
    }
    auto d_left = Differentiator(curr->left_, variable);
    auto d_right = Differentiator(curr->right_, variable);
    switch (curr->type_name_) {
        case NodeTypeName::SUB:{
            logger.WriteLine("Применяя правило взятия произвоной суммы, берем проиводные от слагаемых. \n\n");
            auto answer = CreateBinaryOperation(
                    d_left,
                    d_right,
                    NodeTypeName::SUB);
            logger.WriteBinaryOperationDerivative(curr, answer);
            return answer;
        }
        case NodeTypeName::ADD:{
            logger.WriteLine("Применяя правило взятия произвоной суммы, берем проиводные от слагаемых. \n\n");

            auto answer = CreateBinaryOperation(
                    d_left,
                    d_right,
                    NodeTypeName::ADD);
            logger.WriteBinaryOperationDerivative(curr, answer);
            return answer;
        }
        case NodeTypeName::MUL : {
            logger.WriteLine("Произведение (дифференцируется). \n\n");
            auto first_mul = CreateBinaryOperation(
                    d_left,
                    curr->right_,
                    NodeTypeName::MUL);
            auto second_mul = CreateBinaryOperation(
                    curr->left_,
                    d_right,
                    NodeTypeName::MUL);
            auto answer =  CreateBinaryOperation(
                    first_mul,
                    second_mul,
                    NodeTypeName::ADD);
            logger.WriteBinaryOperationDerivative(curr, answer);
            return answer;
        }
        case NodeTypeName::DIV : {
            logger.WriteLine("Самая сложная производная. \n\n");
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
            auto answer = CreateBinaryOperation(
                    numerator,
                    denomitanor,
                    NodeTypeName::DIV);
            logger.WriteBinaryOperationDerivative(curr, answer);
            return answer;
        }

        case NodeTypeName::POW : {
            /* lets suppose f(x) = h(x)^g(x)
             * so, to diff f(x) = exp(g(x)*ln(h(x)))
             * after that, f'(x) = f(x) * (g(x)*ln(h(x)))' */
            logger.WriteLine("Представим как экспоненту логарифма и подифференцируем. \n\n");
            auto h_x = curr->left_;
            auto g_x = curr->right_;
            auto helpful_mul = CreateBinaryOperation(
                    g_x,
                    CreateUnaryOperation(h_x, NodeTypeName::LOG),
                    NodeTypeName::MUL);
            auto answer = CreateBinaryOperation(
                    curr,
                    Differentiator(helpful_mul, variable),
                    NodeTypeName::MUL);
            logger.WriteBinaryOperationDerivative(curr, answer);
            return answer;
        }

        case NodeTypeName::UNARY_MINUS : {
            logger.WriteLine("Просто унарный минус. \n\n");
            auto answer = Differentiator(curr->left_, variable);
            logger.WriteUnaryOperationDerivative(curr, answer);
            return answer;
        }

        case NodeTypeName::SIN : {
            logger.WriteLine("Производная синуса косинус. \n\n");
            auto tmp = CreateUnaryOperation(
                    curr->left_,
                    NodeTypeName::COS);
            auto answer = CreateBinaryOperation(
                    tmp,
                    Differentiator(curr->left_, variable),
                    NodeTypeName::MUL);
            logger.WriteUnaryOperationDerivative(curr, answer);
            return answer;
        }

        case NodeTypeName::COS : {
            logger.WriteLine("Главное не забыть минус. \n\n");
            auto tmp = CreateUnaryOperation(
                    CreateUnaryOperation(
                            curr->left_,
                            NodeTypeName::COS),
                    NodeTypeName::UNARY_MINUS);
            auto answer = CreateBinaryOperation(
                    tmp,
                    Differentiator(curr->left_, variable),
                    NodeTypeName::MUL);
            logger.WriteUnaryOperationDerivative(curr, answer);
            return answer;
        }

        case NodeTypeName::LOG : {
            logger.WriteLine("Производная логарифма. \n\n");
            auto tmp =  CreateBinaryOperation(
                    CreateNumber(1),
                    curr->left_,
                    NodeTypeName::DIV);
            auto answer = CreateBinaryOperation(
                    tmp,
                    Differentiator(curr->left_, variable),
                    NodeTypeName::MUL);
            logger.WriteUnaryOperationDerivative(curr, answer);
            return answer;
        }

        case NodeTypeName::VARIABLE : {
            logger.WriteLine("Не забыть как смерть икса!!! \n\n");
            auto answer = CreateNumber(curr->value_ == variable ? 1 : 0);
            logger.WriteUnaryOperationDerivative(curr, answer);
            return answer;
        }

        case NodeTypeName::NUMBER : {
            logger.WriteLine("Хоба нолик. \n\n");
            auto answer = CreateNumber(0);
            logger.WriteUnaryOperationDerivative(curr, answer);
            return answer;
        }
        default: return nullptr;
    }
}

#endif //INC_5_DIFFERENTIATOR_DIFFERENTIATOR_H
