#include "val.h"
#include "expr.hpp"
#include <stdexcept>

NumVal::NumVal(int val) : val(val) {}

bool NumVal::equals(Val* other) {
    NumVal* o = dynamic_cast<NumVal*>(other);
    return o != nullptr && val == o->val;
}

Val* NumVal::add_to(Val* other) {
    NumVal* o = dynamic_cast<NumVal*>(other);
    if (o == nullptr)
        throw std::runtime_error("Adding non-numbers");
    return new NumVal(val + o->val);
}

Val* NumVal::mult_with(Val* other) {
    NumVal* o = dynamic_cast<NumVal*>(other);
    if (o == nullptr)
        throw std::runtime_error("Multiplying non-numbers");
    return new NumVal(val * o->val);
}

std::string NumVal::to_string() {
    return std::to_string(val);
}

Expr* NumVal::to_expr() {
    return new ExprNum(val);
}

bool NumVal::is_true() {
    throw std::runtime_error("NumVal has no truth value");
}


BoolVal::BoolVal(bool val) : val(val) {}

bool BoolVal::equals(Val* other) {
    BoolVal* o = dynamic_cast<BoolVal*>(other);
    return o != nullptr && val == o->val;
}

Val* BoolVal::add_to(Val* other) {
    (void)other;
    throw std::runtime_error("Adding non-numbers");
}

Val* BoolVal::mult_with(Val* other) {
    (void)other;
    throw std::runtime_error("Multiplying non-numbers");
}

std::string BoolVal::to_string() {
    return val ? "_true" : "_false";
}

Expr* BoolVal::to_expr() {
    return new BoolExpr(val);
}

bool BoolVal::is_true() {
    return val;
}