/**
 * @file Expr.h
 * @brief Expression tree classes for the msdscript interpreter.
 * 
 * Expression from a tree (AST). Each node can be evaluated to get value, 
 * printed as normal code e.g (1+2), compared to another tree to see if they have the same structure, 
 * Also, we can check if it has variables, finally, have variables replaced with expressions. 
 */

#pragma once  // prevent header from being included multiple times
#include <string> // for string variables
#include <ostream> // 

/**
 * @brief Abstract base class for all expression nodes. 
 *  
 * Subclasses must implement the core operation: equals, interp, has_variable, 
 * subst, and printExp.
 */
class Expr {
public:
    virtual ~Expr() = default; /*!< Setup Expr as default*/
    
    /**
     * @brief the equals method is going to be useed to compare expressions if they are equal or not. 
     * @param e Expression to compare against. 
     * @return true if the two expression trees are equal.
     */
    virtual bool equals(Expr* e) = 0; // each subclass must override

    /**
     * @brief Evaluate the Expr and retrun its integer value
     */
    virtual int interp() = 0;  // checks the value of expression 

    /**
     * @brief Checks if Expression contains any variables. 
     * @return true if any Var node exists inside the expression. 
     */
    virtual bool has_variable() = 0; // checks if expression has variables ("X") = true

    /**
     * @brief Substitute occurrences of a variable with another expression
     * @param variable name to replace with another Expr
     * @param replacement the expression to substitute in place of that variable. 
     * @return A (possibly) new expression tree with substitute applied.
     */
    virtual Expr* subst(std::string, Expr*) = 0; // replaces variable with expression

    /**
     * @brief Print the expression to an output stream using concrete syntax
     * @param out OutPut stream 
     */
    virtual void printExp (std::ostream& out) = 0; // print to stream instead of screen
};

/** 
 * @brief Numeric literal expression (leaf node)
*/
class Num : public Expr {
public:
    int val; /*!< Store Numeric Value*/

    /** 
     * @brief Construct a numeric literal.
     * @param val value to store
    */
    Num(int val);

    bool equals(Expr* e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string, Expr*) override;
    void printExp (std::ostream& out) override; 

};


/** 
 * @brief Variable expression (leaf node) such as "x"
 */
class Var : public Expr {
public:
    // variable name
    std::string name;

    /**
     * @brief Constructor a variable
     * @param name variable name 
     */
    Var(std::string name);

    bool equals(Expr* e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string, Expr*) override;
    void printExp (std::ostream& out) override; 

};

/**
 * @brief Addition expression: (lhs + rhs).
 */
class Add : public Expr {
public:
    Expr* lhs; /**< left */
    Expr* rhs; /**< right*/

    /**
     * @brief Construct an addition node.
     */
    Add(Expr* lhs, Expr* rhs);

    bool equals(Expr* e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string, Expr*) override;
    void printExp (std::ostream& out) override; 

};

/**
 * @brief Multiplication expression: (lhs * rhs).
 */
class Mult : public Expr {
public:
    Expr* lhs; // left
    Expr* rhs; // right

    /**
     * @brief Construct a multiplication node.
     */
     Mult(Expr* lhs, Expr* rhs);

    bool equals(Expr* e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string, Expr*) override;
    void printExp (std::ostream& out) override; 

};


