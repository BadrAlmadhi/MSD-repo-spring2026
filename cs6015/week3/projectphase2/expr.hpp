#ifndef EXPR_H
#define EXPR_H

#include <string>
#include <iostream>
#include <sstream>
#include "pointer.h"

typedef enum {
    prec_none,
    prec_eq,
    prec_add,
    prec_mult
} precedence_t;

// defin class Val
class Val;
class Env;

// base class
CLASS(Expr) {
public:
    virtual ~Expr() {}

    
    virtual bool equals(PTR(Expr) e) = 0;

    // Add Env
    virtual PTR(Val) interp(PTR(Env) env) = 0;
    
    // virtual PTR(Expr) subst(std::string, PTR(Expr) e) = 0;

    virtual void printExpr(std::ostream& out) = 0;

    std::string to_string() {
        std::stringstream st("");
        this->printExpr(st);
        return st.str();
    }

    virtual void pretty_print(std::ostream& out) {
        printExpr(out);
    }

    std::string to_pretty_string() {
        std::stringstream st("");
        this->pretty_print(st);
        return st.str();
    }

    virtual void pretty_print_at(std::ostream& out, precedence_t) {
        pretty_print(out);
    }
};

// Number Expression
class ExprNum : public Expr {
public:
    int value;

    ExprNum(int value);

    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    // PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t) override;
};

// Add Expression
class ExprAdd : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;

    ExprAdd(PTR(Expr) lhs, PTR(Expr) rhs);

    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    //PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t) override;
};

// Multiply Expression
class ExprMult : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;

    ExprMult(PTR(Expr) lhs, PTR(Expr) rhs);

    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    //PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t) override;
};

// Variable Expression
class ExprVar : public Expr {
public:
    std::string var;

    ExprVar(std::string var);

    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    //PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t) override;
};

// Let Expression
class ExprLet : public Expr {
public:
    std::string name;
    PTR(Expr) rhs;
    PTR(Expr) body;

    ExprLet(std::string name, PTR(Expr) rhs, PTR(Expr) body);

    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    //PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t prec) override;
};

// Boolean Expression
class BoolExpr : public Expr {
public:
    bool value;

    BoolExpr(bool value);

    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    //PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t) override;
};

// Equality Expression
class EqExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;

    EqExpr(PTR(Expr) lhs, PTR(Expr) rhs);

    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    //PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t prec) override;
};

// If Expression
class IfExpr : public Expr {
public:
    PTR(Expr) test_part;
    PTR(Expr) then_part;
    PTR(Expr) else_part;

    IfExpr(PTR(Expr) test_part, PTR(Expr) then_part, PTR(Expr) else_part);

    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    //PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t prec) override;
};


// Function Expression
class FunExpr : public Expr {
public:
    std::string formal_arg;
    PTR(Expr) body;

    FunExpr(std::string formal_arg, PTR(Expr) body);

    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    //PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t prec) override;
};

// Function Call Expression
class CallExpr : public Expr {
public:
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;

    CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg);

    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    //PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t prec) override;
};

#endif