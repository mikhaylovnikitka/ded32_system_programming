#include <iostream>
#include "tree.h"
#include "parser.h"
#include "differentiator.h"

int main() {
    std::string expression = "((5)+(sin((x))))";
    node_t root = Parser(0, expression.size() - 1, expression);
    node_t df = Differentiator(root);

    return 0;
}