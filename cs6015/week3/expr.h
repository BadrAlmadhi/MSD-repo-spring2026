
#pragma once // prevent header from beging included multiple times. (avoid duplicate)
#include <string> // for variables

// base class  
class Expr {
  public:
  virtual ~Expr() = default;
  virtual bool equals(Expr *e) = 0; // each subclass must override
};

// Num represent number expression (leaf node in the tree)
class Num : public Expr {
    public:
        int val; // actual value

        // constructor declaration
        Num(int val);

    bool equals(Expr *e) override;
};

// represent addition of two expression
class Add : public Expr {
    public: 
        Expr *lhs; // left
        Expr *rhs; // right

        // declare constructor
        Add(Expr *lhs, Expr *rhs);

    bool equals(Expr *e) override;

};

// represent multiplication 
class Mult : public Expr {
    public: 
        Expr *lhs; // left
        Expr *rhs; // right 

        // declare constructor
        Mult(Expr *lhs, Expr *rhs);
    
    bool equals(Expr *e) override;
};