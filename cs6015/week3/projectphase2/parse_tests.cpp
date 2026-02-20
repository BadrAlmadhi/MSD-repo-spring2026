/* Some ideas for parse tests, probably not enough. Also, you may need
   to adjust these tests, and there may even be mistakes here. */

#include "catch.h"
#include "expr.hpp"
#include "parse.hpp"

#include "catch.h"
#include "expr.hpp"
#include "parse.hpp"

TEST_CASE("parse") {
  CHECK_THROWS(parse_str("()"));

  CHECK(parse_str("(1)")->equals(new ExprNum(1)));
  CHECK(parse_str("(((1)))")->equals(new ExprNum(1)));

  CHECK_THROWS(parse_str("(1"));

  CHECK(parse_str("1")->equals(new ExprNum(1)));
  CHECK(parse_str("10")->equals(new ExprNum(10)));
  CHECK(parse_str("  \n 5  ")->equals(new ExprNum(5)));

  CHECK(parse_str("x")->equals(new ExprVar("x")));
  CHECK(parse_str("xyz")->equals(new ExprVar("xyz")));

  CHECK(parse_str("x + y")
        ->equals(new ExprAdd(new ExprVar("x"), new ExprVar("y"))));

  CHECK(parse_str("x * y")
        ->equals(new ExprMult(new ExprVar("x"), new ExprVar("y"))));

  CHECK(parse_str("z * x + y")
        ->equals(new ExprAdd(new ExprMult(new ExprVar("z"), new ExprVar("x")),
                             new ExprVar("y"))));

  CHECK(parse_str("z * (x + y)")
        ->equals(new ExprMult(new ExprVar("z"),
                              new ExprAdd(new ExprVar("x"), new ExprVar("y")))));
}
