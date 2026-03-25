#pragma once
#include <string>

// forward declaration 
class Expr;

class Val {
public:
    virtual ~Val() = default;

    // compare two values for structural equality
    virtual bool equals(Val* other) = 0;

    // operation add and mult 
    virtual Val* add_to(Val* other) = 0;
    virtual Val* mult_with(Val* other) = 0;

    // print result of interp()
    virtual std::string to_string() = 0;

    // convert value back to expr
    virtual Expr* to_expr() = 0;

    virtual bool is_true() = 0;
};

class NumVal : public Val {
public:
    int val;

    explicit NumVal(int val);

    bool equals(Val* other) override;
    Val* add_to(Val* other) override;
    Val* mult_with(Val* other) override;
    std::string to_string() override;
    Expr* to_expr() override;
    bool is_true() override;
};

class BoolVal : public Val {
public:
    bool val;

    explicit BoolVal(bool val);

    bool equals(Val* other) override;
    Val* add_to(Val* other) override;
    Val* mult_with(Val* other) override;
    std::string to_string() override;
    Expr* to_expr() override;
    bool is_true() override;
};

class FunVal : public Val {
public:
    std::string formal_arg;
    Expr* body;

    FunVal(std::string formal_arg, Expr* body);

    bool equals(Val* other) override;
    Val* add_to(Val* other) override;
    Val* mult_with(Val* other) override;
    std::string to_string() override;
    Expr* to_expr() override;
    bool is_true() override;
};