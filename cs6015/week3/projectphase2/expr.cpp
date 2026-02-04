#include "expr.h"
#include <stdexcept>
#include <ostream>



// implementing the declared function from .h
// constructor assign val1
Num::Num(int val) {
    this->val = val;
}


void Num::printExp(std::ostream& out) {
    out << val;
}

// are these two expression same kind?
bool Num::equals(Expr* e) {
    // checks if e pointing at Num
    // r point to Num
    Num* other = dynamic_cast<Num*>(e);
    // check is other is a number
    if (other == nullptr) {
        return false;
    }
    return this->val == other->val;
}

int Num::interp() {
    return this->val;
}

bool Num::has_variable() {
    return false;
}

Expr* Num::subst(std::string, Expr*) {
    // return new Num(this->val);
    return this; // we could use new but we already has the value saved. 
}

// 
Var::Var(std::string name) {
    this->name = name;
}

void Var::printExp(std::ostream& out) {
    out << name;
}

int Var::interp() {
    throw std::runtime_error("Variable cannot be interpreted without a value.");
}

bool Var::has_variable() {
    return true;
}

Expr* Var::subst(std::string var, Expr* expr) {
    if (this->name == var) {
        return expr;
    } else {
        return this; // we already have the value saved in this. so we can just return the value instead of creating new value.
    }
}

// same concept 
bool Var::equals(Expr* e) {
    Var* other = dynamic_cast<Var*>(e);
    if (other == nullptr) {
        return false;
    }
    return this->name == other->name;
}

// here where we split left and right 
// more like how a tree works
Add::Add(Expr* lhs, Expr* rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

void Add::printExp(std::ostream& out) {
    out << "(";
    lhs->printExp(out);
    out << "+";
    rhs->printExp(out);
    out << ")";
}

int Add::interp() {
    return lhs->interp() + rhs->interp();
}

bool Add::has_variable() {
    if (lhs->has_variable() || rhs->has_variable()){
        return true;
    } else {
        return false;
    }
}

Expr* Add::subst(std::string var, Expr* expr) {
    return new Add(lhs->subst(var, expr), rhs->subst(var, expr));
}



// then follow same concept but add both lhs and rhs
bool Add::equals(Expr* e) {
    Add* other = dynamic_cast<Add*>(e);
    if (other == nullptr) {
        return false;
    }

    return this->lhs->equals(other->lhs) &&
           this->rhs->equals(other->rhs);
}

Mult::Mult(Expr* lhs, Expr* rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

void Mult::printExp(std::ostream& out) {
    out << "(";
    lhs->printExp(out);
    out << "*";
    rhs->printExp(out);
    out << ")";
}

bool Mult::equals(Expr* e) {
    Mult* other = dynamic_cast<Mult*>(e);
    if (other == nullptr) {
        return false;
    }

    return this->lhs->equals(other->lhs) &&
           this->rhs->equals(other->rhs);
}

int Mult::interp () {

    return lhs->interp() * rhs->interp();
}

bool Mult::has_variable() {
    return lhs->has_variable() || rhs->has_variable();
}

Expr* Mult::subst(std::string var, Expr* expr) {
    return new Mult(lhs->subst(var, expr), rhs->subst(var, expr));
}
