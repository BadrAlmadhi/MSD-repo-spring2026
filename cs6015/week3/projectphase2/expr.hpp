#ifndef EXPR_H
#define EXPR_H

#include <string>
#include <iostream>
#include <sstream>

typedef enum {
    prec_none,
    prec_eq,
    prec_add,
    prec_mult
} precedence_t;

class Val;

// base class
class Expr {
public:
    virtual ~Expr() = default;

    virtual bool equals(Expr *e) = 0;
    virtual Val* interp() = 0;
    virtual bool has_variable() = 0;
    virtual Expr* subst(std::string, Expr* e) = 0;

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

    bool equals(Expr* e) override;
    Val* interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t) override;
};

// Add Expression
class ExprAdd : public Expr {
public:
    Expr* lhs;
    Expr* rhs;

    ExprAdd(Expr* lhs, Expr* rhs);

    bool equals(Expr* e) override;
    Val* interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t) override;
};

// Multiply Expression
class ExprMult : public Expr {
public:
    Expr* lhs;
    Expr* rhs;

    ExprMult(Expr* lhs, Expr* rhs);

    bool equals(Expr* e) override;
    Val* interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t) override;
};

// Variable Expression
class ExprVar : public Expr {
public:
    std::string var;

    ExprVar(std::string var);

    bool equals(Expr* e) override;
    Val* interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t) override;
};

// Let Expression
class ExprLet : public Expr {
public:
    std::string name;
    Expr* rhs;
    Expr* body;

    ExprLet(std::string name, Expr* rhs, Expr* body);

    bool equals(Expr* e) override;
    Val* interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t prec) override;
};

// Boolean Expression
class BoolExpr : public Expr {
public:
    bool value;

    BoolExpr(bool value);

    bool equals(Expr* e) override;
    Val* interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t) override;
};

// Equality Expression
class EqExpr : public Expr {
public:
    Expr* lhs;
    Expr* rhs;

    EqExpr(Expr* lhs, Expr* rhs);

    bool equals(Expr* e) override;
    Val* interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t prec) override;
};

// If Expression
class IfExpr : public Expr {
public:
    Expr* test_part;
    Expr* then_part;
    Expr* else_part;

    IfExpr(Expr* test_part, Expr* then_part, Expr* else_part);

    bool equals(Expr* e) override;
    Val* interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t prec) override;
};

#endif