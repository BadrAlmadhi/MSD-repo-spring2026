
#define CATCH_CONFIG_MAIN

#include "catch.h"      // must match your actual filename
#include "expr.hpp"     // your project header
#include "val.h"
#include "parse.hpp"
#include "pointer.h"
#include <climits>

TEST_CASE("Check if values are equals") {
    CHECK( (NEW(ExprNum)(3))->equals(NEW(ExprNum)(3)) == true);
    CHECK( (NEW(ExprAdd)(NEW(ExprNum)(3), NEW(ExprNum)(3)))
    ->equals(NEW(ExprAdd)(NEW(ExprNum)(3), NEW(ExprNum)(3))) == true);
    CHECK( (NEW(ExprMult)(NEW(ExprNum)(3), NEW(ExprNum)(3)))
    ->equals(NEW(ExprMult)(NEW(ExprNum)(3), NEW(ExprNum)(3))) == true);
    CHECK( (NEW(ExprVar)("X"))->equals(NEW(ExprVar)("X")) == true);
}

TEST_CASE("Check Equals with Wrong values") {
    CHECK( (NEW(ExprNum)(2))->equals(NEW(ExprNum)(3)) == false);
    CHECK( (NEW(ExprAdd)(NEW(ExprNum)(10), NEW(ExprNum)(9)))
    ->equals(NEW(ExprAdd)(NEW(ExprNum)(4), NEW(ExprNum)(3))) == false);
    CHECK( (NEW(ExprMult)(NEW(ExprNum)(3), NEW(ExprNum)(12)))
    ->equals(NEW(ExprMult)(NEW(ExprNum)(23), NEW(ExprNum)(10))) == false);
    CHECK( (NEW(ExprVar)("Y"))->equals(NEW(ExprVar)("X")) == false);
}

TEST_CASE("Mult/Add order") {
    CHECK( (NEW(ExprAdd)(NEW(ExprNum)(2), NEW(ExprNum)(4)))
    ->equals(NEW(ExprAdd)(NEW(ExprNum)(4), NEW(ExprNum)(2))) == false);
    CHECK( (NEW(ExprMult)(NEW(ExprNum)(6), NEW(ExprNum)(9)))
    ->equals(NEW(ExprMult)(NEW(ExprNum)(9), NEW(ExprNum)(6))) == false);
}

TEST_CASE("Wrong type in Add and Mult") {
    CHECK( (NEW(ExprAdd)(NEW(ExprNum)(2), NEW(ExprNum)(4)))
    ->equals(NEW(ExprMult)(NEW(ExprNum)(4), NEW(ExprNum)(2))) == false);
    CHECK( (NEW(ExprMult)(NEW(ExprNum)(6), NEW(ExprNum)(9)))
    ->equals(NEW(ExprAdd)(NEW(ExprNum)(9), NEW(ExprNum)(6))) == false);    
}

TEST_CASE("Tets interp 'Total sum of an expression' if return sum value") {
    CHECK( (NEW(ExprNum)(3))->interp()->equals(NEW(NumVal)(3)));
    CHECK( (NEW(ExprAdd)(NEW(ExprNum)(3), NEW(ExprNum)(3)))->interp()->equals(NEW(NumVal)(6)));
    CHECK( (NEW(ExprMult)(NEW(ExprNum)(10), NEW(ExprNum)(10)))->interp()->equals(NEW(NumVal)(100)));
    CHECK_THROWS_WITH((NEW(ExprVar)("X"))->interp(), "free variable: X");
}

TEST_CASE("Nested expressions") {
   CHECK(
    (NEW(ExprAdd)(NEW(ExprAdd)(NEW(ExprNum)(10), NEW(ExprNum)(15)),
   NEW(ExprAdd)(NEW(ExprNum)(20), NEW(ExprNum)(20))
))->interp()->equals(NEW(NumVal)(65)));

CHECK(
  (NEW(ExprMult)(
      NEW(ExprAdd)(NEW(ExprNum)(2), NEW(ExprNum)(3)),
      NEW(ExprNum)(4)
  ))->interp()->equals(NEW(NumVal)(20))
);
}

TEST_CASE("PrintExpr example ") {
    CHECK((NEW(ExprNum)(10))->to_string() == "10");
    CHECK((NEW(ExprVar)("x"))->to_string() == "x");
    CHECK((NEW(ExprAdd)(NEW(ExprNum)(1), NEW(ExprNum)(2)))->to_string() == "(1+2)");
    CHECK((NEW(ExprMult)(NEW(ExprNum)(2), NEW(ExprAdd)(NEW(ExprNum)(3), NEW(ExprNum)(4))))
      ->to_string() == "(2*(3+4))");
}

TEST_CASE("pretty print examples") {
    CHECK((NEW(ExprMult)(NEW(ExprNum)(1), NEW(ExprAdd)(NEW(ExprNum)(2), NEW(ExprNum)(3))))
          ->to_pretty_string() == "1 * (2 + 3)");

    CHECK((NEW(ExprMult)(NEW(ExprMult)(NEW(ExprNum)(8), NEW(ExprNum)(1)), NEW(ExprVar)("y")))
          ->to_pretty_string() == "(8 * 1) * y");

    CHECK((NEW(ExprMult)(NEW(ExprAdd)(NEW(ExprNum)(3), NEW(ExprNum)(5)),
                        NEW(ExprMult)(NEW(ExprNum)(6), NEW(ExprNum)(1))))
          ->to_pretty_string() == "(3 + 5) * 6 * 1");

    CHECK((NEW(ExprMult)(NEW(ExprMult)(NEW(ExprNum)(7), NEW(ExprNum)(7)),
                        NEW(ExprAdd)(NEW(ExprNum)(9), NEW(ExprNum)(2))))
          ->to_pretty_string() == "(7 * 7) * (9 + 2)");
}

TEST_CASE("Let printExpr to_string") {
    CHECK(
      (NEW(ExprLet)("x", NEW(ExprNum)(5),
          NEW(ExprAdd)(
            NEW(ExprLet)("y", NEW(ExprNum)(3),
              NEW(ExprAdd)(NEW(ExprVar)("y"), NEW(ExprNum)(2))),
            NEW(ExprVar)("x")
          )))->to_string()
      ==
      "(_let x=5 _in ((_let y=3 _in (y+2))+x))"
    );
}

TEST_CASE("Let pretty_print") {
    CHECK(
      (NEW(ExprLet)("x", NEW(ExprNum)(5),
          NEW(ExprAdd)(
            NEW(ExprLet)("y", NEW(ExprNum)(3),
              NEW(ExprAdd)(NEW(ExprVar)("y"), NEW(ExprNum)(2))),
            NEW(ExprVar)("x")
          )))->to_pretty_string() == "_let x = 5\n_in  (_let y = 3\n      _in  y + 2) + x"
    );
}

TEST_CASE("Function interp") {
    CHECK(parse_str("_let f = _fun (x) x + 1 _in f(5)")->interp()->equals(NEW(NumVal)(6)));
    CHECK(parse_str("_let f = _fun (x) x * 2 _in f(4)")->interp()->equals(NEW(NumVal)(8)));
}

TEST_CASE("Function print") {
    CHECK(parse_str("_fun (x) x + 1")->to_string() == "(_fun (x) (x+1))");
    CHECK(parse_str("_let f = _fun (x) x + 1 _in f(5)")->to_string()
          == "(_let f=(_fun (x) (x+1)) _in f(5))");
}

TEST_CASE("Function pretty print") {
    CHECK(parse_str("_fun (x) _if x == 1 _then 1 _else x + 2")->to_pretty_string()
          == "_fun (x)\n  _if x == 1\n  _then 1\n  _else x + 2");
}

TEST_CASE("BoolExpr basics") {
    CHECK((NEW(BoolExpr)(true))->interp()->equals(NEW(BoolVal)(true)));
    CHECK((NEW(BoolExpr)(false))->interp()->equals(NEW(BoolVal)(false)));
    CHECK((NEW(BoolExpr)(true))->to_string() == "_true");
    CHECK((NEW(BoolExpr)(false))->to_string() == "_false");
}

TEST_CASE("EqExpr interp") {
    CHECK((NEW(EqExpr)(NEW(ExprNum)(5), NEW(ExprNum)(5)))->interp()->equals(NEW(BoolVal)(true)));
    CHECK((NEW(EqExpr)(NEW(ExprNum)(5), NEW(ExprNum)(6)))->interp()->equals(NEW(BoolVal)(false)));
}

TEST_CASE("IfExpr interp") {
    CHECK((NEW(IfExpr)(NEW(BoolExpr)(true), NEW(ExprNum)(1), NEW(ExprNum)(2)))
          ->interp()->equals(NEW(NumVal)(1)));

    CHECK((NEW(IfExpr)(NEW(BoolExpr)(false), NEW(ExprNum)(1), NEW(ExprNum)(2)))
          ->interp()->equals(NEW(NumVal)(2)));

    CHECK_THROWS_WITH((NEW(IfExpr)(NEW(ExprNum)(5), NEW(ExprNum)(1), NEW(ExprNum)(2)))->interp(),
                      "NumVal has no truth value");
}

TEST_CASE("subst basics") {
    CHECK((NEW(ExprVar)("x")->subst("x", NEW(ExprNum)(5)))->equals(NEW(ExprNum)(5)));
    CHECK((NEW(ExprVar)("y")->subst("x", NEW(ExprNum)(5)))->equals(NEW(ExprVar)("y")));
    CHECK((NEW(ExprAdd)(NEW(ExprVar)("x"), NEW(ExprNum)(2))
          ->subst("x", NEW(ExprNum)(5)))
          ->equals(NEW(ExprAdd)(NEW(ExprNum)(5), NEW(ExprNum)(2))));
}

TEST_CASE("subst with let shadowing") {
    CHECK((NEW(ExprLet)("x", NEW(ExprNum)(10), NEW(ExprVar)("x"))
          ->subst("x", NEW(ExprNum)(5)))
          ->equals(NEW(ExprLet)("x", NEW(ExprNum)(10), NEW(ExprVar)("x"))));
}

TEST_CASE("parse errors") {
    CHECK_THROWS(parse_str("1 +"));
    CHECK_THROWS(parse_str("_let x = 5 x + 2"));
}

TEST_CASE("FunVal basics") {
    CHECK((NEW(FunExpr)("x", NEW(ExprAdd)(NEW(ExprVar)("x"), NEW(ExprNum)(1))))
          ->interp()->to_string() == "[function]");
}

TEST_CASE("overflow checks") {
    CHECK_THROWS_WITH((NEW(ExprAdd)(NEW(ExprNum)(INT_MAX), NEW(ExprNum)(1)))->interp(),
                      "integer overflow in addition");

    CHECK_THROWS_WITH((NEW(ExprMult)(NEW(ExprNum)(INT_MAX), NEW(ExprNum)(2)))->interp(),
                      "integer overflow in multiplication");
}