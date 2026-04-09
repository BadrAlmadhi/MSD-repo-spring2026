#include "val.h"
#include "expr.hpp"
#include <stdexcept>
#include <climits>

NumVal::NumVal(int val){
    this->val = val;
}

/**
 * try convert other into NumVal*
 */
bool NumVal::equals(Val* other) {
    // convert other to NumVal* because other can be BoolVal, FunVal
    NumVal* o = dynamic_cast<NumVal*>(other);
    // if other is really NumVal compare stored integer
    return o != nullptr && val == o->val;
}

/**
 * Addistion only allowed with number
 */
Val* NumVal::add_to(Val* other) {
    // convert other to NumVal
    NumVal* o = dynamic_cast<NumVal*>(other);
    if (o == nullptr)
        // only accept two numbers
        // 5 + _true = not allowed
        throw std::runtime_error("Adding non-numbers");

    // check over flow long long gives begger range
    long long result = (long long)val+(long long) o->val;
    if (result > INT_MAX || result < INT_MIN)
        throw std::runtime_error("integer overflow in addition");
        // retrun converted int and retrun numeric value
    return new NumVal((int)result);
}

// same idea as add_to
Val* NumVal::mult_with(Val* other) {
    NumVal* o = dynamic_cast<NumVal*>(other);
    if (o == nullptr)
        throw std::runtime_error("Multiplying non-numbers");

    long long result = (long long)val * (long long)o->val;

    if (result > INT_MAX || result < INT_MIN)
        throw std::runtime_error("integer overflow in multiplication");

    return new NumVal((int)result);
}

// converts stored int into string 
std::string NumVal::to_string() {
    return std::to_string(val);
}

// convert to expr
// needed for substittution 
// while subts we need Expr not val
Expr* NumVal::to_expr() {
    return new ExprNum(val);
}

// 
bool NumVal::is_true() {
    throw std::runtime_error("NumVal has no truth value");
}


BoolVal::BoolVal(bool val) {
    this->val = val;
}

// equallity
bool BoolVal::equals(Val* other) {
    // convert to BoolVal
    BoolVal* o = dynamic_cast<BoolVal*>(other);
    // then compare BoolVal
    return o != nullptr && val == o->val;
}

// BoolVal cannot be added
Val* BoolVal::add_to(Val* other) {
    (void)other; // silence unused parameter waring
    throw std::runtime_error("Adding non-numbers");
}

Val* BoolVal::mult_with(Val* other) {
    (void)other; // cannot be mult
    throw std::runtime_error("Multiplying non-numbers");
}

std::string BoolVal::to_string() {
    // if bool is true return true if not return false
    return val ? "_true" : "_false";
}

// make it Expr
Expr* BoolVal::to_expr() {
    return new BoolExpr(val);
}

// BoolVal::is_true() when it's runs in expr.cpp return BoolVal::is_true()
bool BoolVal::is_true() {
    return val;
}

// FunVal
FunVal::FunVal(std::string formal_arg, Expr* body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunVal::equals(Val* other) {
    // convert to FunVal 
    FunVal* o = dynamic_cast<FunVal*>(other);
    return o != nullptr &&
    // compare if both are function 
           formal_arg == o->formal_arg &&
           body->equals(o->body);
}

Val* FunVal::add_to(Val* other) {
    (void)other; // cannot add function 
    throw std::runtime_error("Adding non-numbers");
}

Val* FunVal::mult_with(Val* other) {
    (void)other; // cannot mult function 
    throw std::runtime_error("Multiplying non-numbers");
}

std::string FunVal::to_string() {
    return "[function]"; // just prin [function]
}

Expr* FunVal::to_expr() {
    // convert to ExprFun
    return new FunExpr(formal_arg, body);
}

// prevent nonsense 
bool FunVal::is_true() {
    throw std::runtime_error("function is not a boolean");
}