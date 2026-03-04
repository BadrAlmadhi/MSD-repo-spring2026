#include "catch.h" 
#include "val.h"

TEST_CASE("NumVal basic") {
    Val* v1 = new NumVal(5);
    Val* v2 = new NumVal(5);
    Val* v3 = new NumVal(6);

    CHECK(v1->equals(v2));
    CHECK_FALSE(v1->equals(v3));

    CHECK_FALSE(v1->equals(nullptr));

    Val* sum = v1->add_to(new NumVal(7));
    CHECK(sum->equals(new NumVal(12)));

    Val* mult = v1->mult_with(new NumVal(5));
    CHECK(mult->equals(new NumVal(25)));

    CHECK(v1->to_string() == "5");
}

