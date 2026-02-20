#include "parse.hpp"
#include <cctype> // allow us to use isspace,, isdigit, isalpha, isalunm
#include <sstream> // string stream
#include <stdexcept> // run time error
#include "parse.hpp"
#include "expr.hpp"

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
    int n = 0;
    bool seen_digit = false;

    while (std::isdigit(in.peek())) {
        seen_digit = true;
        // convert characters into int
        n = n * 10 + (in.get() - '0');
    }

    if (!seen_digit) {
        throw std::runtime_error("expected number");
    }
    return n;
}

// forward declaration
static Expr* parse_expr(std::istream &in);       
static Expr* parse_addend(std::istream &in);
static Expr* parse_multicand(std::istream &in);
static Expr* parse_inner(std::istream &in);

// parse the smallest pieces : numbers, variable, parentheses, and _let

static Expr* parse_inner(std::istream &in) {
    skip_ws(in);

    int c = in.peek();
    if (c == EOF)
        throw std::runtime_error("unexpected end of input");

    // number
    if (std::isdigit(c)) {
        int n = parse_number(in);
        return new ExprNum(n);
    }

    // variable name
    if (std::isalpha(c)) {
        std::string name = parse_identifier(in);
        return new ExprVar(name);
    }

    // parentheses: ( expr )
    if (c == '(') {
        consume(in, '(');
        Expr* e = parse_expr(in);
        skip_ws(in);
        consume(in, ')');
        return e;
    }

    // _let x = expr _in expr
    if (c == '_') {
        consume(in, '_');
        std::string kw = parse_identifier(in);  // expects "let"

        if (kw != "let")
            throw std::runtime_error("unknown keyword");

        skip_ws(in);
        std::string var = parse_identifier(in);

        skip_ws(in);
        consume(in, '=');

        Expr* rhs = parse_expr(in);

        skip_ws(in);
        consume(in, '_');
        std::string in_kw = parse_identifier(in);
        if (in_kw != "in")
            throw std::runtime_error("expected _in");

        Expr* body = parse_expr(in);

        return new ExprLet(var, rhs, body);
    }

    throw std::runtime_error("bad expression");
}

// Parse multiplication level: inner ( * inner )*
static Expr* parse_multicand(std::istream &in) {
    // First, parse the left-most inner expression
    Expr* e = parse_inner(in);
    skip_ws(in);

    // If we see '*', keep building a left-associative chain:
    // a*b*c becomes Mult(Mult(a,b),c)
    while (in.peek() == '*') {
        consume(in, '*');
        Expr* rhs = parse_inner(in);
        e = new ExprMult(e, rhs);
        skip_ws(in);
    }

    return e;
}

// Parse addition level: multicand ( + multicand )*
static Expr* parse_addend(std::istream &in) {
    // First, parse the left-most multicand
    Expr* e = parse_multicand(in);
    skip_ws(in);

    // Build a left-associative chain for '+'
    while (in.peek() == '+') {
        consume(in, '+');
        Expr* rhs = parse_multicand(in);
        e = new ExprAdd(e, rhs);
        skip_ws(in);
    }

    return e;
}

// Top-level expression (for your current language, expr == addend)
static Expr* parse_expr(std::istream &in) {
    return parse_addend(in);
}

// Public parse functions
Expr* parse(std::istream &in) {
    // Parse one expression
    Expr* e = parse_expr(in);

    // After parsing, allow whitespace
    skip_ws(in);

    // If there is any non-whitespace left, that's an error.
    // This catches things like: "1 2" or "x +"
    if (in.peek() != EOF)
        throw std::runtime_error("unexpected input after expression");

    return e;
}

Expr* parse_str(const std::string &s) {
    // Used in tests: make a stream from a string, then call parse(stream)
    std::istringstream in(s);
    return parse(in);
}