#include <iostream>
#include <memory>

#include "book_expr_ eval.h"

auto main() -> int {
    BinaryExpression plus(
        std::make_shared<Number>(10), std::make_shared<Number>(20), Operator::kPlus);

    {
        TreeEvaluatorVisitor eval;
        std::cout << plus.accept(eval) << std::endl;

        ReversePolishVisitor printer;
        std::cout << plus.accept(printer) << std::endl;
    }

    {
        // flattern visitor
        std::cout << "Flattern evaluate " << Evalute(&plus) << std::endl;
    }

    return 0;
}