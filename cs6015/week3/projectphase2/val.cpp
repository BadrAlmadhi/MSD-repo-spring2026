#include "val.h"
#include "expr.hpp"
#include <stdexcept>


NumVal::NumVal(int val) : val(val) {}

bool NumVal::equals(Val* other) {
    NumVal* o = dynamic_cast<NumVal*>(other);
    if (o == nullptr) return false;
    return this->val == o->val;
}

Val* NumVal::add_to(Val* other) {
    NumVal* o = dynamic_cast<NumVal*>(other);
    if (o == nullptr) {
        throw std::runtime_error("add_to: not a number");
    }
    return new NumVal(this->val + o->val);
}

Val* NumVal::mult_with(Val* other) {
    NumVal* o = dynamic_cast<NumVal*>(other);
    if (o == nullptr) {
        throw std::runtime_error("mult_with: not a number");
    }
    return new NumVal(this->val * o->val);
}

std::string NumVal::to_string() {
    return std::to_string(val);
}

Expr* NumVal::to_expr() {
    return new ExprNum(val);
}