#include <iostream>
#include <memory>

#include "book_expr_ eval.h"
#include "ch05/func_compose.h"

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

    {
        auto cd = Filter(ExprList(&plus),
                         [](auto expr) { return expr.GetKind() != ExprKind::kOperator; });
        auto map = Map(
            cd,
            [](auto& item) {
                item.SetValue(item.GetValue() * 3);
                return item;
            } |
                [](auto& item) {
                    const auto v = item.GetValue();
                    item.SetValue(v * v);
                    return item;
                });

        for (const auto& item : map) {
            item.ToString();
        }
    }

    return 0;
}