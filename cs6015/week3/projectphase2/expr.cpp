#include "expr.h"

// implementing the declared function from .h
// constructor assign val1
Num::Num(int val) {
    this->val = val;
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

// 
Var::Var(std::string name) {
    this->name = name;
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

bool Mult::equals(Expr* e) {
    Mult* other = dynamic_cast<Mult*>(e);
    if (other == nullptr) {
        return false;
    }

    return this->lhs->equals(other->lhs) &&
           this->rhs->equals(other->rhs);
}
