#include <iostream>
#include <vector>
#include <string>
#include "Tree.h"
#include "Parser.h"
#include "Differentiator.h"
#include "Simplificator.h"
#include "Validator.h"
#include "GlobalVar.h"

void Test(std::vector<std::string>& expressions) {
    for (auto exp :expressions) {
        logger.WriteLine("----------------------------------- \n\n");

        if (!InputValidator(exp)) {
            throw std::runtime_error("Not valid expression");
        }
        Parser parser(exp);
        node_t root = parser.parse();
        if (!TreeValidator(root)) {
            throw std::runtime_error("Tree is not OK");
        }

        logger.WriteLine(R"($f(x)$ = )");
        logger.WriteTreeToTex(root);
        logger.WriteLine("\n\n");

        node_t df = Differentiator(root, 'x');
        if (!TreeValidator(df)) {
            throw std::runtime_error("Tree is not OK");
        }
        node_t simple_df = Simplificator(df);

        logger.WriteLine("RESULT  ");
        logger.WriteLine(R"($\frac{\partial }{\partial x}$ = )");
        logger.WriteTreeToTex(simple_df);
        logger.WriteLine("\n\n");

        logger.WriteLine("----------------------------------- \n\n");
    }
}

int main() {
    std::vector<std::string> expressions = {"(log(x))^5",
                                            "x^x",
                                            "(x^2)+(x^3)",
                                            "x-x",
                                            "1+x"};
    Test(expressions);
    return 0;
}