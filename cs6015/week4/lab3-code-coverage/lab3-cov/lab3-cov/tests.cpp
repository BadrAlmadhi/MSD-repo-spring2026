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


TEST_CASE("interp: numbers, add, and mult") {
    CHECK((new Num(5))->interp() == 5);
    CHECK((new Mult(new Num(3), new Num(2)))->interp() == 6);
    CHECK((new Add (new Add(new Num(10), new Num(15)), new Add(new Num(20), new Num(20))))->interp() == 65);
}

TEST_CASE("interp: variables throws") {
    CHECK_THROWS_WITH((new Var("x"))->interp(), "Variable cannot be interpreted without a value.");
}

TEST_CASE("has_variable") {
    CHECK((new Var("x"))->has_variable() == true);
    CHECK((new Num(1))->has_variable() == false);
    CHECK( (new Add( new Var("x"), new Num(1) ) )->has_variable() == true);
    CHECK( (new Mult( new Num(2), new Num(1) ))->has_variable() == false );
    CHECK( (new Mult(new Num(2), new Add(new Num(1), new Var("z"))))->has_variable() == true );

}

TEST_CASE("subst: replaces matching variables") {
    CHECK( (new Add(new Var("x"), new Num(7)))
           ->subst("x", new Var("y"))
           ->equals(new Add(new Var("y"), new Num(7))) );

    CHECK( (new Var("x"))
           ->subst("x", new Add(new Var("y"), new Num(7)))
           ->equals(new Add(new Var("y"), new Num(7))) );

    CHECK( (new Mult(new Var("x"), new Num(2)))
           ->subst("x", new Num(5))
           ->equals(new Mult(new Num(5), new Num(2))) );
}


TEST_CASE("subst: no replacement when variable name doesn't match") {
    // if your Var::subst returns this for non-match, this still passes because equals() is structural
    CHECK( (new Var("x"))
           ->subst("y", new Num(99))
           ->equals(new Var("x")) );

    CHECK( (new Add(new Var("x"), new Num(1)))
           ->subst("y", new Num(99))
           ->equals(new Add(new Var("x"), new Num(1))) );
}
