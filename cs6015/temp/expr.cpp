#include "expr.h"




// initialize values for num
Num::Num(int val) {
    this->val = val;
}


Add::Add(Expr *lhs, Expr *rhs){
    this->lhs = lhs;
    this->rhs = rhs;
}

Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}


bool Num::equals(Expr *e) {
    // treat e as Num
    Num* other = dynamic_cast<Num*>(e);

    // if e not a number, they are not equal. 
    if (other == nullptr){
        return false;
    }

    return this->val == other->val;
}