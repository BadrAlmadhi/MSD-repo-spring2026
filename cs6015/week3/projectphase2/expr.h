#pragma once  // prevent header from being included multiple times
#include <string> // for variables

// base class
class Expr {
public:
    virtual ~Expr() = default;
    virtual bool equals(Expr* e) = 0; // each subclass must override
};

// Num represents number expression (leaf node in the tree)
class Num : public Expr {
public:
    int val; // actual value

    // constructor declaration
    Num(int val);

    bool equals(Expr* e) override;
};

// Var represents a variable expression (leaf node), e.g., "x"
class Var : public Expr {
public:
    std::string name;

    // constructor declaration
    Var(std::string name);

    bool equals(Expr* e) override;
};

// Add represents addition of two expressions
class Add : public Expr {
public:
    Expr* lhs; // left
    Expr* rhs; // right

    // constructor declaration
    Add(Expr* lhs, Expr* rhs);

    bool equals(Expr* e) override;
};

// Mult represents multiplication of two expressions
class Mult : public Expr {
public:
    Expr* lhs; // left
    Expr* rhs; // right

    // constructor declaration
    Mult(Expr* lhs, Expr* rhs);

    bool equals(Expr* e) override;
};
