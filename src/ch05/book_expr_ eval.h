#include <cassert>
#include <iostream>
#include <memory>
#include <utility>

// #include "func_compose.h"

enum class Operator : unsigned char { kIllegal, kPlus, kMius, kMul, kDiv, kUnaryPlus, kBinaryPlus };

class Number;
class BinaryExpression;
class UnaryExpression;

class IExpressionVisitor {
   public:
    virtual double Visit(Number& number) = 0;
    virtual double Visit(BinaryExpression& number) = 0;
    virtual double Visit(UnaryExpression& number) = 0;
};

class Expression {
   public:
    virtual double accept(IExpressionVisitor& vistor) = 0;
    virtual ~Expression() = default;
};

class Number final : public Expression {
   public:
    explicit Number(const double num) : num_{num} {}
    ~Number() override = default;

    auto Set(const double num) {
        num_ = num;
    }

    auto Get() const {
        return num_;
    }

    double accept(IExpressionVisitor& visitor) override {
        return visitor.Visit(*this);
    }

   private:
    double num_ = 0.0;
};

class BinaryExpression final : public Expression {
    using expr_ptr = std::shared_ptr<Expression>;

   public:
    BinaryExpression(expr_ptr left, expr_ptr right, Operator op)
        : left_(std::move(left)), right_(std::move(right)), op_(op) {}
    ~BinaryExpression() override = default;

    auto GetOperator() const {
        return op_;
    }

    auto GetLeft() const {
        return left_;
    }

    auto getRight() const {
        return right_;
    }

    double accept(IExpressionVisitor& visitor) override {
        return visitor.Visit(*this);
    }

   private:
    expr_ptr left_{};
    expr_ptr right_{};
    Operator op_{Operator::kIllegal};
};

class UnaryExpression final : public Expression {
    using expr_ptr = std::shared_ptr<Expression>;

   public:
    UnaryExpression(expr_ptr right, Operator op) : right_(std::move(right)), op_(op) {}
    ~UnaryExpression() override = default;

    auto GetOperator() const {
        return op_;
    }

    auto getRight() const {
        return right_;
    }

    double accept(IExpressionVisitor& visitor) override {
        return visitor.Visit(*this);
    }

   private:
    expr_ptr right_{};
    Operator op_{Operator::kIllegal};
};

class TreeEvaluatorVisitor final : public IExpressionVisitor {
   public:
    double Visit(Number& number) override {
        return number.Get();
    }

    double Visit(BinaryExpression& expr) override {
        const auto lval = expr.GetLeft()->accept(*this);
        const auto rval = expr.getRight()->accept(*this);
        switch (expr.GetOperator()) {
            case Operator::kPlus: return lval + rval;
            case Operator::kMius: return lval - rval;
            case Operator::kMul: return lval * rval;
            case Operator::kDiv: return lval / rval;
            default: assert(false && "invalid operation");
        }
        return 0.0;
    }

    double Visit(UnaryExpression& expr) override {
        const auto rval = expr.getRight()->accept(*this);
        switch (expr.GetOperator()) {
            case Operator::kPlus: return +rval;
            case Operator::kMius: return -rval;
            default: assert(false && "invalid operator");
        }

        return 0.0;
    }
};

class ReversePolishVisitor : public IExpressionVisitor {
   public:
    double Visit(Number& expr) override {
        std::cout << expr.Get() << std::endl;
        return 42.0;
    }

    double Visit(BinaryExpression& expr) override {
        expr.GetLeft()->accept(*this);
        expr.getRight()->accept(*this);
        switch (expr.GetOperator()) {
            case Operator::kPlus: std::cout << " + " << std::endl; break;
            case Operator::kMius: std::cout << " - " << std::endl; break;
            case Operator::kMul: std::cout << " x " << std::endl; break;
            case Operator::kDiv: std::cout << " / " << std::endl; break;
            default: assert(false && "invalid operator");
        }

        return 42.0;
    }

    double Visit(UnaryExpression& expr) override {
        expr.getRight()->accept(*this);
        switch (expr.GetOperator()) {
            case Operator::kPlus: std::cout << "+" << std::endl; break;
            case Operator::kMius: std::cout << "-" << std::endl; break;
            default: assert(false && "invalid operator");
        }

        return 42.0;
    }
};