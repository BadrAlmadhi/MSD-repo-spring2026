#pragma once
#include <string>
#include "pointer.h"

// forward declaration 
class Expr;
class Env;

CLASS(Val) {
public:
    virtual ~Val() {}

    // compare two values for structural equality
    virtual bool equals(PTR(Val) other) = 0;

    // operation add and mult 
    virtual PTR(Val) add_to(PTR(Val) other) = 0;
    virtual PTR(Val) mult_with(PTR(Val) other) = 0;

    // print result of interp()
    virtual std::string to_string() = 0;

    // convert value back to expr
    virtual PTR(Expr) to_expr() = 0;

    virtual bool is_true() = 0;
};

class NumVal : public Val {
public:
    int val;

    explicit NumVal(int val);

    bool equals(PTR(Val) other) override;
    PTR(Val) add_to(PTR(Val) other) override;
    PTR(Val) mult_with(PTR(Val) other) override;
    std::string to_string() override;
    PTR(Expr) to_expr() override;
    bool is_true() override;
};

class BoolVal : public Val {
public:
    bool val;

    explicit BoolVal(bool val);

    bool equals(PTR(Val) other) override;
    PTR(Val) add_to(PTR(Val) other) override;
    PTR(Val) mult_with(PTR(Val) other) override;
    std::string to_string() override;
    PTR(Expr) to_expr() override;
    bool is_true() override;
};

class FunVal : public Val {
public:
    std::string formal_arg;
    PTR(Expr) body;
    PTR(Env) env;

    FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env);

    bool equals(PTR(Val) other) override;
    PTR(Val) add_to(PTR(Val) other) override;
    PTR(Val) mult_with(PTR(Val) other) override;
    std::string to_string() override;
    PTR(Expr) to_expr() override;
    bool is_true() override;
};