#include <iostream>
#include "parse.hpp"

static void consume(std::istream &in, int expect);
void skip_whitespace(std::istream &in);
Expr *parse_num(std::istream &inn);

Expr *parse_expr (std::istream & inn) {
    skip_whitespace (inn);
    
    int c = inn.peek();
    if ((c == '-') || isdigit(c))
      return parse_num(inn);
    else if (c == '(') {
      consume(inn, '(');
      Expr *e = parse_expr(inn);
      skip_whitespace(inn);
      c = inn.get();
      if (c != ')')
        throw std::runtime_error("missing close parenthesis");
      return e;
    } else {
      consume(inn, c);
      throw std::runtime_error("invalid input");
  }
}

Expr *parse_num(std::istream &inn) {
    int n = 0;
    bool negative = false;
    
    if (inn.peek() == '-') {
        negative = true;
        consume (inn, '-');
        
    }

    while (1) {
        int c = inn.peek(); 
        if (isdigit(c))
        {
            consume(inn, c);
            n = n*10 + (c - '0');
        }
        else
            break;
    }
    
    if (negative)
        n = -n;
  
    return new NumExpr(n);
}


static void consume(std::istream &in, int expect) {
    int c = in.get();
    if (c!=expect) {
        throw std::runtime_error("consume mismatch");
    }
}

void skip_whitespace(std::istream &in) {
  while (1) {
    int c = in.peek();
    if (!isspace(c))
      break;
    consume(in, c);
      }
}
