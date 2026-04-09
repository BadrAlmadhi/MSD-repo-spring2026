#include "val.h"
#include "expr.hpp"
#include <stdexcept>
#include <climits>
#include "env.h"

NumVal::NumVal(int val){
    this->val = val;
}

/**
 * try convert other into NumVal*
 */
bool NumVal::equals(PTR(Val) other) {
    // convert other to NumVal* because other can be BoolVal, FunVal
    PTR(NumVal) o = CAST(NumVal)(other);
    // if other is really NumVal compare stored integer
    return o != nullptr && val == o->val;
}

/**
 * Addistion only allowed with number
 */
PTR(Val) NumVal::add_to(PTR(Val) other) {
    // convert other to NumVal
    PTR(NumVal) o = CAST(NumVal)(other);
    if (o == nullptr)
        // only accept two numbers
        // 5 + _true = not allowed
        throw std::runtime_error("Adding non-numbers");

    // check over flow long long gives begger range
    long long result = (long long)val+(long long) o->val;
    if (result > INT_MAX || result < INT_MIN)
        throw std::runtime_error("integer overflow in addition");
        // retrun converted int and retrun numeric value
    return NEW(NumVal)((int)result);
}

// same idea as add_to
PTR(Val) NumVal::mult_with(PTR(Val) other) {
    PTR(NumVal) o = CAST(NumVal)(other);
    if (o == nullptr)
        throw std::runtime_error("Multiplying non-numbers");

    long long result = (long long)val * (long long)o->val;

    if (result > INT_MAX || result < INT_MIN)
        throw std::runtime_error("integer overflow in multiplication");

    return NEW(NumVal)((int)result);
}

// converts stored int into string 
std::string NumVal::to_string() {
    return std::to_string(val);
}

// convert to expr
// needed for substittution 
// while subts we need Expr not val
PTR(Expr) NumVal::to_expr() {
    return NEW(ExprNum)(val);
}

// 
bool NumVal::is_true() {
    throw std::runtime_error("NumVal has no truth value");
}


BoolVal::BoolVal(bool val) {
    this->val = val;
}

// equallity
bool BoolVal::equals(PTR(Val) other) {
    // convert to BoolVal
    PTR(BoolVal) o = CAST(BoolVal)(other);
    // then compare BoolVal
    return o != nullptr && val == o->val;
}

// BoolVal cannot be added
PTR(Val) BoolVal::add_to(PTR(Val) other) {
    (void)other; // silence unused parameter waring
    throw std::runtime_error("Adding non-numbers");
}

PTR(Val) BoolVal::mult_with(PTR(Val) other) {
    (void)other; // cannot be mult
    throw std::runtime_error("Multiplying non-numbers");
}

std::string BoolVal::to_string() {
    // if bool is true return true if not return false
    return val ? "_true" : "_false";
}

// make it Expr
PTR(Expr) BoolVal::to_expr() {
    return NEW(BoolExpr)(val);
}

// BoolVal::is_true() when it's runs in expr.cpp return BoolVal::is_true()
bool BoolVal::is_true() {
    return val;
}

// FunVal
FunVal::FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env) {
    this->formal_arg = formal_arg;
    this->body = body;
    this->env = env;
}

bool FunVal::equals(PTR(Val) other) {
    // convert to FunVal 
    PTR(FunVal) o = CAST(FunVal)(other);
    return o != nullptr &&
    // compare if both are function 
           formal_arg == o->formal_arg &&
           body->equals(o->body);
}

PTR(Val) FunVal::add_to(PTR(Val) other) {
    (void)other; // cannot add function 
    throw std::runtime_error("Adding non-numbers");
}

PTR(Val) FunVal::mult_with(PTR(Val) other) {
    (void)other; // cannot mult function 
    throw std::runtime_error("Multiplying non-numbers");
}

std::string FunVal::to_string() {
    return "[function]"; // just prin [function]
}

PTR(Expr) FunVal::to_expr() {
    // convert to ExprFun
    return NEW(FunExpr)(formal_arg, body);
}

// prevent nonsense 
bool FunVal::is_true() {
    throw std::runtime_error("function is not a boolean");
}