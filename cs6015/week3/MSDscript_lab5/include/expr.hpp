
#ifndef EXPR_H
#define EXPR_H
#include <string>
#include <iostream>
#include <sstream>
// grammar

typedef enum {
  prec_none,
  prec_add,
  prec_mult
} precedence_t;

// base class
class Expr{
    public:
    // virtual destructor (safety requiems for polymorphism)
    // when we delete Expr* deconstruct
    virtual ~Expr() = default; 
    // = 0 forces every subclass to provide its own implementation
    virtual bool equals(Expr *e) = 0;
    virtual int interp() = 0;
    virtual bool has_variable() = 0;
    virtual Expr* subst(std::string, Expr* e) = 0;

    // print expr
    virtual void printExpr(std::ostream& out) = 0;

    // since this logic be applied to all subclasses we add it in base class
    // objet just print in string no matter where at in subclasses
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

// extend expr class 
// inherit Expr

// Number Expression
class ExprNum : public Expr {
public:

    int value;

    // constructor
    ExprNum(int value);
    
    bool equals(Expr* e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void printExpr(std::ostream& out) override;
};

// Add Expression
class ExprAdd : public Expr{
    public:
    
    Expr* lhs;
    Expr* rhs;
     
    ExprAdd(Expr* lhs, Expr* rhs);

    bool equals(Expr* e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t) override;
};

// Multiply
class ExprMult : public Expr {
    public: 

    Expr* lhs;
    Expr* rhs;

    ExprMult(Expr* lhs, Expr* rhs);

    bool equals(Expr* e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t) override;
};

// variable
class ExprVar : public Expr {
    public:

    std::string var; 

    ExprVar(std::string var);

    bool equals(Expr* e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void printExpr(std::ostream& out) override;

};

class ExprLet : public Expr {
public:
    std::string name;
    Expr* rhs;
    Expr* body;

    ExprLet(std::string name, Expr* rhs, Expr* body);

    bool equals(Expr* e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void printExpr(std::ostream& out) override;
    void pretty_print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t prec) override;
};

#endif