#include "parse.hpp"
#include <cctype> // allow us to use isspace,, isdigit, isalpha, isalunm
#include <sstream> // string stream
#include <stdexcept> // run time error
#include "parse.hpp"
#include "expr.hpp"
#include <climits>

// helper functions 

// 1 skip whitespaces characters (spaces, tabs, newlines) 
// this let parser ignore formatting like "1 +   2"
static void skip_ws(std::istream &in) {
    while (std::isspace(in.peek()))
        in.get();
}

// consume one character from the stream
// if next character is not what we expect, we throw an error
static void consume(std::istream &in, char expected) {
    int got = in.get();
    if (got != expected)
        throw std::runtime_error("unexpected character");
}

// parse an identifier such as x, abc123, my_var, _let, _in
// read from input as long as they look like a valid identifier

static std::string parse_identifier(std::istream &in) {
    std::string s;

    // read letters/digits/underscore
    while (std::isalnum(in.peek()) || in.peek() == '_') {
        s.push_back(char(in.get()));
    }

    if (s == "")
        throw std::runtime_error("expected identifier");

    return s;
}


// parse non negative integer
static int parse_number(std::istream &in) {
    unsigned int n = 0;
    bool seen_digit = false;

    while (std::isdigit(in.peek())) {
        seen_digit = true;
        unsigned int digit = (unsigned int)(in.get() - '0');

        if (n > (((unsigned int)INT_MAX) - digit) / 10u) {
            throw std::runtime_error("number constant is too big");
        }

        n = n * 10u + digit;
    }

    if (!seen_digit) {
        throw std::runtime_error("expected number");
    }

    return (int)n;
}

// forward declaration
static PTR(Expr) parse_expr(std::istream &in);      
static PTR(Expr) parse_comparg(std::istream &in); 
static PTR(Expr) parse_addend(std::istream &in);
static PTR(Expr) parse_multicand(std::istream &in);
static PTR(Expr) parse_inner(std::istream &in);
static PTR(Expr) parse_call(std::istream &in);

// parse the smallest pieces : numbers, variable, parentheses, and _let

// parse the smallest pieces : numbers, variable, parentheses, booleans, _if, and _let

static PTR(Expr) parse_inner(std::istream &in) {
    skip_ws(in);

    int c = in.peek();
    if (c == EOF)
        throw std::runtime_error("unexpected end of input");

    // negative number
    if (c == '-') {
        consume(in, '-');

        if (!std::isdigit(in.peek()))
            throw std::runtime_error("bad expression");

        int n = parse_number(in);
        return NEW(ExprNum)(-n);
    }

    // number
    if (std::isdigit(c)) {
        int n = parse_number(in);
        return NEW(ExprNum)(n);
    }

    // variable name
    if (std::isalpha(c)) {
        std::string name = parse_identifier(in);
        return NEW(ExprVar)(name);
    }

    // parentheses: ( expr )
    if (c == '(') {
        consume(in, '(');
        PTR(Expr) e = parse_expr(in);
        skip_ws(in);
        consume(in, ')');
        return e;
    }

    // keywords that start with _
    // _let x = expr _in expr
    // _true
    // _false
    // _if expr _then expr _else expr
    // _fun (x) expr
    if (c == '_') {
        consume(in, '_');
        std::string kw = parse_identifier(in);

        // _let x = expr _in expr
        if (kw == "let") {
            skip_ws(in);
            std::string var = parse_identifier(in);

            skip_ws(in);
            consume(in, '=');

            PTR(Expr) rhs = parse_expr(in);

            skip_ws(in);
            consume(in, '_');
            std::string in_kw = parse_identifier(in);

            if (in_kw != "in")
                throw std::runtime_error("expected _in");

            PTR(Expr) body = parse_expr(in);

            return NEW(ExprLet)(var, rhs, body);
        }

        // boolean true
        if (kw == "true") {
            return NEW(BoolExpr)(true);
        }

        // boolean false
        if (kw == "false") {
            return NEW(BoolExpr)(false);
        }

        // _if expr _then expr _else expr
        if (kw == "if") {
            PTR(Expr) test_part = parse_expr(in);

            skip_ws(in);
            consume(in, '_');
            std::string then_kw = parse_identifier(in);

            if (then_kw != "then")
                throw std::runtime_error("expected _then");

            PTR(Expr) then_part = parse_expr(in);

            skip_ws(in);
            consume(in, '_');
            std::string else_kw = parse_identifier(in);

            if (else_kw != "else")
                throw std::runtime_error("expected _else");

            PTR(Expr) else_part = parse_expr(in);

            return NEW(IfExpr)(test_part, then_part, else_part);
        }

        // _fun (x) expr
        if (kw == "fun") {
            skip_ws(in);
            consume(in, '(');
            std::string arg = parse_identifier(in);
            skip_ws(in);
            consume(in, ')');

            PTR(Expr) body = parse_expr(in);
            return NEW(FunExpr)(arg, body);
        }

        throw std::runtime_error("unknown keyword");
    }

    throw std::runtime_error("bad expression");
}

// Parse multiplication level: inner ( * inner )*
static PTR(Expr) parse_multicand(std::istream &in) {
    // First, parse the left-most inner expression
    PTR(Expr) e = parse_call(in);
    skip_ws(in);

    // If we see '*', keep building a left-associative chain:
    // a*b*c becomes Mult(Mult(a,b),c)
    while (in.peek() == '*') {
        consume(in, '*');
        PTR(Expr) rhs = parse_call(in);
        e = NEW(ExprMult)(e, rhs);
        skip_ws(in);
    }

    return e;
}

// Parse addition level: multicand ( + multicand )*
static PTR(Expr) parse_addend(std::istream &in) {
    // First, parse the left-most multicand
    PTR(Expr) e = parse_multicand(in);
    skip_ws(in);

    // Build a left-associative chain for '+'
    while (in.peek() == '+') {
        consume(in, '+');
        PTR(Expr) rhs = parse_multicand(in);
        e = NEW(ExprAdd)(e, rhs);
        skip_ws(in);
    }

    return e;
}

// Parse equality level: addend ( == addend )*
static PTR(Expr) parse_comparg(std::istream &in) {
    // First, parse the left-most addend
    PTR(Expr) e = parse_addend(in);
    skip_ws(in);

    // Build a left-associative chain for ==
    while (in.peek() == '=') {
        consume(in, '=');
        consume(in, '=');

        PTR(Expr) rhs = parse_addend(in);
        e = NEW(EqExpr)(e, rhs);
        skip_ws(in);
    }

    return e;
}

// Parse function call level: inner ( (expr) )*
static PTR(Expr) parse_call(std::istream &in) {
    PTR(Expr) e = parse_inner(in);
    skip_ws(in);

    while (in.peek() == '(') {
        consume(in, '(');
        PTR(Expr) arg = parse_expr(in);
        skip_ws(in);
        consume(in, ')');

        e = NEW(CallExpr)(e, arg);
        skip_ws(in);
    }

    return e;
}


// Top-level expression (for your current language, expr == addend)
static PTR(Expr) parse_expr(std::istream &in) {
    return parse_comparg(in);
}

// Public parse functions
PTR(Expr) parse(std::istream &in) {
    // Parse one expression
    PTR(Expr) e = parse_expr(in);

    // After parsing, allow whitespace
    skip_ws(in);

    // If there is any non-whitespace left, that's an error.
    // This catches things like: "1 2" or "x +"
    if (in.peek() != EOF)
        throw std::runtime_error("unexpected input after expression");

    return e;
}

PTR(Expr) parse_str(const std::string &s) {
    // Used in tests: make a stream from a string, then call parse(stream)
    std::istringstream in(s);
    return parse(in);
}

