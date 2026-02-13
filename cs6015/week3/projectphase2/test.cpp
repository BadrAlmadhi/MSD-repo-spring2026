#include "catch.h"      // must match your actual filename
#include "expr.hpp"     // your project header

TEST_CASE("Check if values are equals") {
    CHECK( (new ExprNum(3))->equals(new ExprNum(3)) == true);
    CHECK( (new ExprAdd(new ExprNum(3), new ExprNum(3)))
    ->equals(new ExprAdd(new ExprNum(3), new ExprNum(3))) == true);
    CHECK( (new ExprMult(new ExprNum(3), new ExprNum(3)))
    ->equals(new ExprMult(new ExprNum(3), new ExprNum(3))) == true);
    CHECK( (new ExprVar("X"))->equals(new ExprVar("X")) == true);
}

TEST_CASE("Check Equals with Wrong values") {
    CHECK( (new ExprNum(2))->equals(new ExprNum(3)) == false);
    CHECK( (new ExprAdd(new ExprNum(10), new ExprNum(9)))
    ->equals(new ExprAdd(new ExprNum(4), new ExprNum(3))) == false);
    CHECK( (new ExprMult(new ExprNum(3), new ExprNum(12)))
    ->equals(new ExprMult(new ExprNum(23), new ExprNum(10))) == false);
    CHECK( (new ExprVar("Y"))->equals(new ExprVar("X")) == false);
}

TEST_CASE("Mult/Add order") {
    CHECK( (new ExprAdd(new ExprNum(2), new ExprNum(4)))
    ->equals(new ExprAdd(new ExprNum(4), new ExprNum(2))) == false);
    CHECK( (new ExprMult(new ExprNum(6), new ExprNum(9)))
    ->equals(new ExprMult(new ExprNum(9), new ExprNum(6))) == false);
}

TEST_CASE("Wrong type in Add and Mult") {
    CHECK( (new ExprAdd(new ExprNum(2), new ExprNum(4)))
    ->equals(new ExprMult(new ExprNum(4), new ExprNum(2))) == false);
    CHECK( (new ExprMult(new ExprNum(6), new ExprNum(9)))
    ->equals(new ExprAdd(new ExprNum(9), new ExprNum(6))) == false);    
}



TEST_CASE("Tets interp 'Total sum of an expression' if return sum value") {
    CHECK( (new ExprNum(3))->interp() == 3);
    CHECK( (new ExprAdd(new ExprNum(3), new ExprNum(3)))->interp() == 6);
    CHECK( (new ExprMult(new ExprNum(10), new ExprNum(10)))->interp() == 100);
    CHECK_THROWS_WITH( (new ExprVar("X"))->interp(), "This expression cannot be evaluated because it contain a variable");
}

TEST_CASE("Nested expressions") {
   CHECK(
    (new ExprAdd(new ExprAdd(new ExprNum(10), new ExprNum(15)),
   new ExprAdd(new ExprNum(20), new ExprNum(20))
))->interp() == 65
);

CHECK(
  (new ExprMult(
      new ExprAdd(new ExprNum(2), new ExprNum(3)),
      new ExprNum(4)
  ))->interp() == 20
);
}

TEST_CASE("PrintExpr example ") {
    CHECK((new ExprNum(10))->to_string() == "10");
    CHECK((new ExprVar("x"))->to_string() == "x");
    CHECK((new ExprAdd(new ExprNum(1), new ExprNum(2)))->to_string() == "(1+2)");
    CHECK((new ExprMult(new ExprNum(2), new ExprAdd(new ExprNum(3), new ExprNum(4))))
      ->to_string() == "(2*(3+4))");
}

TEST_CASE("pretty print examples") {
    CHECK((new ExprMult(new ExprNum(1), new ExprAdd(new ExprNum(2), new ExprNum(3))))
          ->to_pretty_string() == "1 * (2 + 3)");

    CHECK((new ExprMult(new ExprMult(new ExprNum(8), new ExprNum(1)), new ExprVar("y")))
          ->to_pretty_string() == "(8 * 1) * y");

    CHECK((new ExprMult(new ExprAdd(new ExprNum(3), new ExprNum(5)),
                        new ExprMult(new ExprNum(6), new ExprNum(1))))
          ->to_pretty_string() == "(3 + 5) * 6 * 1");

    CHECK((new ExprMult(new ExprMult(new ExprNum(7), new ExprNum(7)),
                        new ExprAdd(new ExprNum(9), new ExprNum(2))))
          ->to_pretty_string() == "(7 * 7) * (9 + 2)");
}

TEST_CASE("Let printExpr to_string") {
    CHECK(
      (new ExprLet("x", new ExprNum(5),
          new ExprAdd(
            new ExprLet("y", new ExprNum(3),
              new ExprAdd(new ExprVar("y"), new ExprNum(2))),
            new ExprVar("x")
          )))->to_string()
      ==
      "(_let x=5 _in ((_let y=3 _in (y+2))+x))"
    );
}

TEST_CASE("Let has_variable") {
    CHECK((new ExprLet("x", new ExprNum(5), new ExprNum(1)))->has_variable() == false);
    CHECK((new ExprLet("x", new ExprNum(5), new ExprVar("x")))->has_variable() == true);
}

TEST_CASE("Let pretty_print") {
    CHECK(
      (new ExprLet("x", new ExprNum(5),
          new ExprAdd(
            new ExprLet("y", new ExprNum(3),
              new ExprAdd(new ExprVar("y"), new ExprNum(2))),
            new ExprVar("x")
          )))->to_pretty_string() == "_let x = 5\n_in  (_let y = 3\n      _in  y + 2) + x"
    );
}