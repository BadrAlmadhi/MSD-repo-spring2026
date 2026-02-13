#include "expr.hpp"
#include <string>
#include <stdexcept>
#include <ios>

// provides string stream classes that allow 
// developers to perform input and output 
// operations on strings in memory, treating 
// a string object like a stream.
#include <sstream>


/**
 * Helper function that figure out how far we are from the start of Current Line
 * count from last \n
 */

 static int current_column(std::ostream& out) {
    // since to_pretty_string use stringstream 
    auto* ss = dynamic_cast<std::stringstream*>(&out);
    if (ss == nullptr) return 0; // we want to make sure ss is a pointer

    std::string s = ss->str(); // everything inside stream
    size_t last_newline = s.rfind("\n"); // find last \na

    if (last_newline == std::string::npos) return (int)s.size(); // first line

    return (int)(s.size() - last_newline -1); // char after '\n'

 }


// initialize value in constructor
// Num
ExprNum::ExprNum(int value) {
    this->value = value;
}

// equal function for Num
bool ExprNum::equals(Expr* e) {
    // dynamic_cast convert Expr* to ExprNum*
    ExprNum* other = dynamic_cast<ExprNum*>(e);
    if (other == nullptr) return false;
    return value == other->value;
}

// interp for Num
int ExprNum::interp() {
    return value;
}

// Num has variable
bool ExprNum::has_variable() {
    return false;
}

// Num printExpr
void ExprNum::printExpr(std::ostream& out) {
    // since parameter is ostream we can use <<
    out << value;
}


// for subst just return new value
Expr* ExprNum::subst(std::string, Expr*) {
    return new ExprNum(value);
}

// Add constructor
ExprAdd::ExprAdd(Expr* lhs, Expr* rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

// equal function for Add
bool ExprAdd::equals(Expr* e){
    ExprAdd* other = dynamic_cast<ExprAdd*>(e);

    if (other == nullptr) return false;
    
    // we use recursion to compare values
    return lhs->equals(other->lhs) && rhs->equals(other->rhs);
}

// interp for Add
int ExprAdd::interp() {
    return lhs->interp() + rhs->interp();
}

// has_variable for Add
bool ExprAdd::has_variable() {
    if (lhs->has_variable() || rhs->has_variable()) {
        return true;
    } else {
        return false;
    }
}

//subs for Add
Expr* ExprAdd::subst(std::string s, Expr* e) {
    Expr* lhs_sub = lhs->subst(s, e);
    Expr* rhs_sub = rhs->subst(s,e);
    return new ExprAdd(lhs_sub, rhs_sub);
}

// printExpr for add
void ExprAdd::printExpr(std::ostream& out) {
    out << "(";
    // pinter to printExpr
    lhs->printExpr(out);
    out << "+";
    rhs->printExpr(out);
    out << ")";
}

void ExprAdd::pretty_print(std::ostream& out) {
    pretty_print_at(out, prec_none);
}

void ExprAdd::pretty_print_at(std::ostream& out, precedence_t prec) {
    bool need_parent = (prec == prec_mult) || (prec == prec_add);
    if (need_parent) out << "(";

    // left side this tells it's in the left side of add
    lhs->pretty_print_at(out, prec_add);

    out << " + ";

    // right side allow 
    rhs->pretty_print_at(out, prec_none);

    if (need_parent) out << ")";
}

// Mult constructor
ExprMult::ExprMult(Expr* lhs, Expr* rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

// Mult equals
bool ExprMult::equals(Expr* e){
    ExprMult* other = dynamic_cast<ExprMult*>(e);
    if (other == nullptr) return false;
    return lhs->equals(other->lhs) && rhs->equals(other->rhs);
}

// Mult interp
int ExprMult::interp() {
    return lhs->interp() * rhs->interp();
}

// Mult has_variable
bool ExprMult::has_variable() {
    if (lhs->has_variable() || rhs->has_variable() ) {
        return true;
    } else {
        return false;
    }
}

// Mult subst
Expr* ExprMult::subst(std::string s, Expr* e) {
    Expr* lhs_sub = lhs->subst(s,e);
    Expr* rhs_sub = rhs->subst(s,e);
    return new ExprMult(lhs_sub, rhs_sub);
}

// Print expression Mult
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
    bool need_parens = (prec == prec_mult); // left child of another mult

    if (need_parens) out << "(";

    // left: force left association parentheses for (a*b)*c
    lhs->pretty_print_at(out, prec_mult);

    out << " * ";

    if (dynamic_cast<ExprAdd*>(rhs) != nullptr) {
        rhs->pretty_print_at(out, prec_mult);
    } else {
        rhs->pretty_print_at(out, prec_none);
    }

    if (need_parens) out << ")";
}

// Val
ExprVar::ExprVar(std::string var){
    this->var = var;
}

// equals for Var
bool ExprVar::equals(Expr* e){
    ExprVar* other = dynamic_cast<ExprVar*>(e);
    if (other == nullptr) return false;
    return var == other->var;
}

// interp for var
int ExprVar::interp() {
    throw std::runtime_error("This expression cannot be evaluated because it contain a variable");
}

// Var has_variable
bool ExprVar::has_variable() {
    return true;
}

// just return new variable type
Expr* ExprVar::subst(std::string s, Expr* e) {
    // we want to return the expr not the string
    if (var == s) {
        return e;
    } else {
        return new ExprVar(var);
    }
}

// print expr Val 
void ExprVar::printExpr(std::ostream& out) {
    out << var;
}


//----------
// Let
// ----------

// constructor
ExprLet::ExprLet(std::string name, Expr* rhs, Expr* body) {
    this->name = name;
    this->rhs = rhs;
    this->body = body;
}

bool ExprLet::equals(Expr* e) {
    ExprLet* other = dynamic_cast<ExprLet*>(e);
    if (other == nullptr) return false;
    return name == other->name // name is not an Expr use regular comparison
        && rhs->equals(other->rhs)
        && body->equals(other->body);
}

// interp is evaluating rhs, substitute into body, then interpret body
int ExprLet::interp() {
    int v = rhs->interp();
    Expr* new_body = body->subst(name, new ExprNum(v));
    return new_body->interp();
}

// has_variable: only rhs/body matter (name itself doesn't count)
bool ExprLet::has_variable() {
    return rhs->has_variable() || body->has_variable();
}

// subst: always substitute in rhs.
// In body: if let binds the same name, don't substitute into body (shadowing)
Expr* ExprLet::subst(std::string s, Expr* e) {
    Expr* new_rhs = rhs->subst(s, e);

    if (name == s) {
        // shadowing: do not substitute into body
        return new ExprLet(name, new_rhs, body);
    } else {
        Expr* new_body = body->subst(s, e);
        return new ExprLet(name, new_rhs, new_body);
    }
}

// printExpr: always parenthesized, no spaces around '='
// single spaces after _let and before/after _in
void ExprLet::printExpr(std::ostream& out) {
    out << "(_let " << name << "=";
    rhs->printExpr(out);
    out << " _in ";
    body->printExpr(out);
    out << ")";
}

// pretty_print: for now, call helper with prec_none
void ExprLet::pretty_print(std::ostream& out) {
    pretty_print_at(out, prec_none);
}


void ExprLet::pretty_print_at(std::ostream& out, precedence_t prec) {
    // add parentheses if needed
    bool need_parens = (prec == prec_add) || (prec == prec_mult);

    if (need_parens) out << "(";

    // find where _let start on this line
    int let_column = current_column(out);

    // print first line
    out << "_let " << name << " = ";
    rhs->pretty_print_at(out, prec_none);

    out << "\n";

    // indent "_in" to line up under _let
    out << std::string(let_column, ' ');
    out << "_in  ";

    body->pretty_print_at(out, prec_none);

    if (need_parens) out << ")";
}