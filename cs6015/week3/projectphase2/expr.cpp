#include "expr.hpp"
#include "val.h"
#include <stdexcept>
#include <sstream>
#include <string>

// helper 
// return true if expression is a keyword form that usually needs special handling
static bool is_keyword_expr(Expr* e) {
    return dynamic_cast<ExprLet*>(e) != nullptr ||
           dynamic_cast<IfExpr*>(e) != nullptr;
}

// print an expression after a prefix, and indent later lines to match the prefix
static void print_multiline(std::ostream& out,
                            std::string prefix,
                            Expr* e,
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

bool ExprNum::equals(Expr* e) {
    ExprNum* num = dynamic_cast<ExprNum*>(e);
    return num != nullptr && this->value == num->value;
}

Val* ExprNum::interp() {
    return new NumVal(value);
}



Expr* ExprNum::subst(std::string s, Expr* e) {
    (void)s;
    (void)e;
    return new ExprNum(value);
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

ExprVar::ExprVar(std::string var) {
    this->var = var;
}

bool ExprVar::equals(Expr* e) {
    ExprVar* v = dynamic_cast<ExprVar*>(e);
    return v != nullptr && this->var == v->var;
}

Val* ExprVar::interp() {
    throw std::runtime_error("free variable: " + var);
}



Expr* ExprVar::subst(std::string s, Expr* e) {
    if (var == s)
        return e;
    return new ExprVar(var);
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

ExprAdd::ExprAdd(Expr* lhs, Expr* rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool ExprAdd::equals(Expr* e) {
    ExprAdd* add = dynamic_cast<ExprAdd*>(e);
    return add != nullptr &&
           lhs->equals(add->lhs) &&
           rhs->equals(add->rhs);
}

Val* ExprAdd::interp() {
    return lhs->interp()->add_to(rhs->interp());
}



Expr* ExprAdd::subst(std::string s, Expr* e) {
    return new ExprAdd(lhs->subst(s, e), rhs->subst(s, e));
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

ExprMult::ExprMult(Expr* lhs, Expr* rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool ExprMult::equals(Expr* e) {
    ExprMult* mult = dynamic_cast<ExprMult*>(e);
    return mult != nullptr &&
           lhs->equals(mult->lhs) &&
           rhs->equals(mult->rhs);
}

Val* ExprMult::interp() {
    return lhs->interp()->mult_with(rhs->interp());
}



Expr* ExprMult::subst(std::string s, Expr* e) {
    return new ExprMult(lhs->subst(s, e), rhs->subst(s, e));
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

    if (dynamic_cast<ExprAdd*>(rhs) != nullptr ||
        dynamic_cast<EqExpr*>(rhs) != nullptr ||
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

ExprLet::ExprLet(std::string name, Expr* rhs, Expr* body) {
    this->name = name;
    this->rhs = rhs;
    this->body = body;
}

bool ExprLet::equals(Expr* e) {
    ExprLet* let = dynamic_cast<ExprLet*>(e);
    return let != nullptr &&
           name == let->name &&
           rhs->equals(let->rhs) &&
           body->equals(let->body);
}

Val* ExprLet::interp() {
    Val* rhs_val = rhs->interp();
    Expr* new_body = body->subst(name, rhs_val->to_expr());
    return new_body->interp();
}



Expr* ExprLet::subst(std::string s, Expr* e) {
    Expr* new_rhs = rhs->subst(s, e);

    if (name == s) {
        return new ExprLet(name, new_rhs, body);
    }
    else {
        return new ExprLet(name, new_rhs, body->subst(s, e));
    }
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

bool BoolExpr::equals(Expr* e) {
    BoolExpr* b = dynamic_cast<BoolExpr*>(e);
    return b != nullptr && this->value == b->value;
}

Val* BoolExpr::interp() {
    return new BoolVal(value);
}



Expr* BoolExpr::subst(std::string s, Expr* e) {
    (void)s;
    (void)e;
    return new BoolExpr(value);
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

EqExpr::EqExpr(Expr* lhs, Expr* rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool EqExpr::equals(Expr* e) {
    EqExpr* eq = dynamic_cast<EqExpr*>(e);
    return eq != nullptr &&
           lhs->equals(eq->lhs) &&
           rhs->equals(eq->rhs);
}

Val* EqExpr::interp() {
    Val* left_val = lhs->interp();
    Val* right_val = rhs->interp();
    return new BoolVal(left_val->equals(right_val));
}


Expr* EqExpr::subst(std::string s, Expr* e) {
    return new EqExpr(lhs->subst(s, e), rhs->subst(s, e));
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

IfExpr::IfExpr(Expr* test_part, Expr* then_part, Expr* else_part) {
    this->test_part = test_part;
    this->then_part = then_part;
    this->else_part = else_part;
}

bool IfExpr::equals(Expr* e) {
    IfExpr* iff = dynamic_cast<IfExpr*>(e);
    return iff != nullptr &&
           test_part->equals(iff->test_part) &&
           then_part->equals(iff->then_part) &&
           else_part->equals(iff->else_part);
}

Val* IfExpr::interp() {
    Val* test_val = test_part->interp();

    if (test_val->is_true())
        return then_part->interp();
    else
        return else_part->interp();
}



Expr* IfExpr::subst(std::string s, Expr* e) {
    return new IfExpr(
        test_part->subst(s, e),
        then_part->subst(s, e),
        else_part->subst(s, e)
    );
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

FunExpr::FunExpr(std::string formal_arg, Expr* body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunExpr::equals(Expr* e) {
    FunExpr* f = dynamic_cast<FunExpr*>(e);
    return f != nullptr &&
           formal_arg == f->formal_arg &&
           body->equals(f->body);
}

Val* FunExpr::interp() {
    return new FunVal(formal_arg, body);
}

Expr* FunExpr::subst(std::string s, Expr* e) {
    if (formal_arg == s)
        return new FunExpr(formal_arg, body);
    return new FunExpr(formal_arg, body->subst(s, e));
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
CallExpr::CallExpr(Expr* to_be_called, Expr* actual_arg) {
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}

bool CallExpr::equals(Expr* e) {
    CallExpr* c = dynamic_cast<CallExpr*>(e);
    return c != nullptr &&
           to_be_called->equals(c->to_be_called) &&
           actual_arg->equals(c->actual_arg);
}

Val* CallExpr::interp() {
    Val* fun_val = to_be_called->interp();

    FunVal* f = dynamic_cast<FunVal*>(fun_val);
    if (f == nullptr)
        throw std::runtime_error("not a function");

    Val* arg_val = actual_arg->interp();

    Expr* new_body = f->body->subst(f->formal_arg, arg_val->to_expr());
    return new_body->interp();
}

Expr* CallExpr::subst(std::string s, Expr* e) {
    return new CallExpr(
        to_be_called->subst(s, e),
        actual_arg->subst(s, e)
    );
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