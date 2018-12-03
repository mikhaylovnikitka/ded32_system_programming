#include <iostream>
#include "tree.h"
#include "parser.h"
#include "differentiator.h"
#include "Simplificator.h"
#include "validator.h"

int main() {
    std::string expression = "((5)+(sin((x))))";
    node_t root = Parser(0, expression.size() - 1, expression);
    if (!TreeValidator(root)) {
        throw std::runtime_error("Tree is not OK");
    }
    node_t df = Differentiator(root, 'x');
    if (!TreeValidator(df)) {
        throw std::runtime_error("Tree is not OK");
    }
    node_t simple_df = Simplificator(df);

    return 0;
}