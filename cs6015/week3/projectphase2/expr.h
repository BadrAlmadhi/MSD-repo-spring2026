#pragma once  // prevent header from being included multiple times
#include <string> // for string variables
#include <ostream> // 

// base class
class Expr {
public:
    virtual ~Expr() = default;
    virtual bool equals(Expr* e) = 0; // each subclass must override
    virtual int interp() = 0;  // checks the value of expression 
    virtual bool has_variable() = 0; // checks if expression has variables ("X") = true
    virtual Expr* subst(std::string, Expr*) = 0; // replaces variable with expression
    virtual void printExp (std::ostream& out) = 0; // print to stream instead of screen
};

// Num represents number expression (leaf node in the tree)
class Num : public Expr {
public:
    int val; // actual value

    // constructor declaration
    Num(int val);

    bool equals(Expr* e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string, Expr*) override;
    void printExp (std::ostream& out) override; 

};

// Var represents a variable expression (leaf node), e.g., "x"
class Var : public Expr {
public:
    std::string name;

    // constructor declaration
    Var(std::string name);

    bool equals(Expr* e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string, Expr*) override;
    void printExp (std::ostream& out) override; 

};

// Add represents addition of two expressions
class Add : public Expr {
public:
    Expr* lhs; // left
    Expr* rhs; // right

    // constructor declaration
    Add(Expr* lhs, Expr* rhs);

    bool equals(Expr* e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string, Expr*) override;
    void printExp (std::ostream& out) override; 

};

// Mult represents multiplication of two expressions
class Mult : public Expr {
public:
    Expr* lhs; // left
    Expr* rhs; // right

    // constructor declaration
    Mult(Expr* lhs, Expr* rhs);

    bool equals(Expr* e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string, Expr*) override;
    void printExp (std::ostream& out) override; 

};


