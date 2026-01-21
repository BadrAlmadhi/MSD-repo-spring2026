#include "catch.h"
#include "expr.h"


// TEST_CASE("Description")
TEST_CASE("Var equals examples") {
    // CHECK = works like assert. 
    CHECK((new Var("x"))->equals(new Var("x")) == true);
    CHECK((new Var("x"))->equals(new Var("y")) == false);
    CHECK((new Num(1))->equals(new Var("x")) == false);
}

// same concept
TEST_CASE("Num equals") {
    CHECK((new Num(5))->equals(new Num(5)) == true);
    CHECK((new Num(5))->equals(new Num(6)) == false);
    CHECK((new Num(5))->equals(new Add(new Num(1), new Num(2))) == false);

}

TEST_CASE("Add equals examples") {

    CHECK((new Add(new Num(2), new Num(3)))
    ->equals(new Add(new Num(2), new Num(3))) == true);

    CHECK((new Add(new Num(2), new Num(3)))
     ->equals(new Add(new Num(3), new Num(2))) == false);

    CHECK((new Add(new Num(2), new Num(3)))
    ->equals(new Mult(new Num(2), new Num(3))) == false);
}

TEST_CASE("Mult equals") {
    CHECK((new Mult(new Num(2), new Num(3)))
    ->equals(new Mult(new Num(2), new Num(3))) == true);

    CHECK((new Mult(new Num(2), new Num(3)))
    ->equals(new Mult(new Num(3), new Num(2))) == false);

    CHECK((new Mult(new Num(2), new Num(3)))
    ->equals(new Add(new Num(2), new Num(3))) == false);
}

TEST_CASE("Nested expressions equals") {
    Expr* a = new Add(new Num(1), new Mult(new Var("x"), new Num(5)));
    Expr* b = new Add(new Num(1), new Mult(new Var("x"), new Num(5)));
    Expr* c = new Add(new Num(1), new Mult(new Var("y"), new Num(5)));

    CHECK(a->equals(b) == true);
    CHECK(a->equals(c) == false);
}
