//
// Created by Никита on 27/11/2018.
//

#ifndef INC_5_DIFFERENTIATOR_TREE_H
#define INC_5_DIFFERENTIATOR_TREE_H

#pragma once

#include <cmath>
#include <memory>
#include <cassert>
#include <utility>

#define ASSERT_TRUE(x) assert((x))

enum NodeTypeName {
    ADD, SUB, MUL, DIV, POW,        //these for BinaryOperation
    UNARY_MINUS, SIN, COS, LOG,     //there for UnaryOperation
    NUMBER,
    VARIABLE
};

struct Node {
    NodeTypeName type_name_;
    std::shared_ptr<Node> left_;
    std::shared_ptr<Node> right_;

    int value_;

    Node(NodeTypeName typeName, int value = 0):
            type_name_(typeName),
            value_(value),
            left_(nullptr),
            right_(nullptr) {}
};

using node_t =  std::shared_ptr<Node>;

node_t CreateBinaryOperation(node_t left, node_t right, NodeTypeName typeName) {
    bool validate = (typeName == ADD) ||
                    (typeName == SUB) ||
                    (typeName == MUL) ||
                    (typeName == DIV) ||
                    (typeName == POW);
    ASSERT_TRUE(validate);

    auto new_node = std::make_shared<Node>(typeName);
    new_node->left_ = left;
    new_node->right_ = right;
    return new_node;
};

node_t CreateUnaryOperation(node_t left, NodeTypeName typeName) {
    bool validate = (typeName == UNARY_MINUS) ||
                    (typeName == SIN) ||
                    (typeName == COS) ||
                    (typeName == LOG);
    ASSERT_TRUE(validate);

    auto new_node = std::make_shared<Node>(typeName);
    new_node->left_ = left;
    return new_node;
}
node_t CreateNumber(int value) {
    auto new_node = std::make_shared<Node>(NodeTypeName::NUMBER);
    new_node->value_ = value;
    return new_node;
}

node_t CreateVariable(int value) {
    auto new_node = std::make_shared<Node>(NodeTypeName::VARIABLE);
    new_node->value_ = value;
    return new_node;
}
#endif //INC_5_DIFFERENTIATOR_TREE_H

