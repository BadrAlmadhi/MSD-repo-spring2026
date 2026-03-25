#include "val.h"
#include "expr.hpp"
#include <stdexcept>
#include <climits>

NumVal::NumVal(int val) : val(val) {}

bool NumVal::equals(Val* other) {
    NumVal* o = dynamic_cast<NumVal*>(other);
    return o != nullptr && val == o->val;
}

Val* NumVal::add_to(Val* other) {
    NumVal* o = dynamic_cast<NumVal*>(other);
    if (o == nullptr)
        throw std::runtime_error("Adding non-numbers");

    long long result = (long long)val+(long long) o->val;
    if (result > INT_MAX || result < INT_MIN)
        throw std::runtime_error("integer overflow in addition");
    return new NumVal((int)result);
}

Val* NumVal::mult_with(Val* other) {
    NumVal* o = dynamic_cast<NumVal*>(other);
    if (o == nullptr)
        throw std::runtime_error("Multiplying non-numbers");

    long long result = (long long)val * (long long)o->val;

    if (result > INT_MAX || result < INT_MIN)
        throw std::runtime_error("integer overflow in multiplication");

    return new NumVal((int)result);
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

// FunVal
FunVal::FunVal(std::string formal_arg, Expr* body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunVal::equals(Val* other) {
    FunVal* o = dynamic_cast<FunVal*>(other);
    return o != nullptr &&
           formal_arg == o->formal_arg &&
           body->equals(o->body);
}

Val* FunVal::add_to(Val* other) {
    (void)other;
    throw std::runtime_error("Adding non-numbers");
}

Val* FunVal::mult_with(Val* other) {
    (void)other;
    throw std::runtime_error("Multiplying non-numbers");
}

std::string FunVal::to_string() {
    return "[function]";
}

Expr* FunVal::to_expr() {
    return new FunExpr(formal_arg, body);
}

bool FunVal::is_true() {
    throw std::runtime_error("function is not a boolean");
}