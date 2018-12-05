#include <iostream>
#include "Tree.h"
#include "Parser.h"
#include "Differentiator.h"
#include "Simplificator.h"
#include "Validator.h"
#include "GlobalVar.h"

int main() {
    std::string expression = "x^5";
    if (!InputValidator(expression)) {
        throw std::runtime_error("Not valid expression");
    }
    Parser parser(expression);
    node_t root = parser.parse();
    if (!TreeValidator(root)) {
        throw std::runtime_error("Tree is not OK");
    }
    node_t df = Differentiator(root, 'x');
    if (!TreeValidator(df)) {
        throw std::runtime_error("Tree is not OK");
    }
    node_t simple_df = Simplificator(df);


    logger.WriteLine("----------------------------------------- \n\n");
    logger.WriteLine(R"($f(x)$ = )");
    logger.WriteTreeToTex(root);
    logger.WriteLine("\n\n");
    logger.WriteLine(R"($\frac{\partial }{\partial x}$ = )");
    logger.WriteTreeToTex(simple_df);
    logger.WriteLine("\n\n");

    return 0;
}