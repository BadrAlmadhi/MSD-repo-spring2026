#include "expr.hpp"
#include "val.h"
#include <stdexcept>

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

bool ExprNum::has_variable() {
    return false;
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

bool ExprVar::has_variable() {
    return true;
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

bool ExprAdd::has_variable() {
    return lhs->has_variable() || rhs->has_variable();
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

bool ExprMult::has_variable() {
    return lhs->has_variable() || rhs->has_variable();
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

    if (dynamic_cast<ExprAdd*>(rhs) != nullptr || dynamic_cast<EqExpr*>(rhs) != nullptr)
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

bool ExprLet::has_variable() {
    return rhs->has_variable() || body->has_variable();
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

    out << "_let " << name << " = ";
    rhs->pretty_print_at(out, prec_none);
    out << "\n_in  ";
    body->pretty_print_at(out, prec_none);

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

bool BoolExpr::has_variable() {
    return false;
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

bool EqExpr::has_variable() {
    return lhs->has_variable() || rhs->has_variable();
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

bool IfExpr::has_variable() {
    return test_part->has_variable() ||
           then_part->has_variable() ||
           else_part->has_variable();
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

    out << "_if ";
    test_part->pretty_print_at(out, prec_none);
    out << "\n_then ";
    then_part->pretty_print_at(out, prec_none);
    out << "\n_else ";
    else_part->pretty_print_at(out, prec_none);

    if (need_parens)
        out << ")";
}