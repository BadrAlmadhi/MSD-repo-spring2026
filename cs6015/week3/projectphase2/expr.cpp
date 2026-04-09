#include "expr.hpp"
#include "val.h"
#include <stdexcept>
#include <sstream>
#include <string>
#include "env.h"

// helper 
// return true if expression is a keyword form that usually needs special handling
static bool is_keyword_expr(PTR(Expr) e) {
    return CAST(ExprLet)(e) != nullptr ||
           CAST(IfExpr)(e) != nullptr;
}

// print an expression after a prefix, and indent later lines to match the prefix
static void print_multiline(std::ostream& out,
                            std::string prefix,
                            PTR(Expr) e,
                            precedence_t prec) {
    std::ostringstream temp;
    e->pretty_print_at(temp, prec);
    std::string s = temp.str();

    out << prefix;
    for (size_t i = 0; i < s.size(); i++) {
        out << s[i];
        if (s[i] == '\n' && i + 1 < s.size()) {
            out << std::string(prefix.size(), ' ');
        }
    }
}

// =========================
// ExprNum
// =========================

ExprNum::ExprNum(int value) {
    this->value = value;
}

bool ExprNum::equals(PTR(Expr) e) {
    PTR(ExprNum) num = CAST(ExprNum)(e);
    return num != nullptr && this->value == num->value;
}

/**
 * return a value NumVal()
 * we can use in compution
 * interp evaluate the experssion
 * gives back result
 * At runtime preduce NumVal(value); holds actual value
*/
PTR(Val) ExprNum::interp(PTR(Env) env) {
    (void)env; // silance compiler
    return NEW(NumVal)(value);
}

void ExprNum::printExpr(std::ostream& out) {
    out << value;
}

void ExprNum::pretty_print_at(std::ostream& out, precedence_t) {
    out << value;
}

// =========================
// ExprVar
// =========================

/**
 * store variable like x
 */
ExprVar::ExprVar(std::string var) {
    this->var = var;
}

bool ExprVar::equals(PTR(Expr) e) {
    PTR(ExprVar) v = CAST(ExprVar)(e);
    return v != nullptr && this->var == v->var;
}

PTR(Val) ExprVar::interp(PTR(Env) env) {
    return env->lookup(var);
}


void ExprVar::printExpr(std::ostream& out) {
    out << var;
}

void ExprVar::pretty_print_at(std::ostream& out, precedence_t) {
    out << var;
}

// =========================
// ExprAdd
// =========================

ExprAdd::ExprAdd(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool ExprAdd::equals(PTR(Expr) e) {
    PTR(ExprAdd) add = CAST(ExprAdd)(e);
    return add != nullptr &&
           lhs->equals(add->lhs) &&
           rhs->equals(add->rhs);
}

// this uses val.cpp 
PTR(Val) ExprAdd::interp(PTR(Env) env) {
    return lhs->interp(env)->add_to(rhs->interp(env));
}


void ExprAdd::printExpr(std::ostream& out) {
    out << "(";
    lhs->printExpr(out);
    out << "+";
    rhs->printExpr(out);
    out << ")";
}

void ExprAdd::pretty_print(std::ostream& out) {
    pretty_print_at(out, prec_none);
}

void ExprAdd::pretty_print_at(std::ostream& out, precedence_t prec) {
    bool need_parens = (prec == prec_add || prec == prec_mult);

    if (need_parens)
        out << "(";

    lhs->pretty_print_at(out, prec_add);
    out << " + ";

    if (is_keyword_expr(rhs))
        rhs->pretty_print_at(out, prec_add);
    else
        rhs->pretty_print_at(out, prec_none);

    if (need_parens)
        out << ")";
}

// =========================
// ExprMult
// =========================

ExprMult::ExprMult(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool ExprMult::equals(PTR(Expr) e) {
    PTR(ExprMult) mult = CAST(ExprMult)(e);
    return mult != nullptr &&
           lhs->equals(mult->lhs) &&
           rhs->equals(mult->rhs);
}

PTR(Val) ExprMult::interp(PTR(Env) env) {
    return lhs->interp(env)->mult_with(rhs->interp(env));
}

void ExprMult::printExpr(std::ostream& out) {
    out << "(";
    lhs->printExpr(out);
    out << "*";
    rhs->printExpr(out);
    out << ")";
}

void ExprMult::pretty_print(std::ostream& out) {
    pretty_print_at(out, prec_none);
}

void ExprMult::pretty_print_at(std::ostream& out, precedence_t prec) {
    bool need_parens = (prec == prec_mult);

    if (need_parens)
        out << "(";

    lhs->pretty_print_at(out, prec_mult);
    out << " * ";

    if (CAST(ExprAdd)(rhs) != nullptr ||
        CAST(EqExpr)(rhs) != nullptr ||
        is_keyword_expr(rhs))
        rhs->pretty_print_at(out, prec_mult);
    else
        rhs->pretty_print_at(out, prec_none);

    if (need_parens)
        out << ")";
}

// =========================
// ExprLet
// =========================

ExprLet::ExprLet(std::string name, PTR(Expr) rhs, PTR(Expr) body) {
    this->name = name;
    this->rhs = rhs;
    this->body = body;
}

bool ExprLet::equals(PTR(Expr) e) {
    PTR(ExprLet) let = CAST(ExprLet)(e);
    return let != nullptr &&
           name == let->name &&
           rhs->equals(let->rhs) &&
           body->equals(let->body);
}

PTR(Val) ExprLet::interp(PTR(Env) env) {
    PTR(Val) rhs_val = rhs->interp(env);
    PTR(Env) new_env = NEW(ExtendedEnv)(name, rhs_val, env);
    return body->interp(new_env);
}

void ExprLet::printExpr(std::ostream& out) {
    out << "(_let ";
    out << name;
    out << "=";
    rhs->printExpr(out);
    out << " _in ";
    body->printExpr(out);
    out << ")";
}

void ExprLet::pretty_print(std::ostream& out) {
    pretty_print_at(out, prec_none);
}

void ExprLet::pretty_print_at(std::ostream& out, precedence_t prec) {
    bool need_parens = (prec != prec_none);

    if (need_parens)
        out << "(";

    print_multiline(out, "_let " + name + " = ", rhs, prec_none);
    out << "\n";

    if (need_parens)
        out << " ";
    print_multiline(out, "_in  ", body, prec_none);

    if (need_parens)
        out << ")";
}

// =========================
// BoolExpr
// =========================

BoolExpr::BoolExpr(bool value) {
    this->value = value;
}

bool BoolExpr::equals(PTR(Expr) e) {
    PTR(BoolExpr) b = CAST(BoolExpr)(e);
    return b != nullptr && this->value == b->value;
}

PTR(Val) BoolExpr::interp(PTR(Env) env) {
    (void)env;
    return NEW(BoolVal)(value);
}

void BoolExpr::printExpr(std::ostream& out) {
    out << (value ? "_true" : "_false");
}

void BoolExpr::pretty_print(std::ostream& out) {
    printExpr(out);
}

void BoolExpr::pretty_print_at(std::ostream& out, precedence_t) {
    printExpr(out);
}

// =========================
// EqExpr
// =========================

EqExpr::EqExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool EqExpr::equals(PTR(Expr) e) {
    PTR(EqExpr) eq = CAST(EqExpr)(e);
    return eq != nullptr &&
           lhs->equals(eq->lhs) &&
           rhs->equals(eq->rhs);
}

PTR(Val) EqExpr::interp(PTR(Env) env) {
    PTR(Val) left_val = lhs->interp(env);
    PTR(Val) right_val = rhs->interp(env);
    return NEW(BoolVal)(left_val->equals(right_val));
}

void EqExpr::printExpr(std::ostream& out) {
    out << "(";
    lhs->printExpr(out);
    out << "==";
    rhs->printExpr(out);
    out << ")";
}

void EqExpr::pretty_print(std::ostream& out) {
    pretty_print_at(out, prec_none);
}

void EqExpr::pretty_print_at(std::ostream& out, precedence_t prec) {
    bool need_parens = (prec != prec_none);

    if (need_parens)
        out << "(";

    lhs->pretty_print_at(out, prec_eq);
    out << " == ";

    if (is_keyword_expr(rhs))
        rhs->pretty_print_at(out, prec_eq);
    else
        rhs->pretty_print_at(out, prec_none);

    if (need_parens)
        out << ")";
}

// =========================
// IfExpr
// =========================

IfExpr::IfExpr(PTR(Expr) test_part, PTR(Expr) then_part, PTR(Expr) else_part) {
    this->test_part = test_part;
    this->then_part = then_part;
    this->else_part = else_part;
}

bool IfExpr::equals(PTR(Expr) e) {
    PTR(IfExpr) iff = CAST(IfExpr)(e);
    return iff != nullptr &&
           test_part->equals(iff->test_part) &&
           then_part->equals(iff->then_part) &&
           else_part->equals(iff->else_part);
}

PTR(Val) IfExpr::interp(PTR(Env) env) {
    PTR(Val) test_val = test_part->interp(env);

    if (test_val->is_true())
        return then_part->interp(env);
    else
        return else_part->interp(env);
}

void IfExpr::printExpr(std::ostream& out) {
    out << "(_if ";
    test_part->printExpr(out);
    out << " _then ";
    then_part->printExpr(out);
    out << " _else ";
    else_part->printExpr(out);
    out << ")";
}

void IfExpr::pretty_print(std::ostream& out) {
    pretty_print_at(out, prec_none);
}

void IfExpr::pretty_print_at(std::ostream& out, precedence_t prec) {
    bool need_parens = (prec != prec_none);

    if (need_parens)
        out << "(";

    print_multiline(out, "_if ", test_part, prec_none);
    out << "\n";
    print_multiline(out, "_then ", then_part, prec_none);
    out << "\n";
    print_multiline(out, "_else ", else_part, prec_none);

    if (need_parens)
        out << ")";
}

//================
// FunExpr
//================

FunExpr::FunExpr(std::string formal_arg, PTR(Expr) body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunExpr::equals(PTR(Expr) e) {
    PTR(FunExpr) f = CAST(FunExpr)(e);
    return f != nullptr &&
           formal_arg == f->formal_arg &&
           body->equals(f->body);
}

PTR(Val) FunExpr::interp(PTR(Env) env) {
    return NEW(FunVal)(formal_arg, body, env);
}

void FunExpr::printExpr(std::ostream& out) {
    out << "(_fun (" << formal_arg << ") ";
    body->printExpr(out);
    out << ")";
}

void FunExpr::pretty_print(std::ostream& out) {
    pretty_print_at(out, prec_none);
}

void FunExpr::pretty_print_at(std::ostream& out, precedence_t prec) {
    bool need_parens = (prec != prec_none);

    if (need_parens)
        out << "(";

    out << "_fun (" << formal_arg << ")\n";
    print_multiline(out, "  ", body, prec_none);

    if (need_parens)
        out << ")";
}

// =====================
// CallExpr
// =====================
CallExpr::CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg) {
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}

bool CallExpr::equals(PTR(Expr) e) {
    PTR(CallExpr) c = CAST(CallExpr)(e);
    return c != nullptr &&
           to_be_called->equals(c->to_be_called) &&
           actual_arg->equals(c->actual_arg);
}

PTR(Val) CallExpr::interp(PTR(Env) env) {
    PTR(Val) fun_val = to_be_called->interp(env);

    PTR(FunVal) f = CAST(FunVal)(fun_val);
    if (f == nullptr)
        throw std::runtime_error("not a function");

    PTR(Val) arg_val = actual_arg->interp(env);
    PTR(Env) new_env = NEW(ExtendedEnv)(f->formal_arg, arg_val, f->env);

    return f->body->interp(new_env);
}

void CallExpr::printExpr(std::ostream& out) {
    to_be_called->printExpr(out);
    out << "(";
    actual_arg->printExpr(out);
    out << ")";
}

void CallExpr::pretty_print(std::ostream& out) {
    pretty_print_at(out, prec_none);
}

void CallExpr::pretty_print_at(std::ostream& out, precedence_t) {
    to_be_called->pretty_print(out);
    out << "(";
    actual_arg->pretty_print(out);
    out << ")";
}